#include "codegen.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main () {


    /*Sample program 2
        A = malloc(4);
        B = malloc(4);
        C = malloc(4);
        C[0] = A[0] * B[0];
        C[1] = A[1] * B[1];
        C[2] = A[2] * B[2];
        C[3] = A[3] * B[3];
    */

    //We have 3 memory allocs, 4 stores, 8 reads, 12 expr and 7 astnodes
    ast_node **ast_list = malloc(sizeof(ast_node*) * 7);
    
    stmt **stmt_list = malloc(sizeof(stmt*) * 7);
    
    memalloc **mem_list = malloc(sizeof(memalloc *) * 3);
    
    write **w = malloc(sizeof(write*) * 4);
    
    read **r = malloc(sizeof(read*) * 8);
 
    expr **e = malloc(sizeof(expr *) * 12);
   
    //for each read/const, we need an expr leaf 
    expr_leaf **leaves = malloc(sizeof(expr_leaf *) * 8);
   
    //Set the first 3 memallocs ast
    mem_list[0] = set_memalloc("A", 4);
    mem_list[1] = set_memalloc("B", 4);
    mem_list[2] = set_memalloc("C", 4);

    for(int i = 0; i < 3; i++) {
        stmt_list[i] = set_stmt(mem_list[i], 0);
    }

    //Set the reads
    r[0] = set_read("A", 0);
    r[1] = set_read("B", 0);
    r[2] = set_read("A", 1);
    r[3] = set_read("B", 1);
    r[4] = set_read("A", 2);
    r[5] = set_read("B", 2);
    r[6] = set_read("A", 3);
    r[7] = set_read("B", 3);
    
    //Now set the expr_leaves
    for(int i = 0; i < 8; i++) {
        leaves[i] = set_exp_leaf(r[i], 0);
        e[i] = set_expr('v', NULL, NULL, leaves[i]);
    }
    
    e[8] = set_expr('*', e[0], e[1], NULL);
    e[9] = set_expr('*', e[2], e[3], NULL);
    e[10] = set_expr('*', e[4], e[5], NULL);
    e[11] = set_expr('*', e[6], e[7], NULL);

    //Now set the stores as ast_nodes
    
    for(int i = 0; i < 4; i++) {
        w[i] = set_write("C", i, e[8+i]);
        stmt_list[3+i] = set_stmt(w[i], 1);
        //ast_list[3+i] = set_ast_node(stmt_list[3+i], 1);
    } 
    
    stmt *ast_ex2 = set_block(stmt_list, 7);

    /*
    pretty_print_prog(ast_ex2);
    */
    //Now onto basic block
    
    basic_block *ex2 = blck_create_from_ast(ast_ex2);
    char **mem_count = get_mem_blocks_from_ast(ast_ex2);

    /*
    for(int i = 0; i < 7; i++) {
        if(mem_count[i] == NULL)
            break;
        printf("%s ", mem_count[i]);
    }
    printf("\n");
    */
    //Pretty printing the dfg
    //pretty_print_blck(ex2);

    //for(int i = 0; i < ex2->loc; i ++)
     //   init_schd(ex2->dfgs[i]);
    
    int clk = schedule_cdfg(ex2, mem_count, 0);

    /*
    for(int i = 0; i < ex2->loc; i ++)
        print_schedule(ex2->dfgs[i]);
    */

    int adder_cnt = get_max_res_blck(ex2, 'A');
    int mult_cnt = get_max_res_blck(ex2, 'M');

    //printf("A %d M %d \n", adder_cnt, mult_cnt);

    
    int reg_cnt = get_max_reg_cnt(ex2);
    //printf("Reg %d\n", reg_cnt);

    reg **list = reg_alloc(list, reg_cnt, ex2);

    int add_cnt = node_count_type(ex2->dfgs[0], 'M');
    
    resrc **add2 = inst_upd(adder_cnt, 'A', ex2);
    resrc **mult2 = inst_upd(mult_cnt, 'M', ex2);

    
    
    mux **add_muxes = get_mux_for_resrc(add2, list);
    mux **mul_muxes = get_mux_for_resrc(mult2, list);

    /*
    for(int i = 0; mul_muxes[i] != NULL; i++) {
        printf("Mux %d\n", i);
        for(int j = 0; mul_muxes[i]->inputs[j] != NULL; j++) {
            printf("Reg %d ", mul_muxes[i]->inputs[j]->reg_number);
        }
        printf("\n");
    }
    */
    
   gen_vhdl(ex2, add_muxes, mul_muxes, add2, mult2, mem_count, list);
}