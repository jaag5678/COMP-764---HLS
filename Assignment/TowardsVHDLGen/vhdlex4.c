#include "codegen.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main () {

    /* Example code 4
        A = malloc(6);
        W = malloc(3);
        C = malloc(4);
        C[0] = A[0] * W[0] + A[1] * W[1] + A[2] * W[2];
        C[1] = A[1] * W[0] + A[2] * W[1] + A[3] * W[2];
        C[2] = A[2] * W[0] + A[3] * W[1] + A[4] * W[2];
        C[3] = A[3] * W[0] + A[4] * W[1] + A[5] * W[2];
    */

    //We have 7 statements 
    stmt *stmt_list[7];

    //Of which 3 are memallocs
    memalloc *mem_list[3];

    //And 4 are writes
    write *w[4];

    //We have 24 reads in total (6 for each write)
    read *r[24];

    //We have in total 44 expressions 
    expr *e[44];

    //Each read corresponds to a leaf, so we have 24 leaves
    expr_leaf *leaves[24];

    //First set memlallocs
    mem_list[0] = set_memalloc("A", 6);
    mem_list[1] = set_memalloc("W", 3);
    mem_list[2] = set_memalloc("C", 4);

    for(int i = 0; i < 3; i++) {
        stmt_list[i] = set_stmt(mem_list[i], 0);
    }

    //Set the reads and their corresponding leaves
    r[0] = set_read("A", 0);
    r[1] = set_read("W", 0);
    r[2] = set_read("A", 1);
    r[3] = set_read("W", 1);
    r[4] = set_read("A", 2);
    r[5] = set_read("W", 2);
    r[6] = set_read("A", 1);
    r[7] = set_read("W", 0);
    r[8] = set_read("A", 2);
    r[9] = set_read("W", 1);
    r[10] = set_read("A", 3);
    r[11] = set_read("W", 2);
    r[12] = set_read("A", 2);
    r[13] = set_read("W", 0);
    r[14] = set_read("A", 3);
    r[15] = set_read("W", 1);
    r[16] = set_read("A", 4);
    r[17] = set_read("W", 2);
    r[18] = set_read("A", 3);
    r[19] = set_read("W", 0);
    r[20] = set_read("A", 4);
    r[21] = set_read("W", 1);
    r[22] = set_read("A", 5);
    r[23] = set_read("W", 2);

    for(int i = 0; i < 24; i++) {
        leaves[i] = set_exp_leaf(r[i], 0);
        e[i] = set_expr('v', NULL, NULL, leaves[i]);
    }

    for(int i = 24; i < 36; i++) {
        e[i] = set_expr('*', e[(i-24)*2], e[(i-24)*2 + 1], NULL);
    }

    e[36] = set_expr('+', e[24], e[25], NULL);
    e[37] = set_expr('+', e[36], e[26], NULL);
    e[38] = set_expr('+', e[27], e[28], NULL);
    e[39] = set_expr('+', e[38], e[29], NULL);
    e[40] = set_expr('+', e[30], e[31], NULL);
    e[41] = set_expr('+', e[40], e[32], NULL);
    e[42] = set_expr('+', e[33], e[34], NULL);
    e[43] = set_expr('+', e[42], e[35], NULL);

    w[0] = set_write("C", 0, e[37]);
    w[1] = set_write("C", 1, e[39]);
    w[2] = set_write("C", 2, e[41]);
    w[3] = set_write("C", 3, e[43]);

    for(int i = 3; i < 7; i++) {
        stmt_list[i] = set_stmt(w[i-3], 1);
    }

    stmt *ast_ex4 = set_block(stmt_list, 7);

    //pretty_print_prog(ast_ex4);


    basic_block *ex4 = blck_create_from_ast(ast_ex4);
    char **mem_count = get_mem_blocks_from_ast(ast_ex4);

    

    int clk = schedule_cdfg(ex4, mem_count, 0);
    
    /*
    print_schedule(ex4->dfgs[0]);
    print_schedule(ex4->dfgs[1]);
    print_schedule(ex4->dfgs[2]);
    print_schedule(ex4->dfgs[3]);
    */

    
    //printf("A %d M %d R %d\n", add_cnt, mul_cnt, reg_cnt);
    
    int reg_cnt = get_max_reg_cnt(ex4);

    reg **list = reg_alloc(list, reg_cnt, ex4);

    //cse(list);
    
    for(int i = 0; i < ex4->loc; i++) {
        init_schd(ex4->dfgs[i]);
    }

    schedule_cdfg(ex4, mem_count, 0);
    
    /*
    print_schedule(ex4->dfgs[0]);
    print_schedule(ex4->dfgs[1]);
    print_schedule(ex4->dfgs[2]);
    print_schedule(ex4->dfgs[3]);
    */
    int add_cnt = get_max_res_blck(ex4, 'A');
    int mul_cnt = get_max_res_blck(ex4, 'M');

    //pretty_print_blck(ex4);

    //printf("A %d M %d\n", add_cnt, mul_cnt);
    
    resrc **add = inst_upd(add_cnt, 'A', ex4);
    resrc **mul = inst_upd(mul_cnt, 'M', ex4);

    //printf("Add instances %d %d \n", add[0]->instance, add[1]->instance);
    //printf("Mul instances %d %d %d\n", mul[0]->instance, mul[1]->instance, mul[2]->instance);

    //printf("Add usage %d %d \n", add[0]->use, add[1]->use);
    mux **add_muxes = get_mux_for_resrc(add, list);
    mux **mul_muxes = get_mux_for_resrc(mul, list);

    
    /*
    for(int i = 0; add_muxes[i] != NULL; i++) {
        printf("Mux %d\n", i);
        printf("inst %d\n", add_muxes[i]->output->use);
        for(int j = 0; add_muxes[i]->inputs[j] != NULL; j++) {
            printf("Reg %d ", add_muxes[i]->inputs[j]->reg_number);
            
        }
        printf("\n");
    }
    */
    

    gen_vhdl(ex4, add_muxes, mul_muxes, add, mul, mem_count, list);

}