#include"codegen.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {

    /* Example code 5

        A = malloc(4);
        A[0] = 1;
        A[1] = 2;
        A[2] = 3;
        A[3] = 4;
    */

   
    //We have 5 statements 
    stmt *stmt_list[5];

    //One of memalloc
    memalloc *mem_list[1];

    //4 are writes
    write *w[4];

    //We have total 4 expressions 
    expr *e[4];

    //Each expression is a leaf of cosntants 
    expr_leaf *leaves[4];

    //First set memalloc
    mem_list[0] = set_memalloc("A", 4);
    stmt_list[0] = set_stmt(mem_list[0], 0);

       
    //Set the leaves and the expressions now
    int j = 1; 
    for(int i = 0; i < 4; i++) {
        leaves[i] = set_exp_leaf(&(j), 1);
        e[i] = set_expr('c', NULL, NULL, leaves[i]);
    }
    
    
    //Now set the writes
    for(int i = 0; i < 4; i++) {
        w[i] = set_write("A", 0, e[i]);
    }

    stmt_list[1] = set_stmt(w[0], 1);
    stmt_list[2] = set_stmt(w[1], 1);
    stmt_list[3] = set_stmt(w[2], 1);
    stmt_list[4] = set_stmt(w[3], 1);

    stmt *ast_ex5 = set_block(stmt_list, 5);

    basic_block *ex5 = blck_create_from_ast(ast_ex5);
    char **mem_count = get_mem_blocks_from_ast(ast_ex5);

    int clk = schedule_cdfg(ex5, mem_count, 0);
   
    print_schedule(ex5->dfgs[0]);
    print_schedule(ex5->dfgs[1]);
    print_schedule(ex5->dfgs[2]);
    print_schedule(ex5->dfgs[3]);

    int reg_cnt = get_max_reg_cnt(ex5);

    reg **list = reg_alloc(list, reg_cnt, ex5);

    for(int i = 0; list[i] != NULL; i++) {
        printf("R %d \t", list[i]->reg_number);
    }
    printf("\n");



    int add_cnt = get_max_res_blck(ex5, 'A');
    int mul_cnt = get_max_res_blck(ex5, 'M');

    //pretty_print_blck(ex4);

    printf("A %d M %d\n", add_cnt, mul_cnt);
    
    resrc **add = inst_upd(add_cnt, 'A', ex5);
    resrc **mul = inst_upd(mul_cnt, 'M', ex5);

    mux **add_muxes = get_mux_for_resrc(add, list);
    mux **mul_muxes = get_mux_for_resrc(mul, list);

    gen_vhdl(ex5, add_muxes, mul_muxes, add, mul, mem_count, list);
    
}