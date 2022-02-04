#include "codegen.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {

    /* Example code 3
        A = malloc(4);
        B = malloc(4);
        C = malloc(1);
        C[0] = A[0] * B[0] +
                A[1] * B[1] + 
                A[2] * B[2] + 
                A[3] * B[3];
    */

    //The above program in ast format
    //We have 4 statements
    stmt *stmt_list[4];

    //We have 3 memory segments
    memalloc *mem_list[3];

    //We have one write
    write *w;

    //We have 8 reads
    read *r[8];

    //We have 15 expressions 
    expr *e[15];

    //And corresponding 8 expr leaves
    expr_leaf *leaves[8];

    //First set the memallocs
    mem_list[0] = set_memalloc("A", 4);
    mem_list[1] = set_memalloc("B", 4);
    mem_list[2] = set_memalloc("C", 1);

    for(int i = 0; i < 3; i++) {
        stmt_list[i] = set_stmt(mem_list[i], 0);
        //ast_list[i] = set_ast_node(stmt_list[i], 0);
    }

    r[0] = set_read("A", 0);
    r[1] = set_read("B", 0);
    r[2] = set_read("A", 1);
    r[3] = set_read("B", 1);
    r[4] = set_read("A", 2);
    r[5] = set_read("B", 2);
    r[6] = set_read("A", 3);
    r[7] = set_read("B", 3);

    for(int i = 0; i < 8; i++) {
        leaves[i] = set_exp_leaf(r[i], 0);
        e[i] = set_expr('v', NULL, NULL, leaves[i]);
    }


    e[8] = set_expr('*', e[0], e[1], NULL);
    e[9] = set_expr('*', e[2], e[3], NULL);
    e[10] = set_expr('*', e[4], e[5], NULL);
    e[11] = set_expr('*', e[6], e[7], NULL);

    e[12] = set_expr('+', e[8], e[9], NULL);
    e[13] = set_expr('+', e[12], e[10], NULL);
    e[14] = set_expr('+', e[13], e[11], NULL);

    w = set_write("C", 0, e[14]);
    stmt_list[3] = set_stmt(w, 1);

    //set stmt block
    stmt *ast_ex3 = set_block(stmt_list, 4);


    //pretty_print_prog(ast_ex3);

    basic_block *ex3 = blck_create_from_ast(ast_ex3);

    char **mem_count = get_mem_blocks_from_ast(ast_ex3);

    int clk = schedule_cdfg(ex3, mem_count, 0);

    //print_schedule(ex3->dfgs[0]);

    int add_cnt = get_max_res_blck(ex3, 'A');
    int mul_cnt = get_max_res_blck(ex3, 'M');
    int reg_cnt = get_max_reg_cnt(ex3);

    //printf("A %d M %d R %d\n", add_cnt, mul_cnt, reg_cnt);
    
    reg **list = reg_alloc(list, reg_cnt, ex3);
    resrc **add = inst_upd(add_cnt, 'A', ex3);
    resrc **mul = inst_upd(mul_cnt, 'M', ex3);

    /*
    for(int i =0; list[i] != NULL; i++) {
        printf("R %d\n", list[i]->reg_number);
    }
    */

    mux **add_muxes = get_mux_for_resrc(add, list);
    mux **mul_muxes = get_mux_for_resrc(mul, list);

    /*
    for(int i = 0; add_muxes[i] != NULL; i++) {
        printf("Mux %d\n", i);
        for(int j = 0; add_muxes[i]->inputs[j] != NULL; j++) {
            printf("Reg %d \n", add_muxes[i]->inputs[j]->reg_number);
        }
        printf("\n");
    }
    */

    //printf("Reg %d \n", add_muxes[1]->inputs[0]->reg_number);


    gen_vhdl(ex3, add_muxes, mul_muxes, add, mul, mem_count, list);
}