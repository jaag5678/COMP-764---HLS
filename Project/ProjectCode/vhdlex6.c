#include"codegen.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


int main () {

    /* Example code 

        a = malloc(1);
        b = malloc(1);
        c = malloc(1);
        a = b + c;
        b = c + a;
        c = a + b;

        Testing correct scheduling during data dependency
    */

    srand(time(NULL));


    //We have 6 statements 
    stmt *stmt_list[5];

    //3 of memalloc
    memalloc *mem_list[3];

    //3 writes
    write *w[3];

    //And 6 reads 
    read *r[6];

    //9 expreessions 
    expr *e[9];

    //6 leaves 
    expr_leaf *leaves[6];

    //First set memalloc
    mem_list[0] = set_memalloc("a", 1);
    mem_list[1] = set_memalloc("bs", 1);
    mem_list[2] = set_memalloc("cs", 1);

    for(int i = 0; i < 3; i++)
        stmt_list[i] = set_stmt(mem_list[i], 0);
    
    //Set the reads
    r[0] = set_read("bs", 0);
    r[1] = set_read("cs", 0);
    r[2] = set_read("cs", 0);
    r[3] = set_read("a", 0);
    r[4] = set_read("a", 0);
    r[5] = set_read("bs", 0);

    //SEt leaves and expressions
    for(int i = 0; i < 6; i++) {
        leaves[i] = set_exp_leaf(r[i], 0);
        e[i] = set_expr('v', NULL, NULL, leaves[i]);
    }

    //Now the main expressions 
    for(int i = 0; i < 3; i++) {
        e[i+6] = set_expr('+', e[2*i], e[2*i + 1], NULL);
    }

    //Now set the writes
    w[0] = set_write("a", 0, e[6]);
    w[1] = set_write("bs", 0, e[7]);
    w[2] = set_write("cs", 0, e[8]);

    for(int i = 0; i < 3; i++) {
        stmt_list[i+3] = set_stmt(w[i], 1);
    }

    stmt *ast_ex6 = set_block(stmt_list, 6);

    //pretty_print_prog(ast_ex6);

    basic_block *ex6 = blck_create_from_ast(ast_ex6);
    char **mem_count = get_mem_blocks_from_ast(ast_ex6);

    //int clk = schedule_cdfg(ex6, mem_count, 0);
    gnode **listu_memu = ord_mem_list(ex6);

    set_sc_order(listu_memu);

    print_mem_dep(listu_memu);


    int clk = schedule_cdfg(ex6, mem_count, 0);

    print_schedule(ex6->dfgs[0]);
    print_schedule(ex6->dfgs[1]);
    print_schedule(ex6->dfgs[2]);

    int reg_cnt = get_max_reg_cnt(ex6);

    reg **list = reg_alloc(list, reg_cnt, ex6);

    //cse(list);
    
    /*
    for(int i = 0; i < ex6->loc; i++) {
        init_schd(ex6->dfgs[i]);
    }

    schedule_cdfg(ex6, mem_count, 0);
    */
    /*
    print_schedule(ex4->dfgs[0]);
    print_schedule(ex4->dfgs[1]);
    print_schedule(ex4->dfgs[2]);
    print_schedule(ex4->dfgs[3]);
    */
    int add_cnt = get_max_res_blck(ex6, 'A');
    int mul_cnt = get_max_res_blck(ex6, 'M');

    //pretty_print_blck(ex4);

    printf("A %d M %d\n", add_cnt, mul_cnt);
    
    resrc **add = inst_upd(add_cnt, 'A', ex6);
    resrc **mul = inst_upd(mul_cnt, 'M', ex6);

   
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
    

    //gen_vhdl(ex6, add_muxes, mul_muxes, add, mul, mem_count, list);



}