//Representing vhdl programs using AST data structure

#include "codegen.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
    
    /* Example 1
        mem = malloc(4 * sizeof(int));
        mem[3] = mem[0] * mem[1] + mem[2];
    */

    //The above program in ast format 

    //We know there are two ast nodes
    ast_node *a1, *a2;

    //We identify that there are two statements
    stmt *s1, *s2;

    //First statement is a memory alloaction 
    memalloc *m;

    //Second is a write
    write *w0; 

    //Write value is an expression who's composition has a tree
    expr *e0, *e1, *v0, *v1, *v2;

    //Expression has three leaves
    expr_leaf *l0, *l1, *l2;
    
    //Each leaf is a read 
    read *r0, *r1, *r2;
    
    m = set_memalloc("mem", 4);
    s1 = set_stmt(m, 0);
    
    r0 = set_read("mem", 0);
    r1 = set_read("mem", 1);
    r2 = set_read("mem", 2);
    
    l0 = set_exp_leaf(r0, 0);
    l1 = set_exp_leaf(r1, 0);
    l2 = set_exp_leaf(r2, 0);
    
    v0 = set_expr('v', NULL, NULL, l0);
    v1 = set_expr('v', NULL, NULL, l1);
    v2 = set_expr('v', NULL, NULL, l2);

    e1 = set_expr('*', v0, v1, NULL);
    e0 = set_expr('+', e1, v2, NULL);
    
    //Now finally set the write rhs
    w0 = set_write("mem", 3, e0);

    s2 = set_stmt(w0, 1);

    //pretty_print_ast(a1);
    //pretty_print_ast(a2);

    stmt *stmt_list_ex1[2];
    stmt_list_ex1[0] = s1;
    stmt_list_ex1[1] = s2;

    stmt *ast = set_block(stmt_list_ex1, 2);


    //pretty_print_prog(ast);

    char **sample_mem = get_mem_blocks_from_ast(ast);

    basic_block **cdfg;
    cdfg = malloc(sizeof(basic_block*));

    gnode *g1 = malloc(sizeof(gnode));
    gnode *g2 = malloc(sizeof(gnode));
    //g1 = ast_to_dfg(g1, a1);
    //g2 = ast_to_dfg(g2, a2);

    ast_node *ast_ex1[2];
    ast_ex1[0] = a1;
    ast_ex1[1] = a2;

    basic_block *b = blck_create_from_ast(ast);

    int max_add = node_count_type(b->dfgs[0], 'A');
    int max_mult = node_count_type(b->dfgs[0], 'M');
    int max_mem = node_count_type(b->dfgs[0], 'S') + node_count_type(b->dfgs[0], 'L');
    
    //printf("Number of nodes in graph =Add %d Mult %d Mem %d\n", max_add, max_mult, max_mem);

    init_schd(b->dfgs[0]);

    //printf("Clk cycles req %d \n", clk_cycle - 1);
    
    schedule_cdfg(b, get_mem_blocks_from_ast(ast), 0);

    int add_cnt_1 = get_max_res_blck(b, 'A');
    int mul_cnt_1 = get_max_res_blck(b, 'M');

    char **mem_cnt = get_mem_blocks_from_ast(ast);

    resrc **add1 = inst_upd(add_cnt_1, 'A', b);
    resrc **mul1 = inst_upd(mul_cnt_1, 'M', b);

    //print_schedule(b->dfgs[0]);

    int max_reg_ex1 = get_max_reg_cnt(b);
    reg **list_ex1 = reg_alloc(list_ex1, max_reg_ex1, b);

    mux **add_mux_ex1 = get_mux_for_resrc(add1, list_ex1);
    mux **mul_mux_ex1 = get_mux_for_resrc(mul1, list_ex1);
    
    /*
    for(int i = 0; mul_mux_ex1[i] != NULL; i++) {
        printf("Mux %d\n", i);
        for(int j = 0; mul_mux_ex1[i]->inputs[j] != NULL; j++) {
            printf("Reg %d ", mul_mux_ex1[i]->inputs[j]->reg_number);
        }
        printf("\n");
    }

    for(int i = 0; add_mux_ex1[i] != NULL; i++) {
        printf("Mux %d\n", i);
        for(int j = 0; add_mux_ex1[i]->inputs[j] != NULL; j++) {
            printf("Reg %d ", add_mux_ex1[i]->inputs[j]->reg_number);
        }
        printf("\n");
    }
    */

   gen_vhdl(b, add_mux_ex1, mul_mux_ex1, add1, mul1, mem_cnt, list_ex1);



//---------------------------------------------------------------------------------------------


}