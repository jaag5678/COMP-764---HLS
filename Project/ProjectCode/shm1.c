#include"codegen.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int main() {

    /* 
        Example code - Message passing
        xs=1;ys=1;
        ||
        a=ys1; b=xs1;

    */

   //Set all memory in a separate prog 

    memalloc *mem_list[4];

    mem_list[0] = set_memalloc("a", 1);
    mem_list[1] = set_memalloc("b", 1);
    mem_list[2] = set_memalloc("xs", 1);
    mem_list[3] = set_memalloc("ys", 1);

    stmt *stmt_list[4];

    for(int i = 0; i < 4; i++)
        stmt_list[i] = set_stmt(mem_list[i], 0);

    stmt *mem = set_block(stmt_list, 4);
    
    


    //We have two threads, eachh with one 2 statements 

    stmt *stmt_list1[2], *stmt_list2[2];

    //-------------------Thread 1-------------------------

    //Two writes
    write *w1[2];

    //Two expressions 
    expr *e1[2];

    //Two leaves
    expr_leaf *leaves1[2];

    int j = 1;
    leaves1[0] = set_exp_leaf(&j, 1);
    leaves1[1] = set_exp_leaf(&j, 1);

    e1[0] = set_expr('c', NULL, NULL, leaves1[0]);
    e1[1] = set_expr('c', NULL, NULL, leaves1[1]);
    
    w1[0] = set_write("xs", 0, e1[0]);
    w1[1] = set_write("ys", 0, e1[1]);

    stmt_list1[0] = set_stmt(w1[0], 1);
    stmt_list1[1] = set_stmt(w1[1], 1);

    stmt *mp_t1 = set_block(stmt_list1, 2);

    basic_block *t1 = blck_create_from_ast(mp_t1);


    //------------Thread 2-------------------------------

    //Two reads and two writes
    write *w2[2];
    read *r2[2];

    //Two expressions 
    expr *e2[2];

    //Two leaves
    expr_leaf *leaves2[2];

    r2[0] = set_read("ys", 0);
    r2[1] = set_read("xs", 0);

    leaves2[0] = set_exp_leaf(r2[0], 0);
    leaves2[1] = set_exp_leaf(r2[1], 0);

    e2[0] = set_expr('v', NULL, NULL, leaves2[0]);
    e2[1] = set_expr('v', NULL, NULL, leaves2[1]);

    w2[0] = set_write("a", 0, e2[0]);
    w2[1] = set_write("b", 0, e2[1]);

    

    stmt_list2[0] = set_stmt(w2[0], 1);
    stmt_list2[1] = set_stmt(w2[1], 1);

    
    stmt *mp_t2 = set_block(stmt_list2, 2);

    
    basic_block *t2 = blck_create_from_ast(mp_t2);


    //-------------Basic Blocks ready---------------------
    
    char **m1 = get_mem_blocks_from_ast(mem);
    char **m2 = m1;

    
    //printf("T1 loc %d T2 loc %d \n", t1->loc, t2->loc);
    
    //ord_mem_list(t1);
    
    set_sc_order(ord_mem_list(t1));
    set_sc_order(ord_mem_list(t2));
    
    int clk1 = schedule_cdfg(t1, m1, 0);
    int clk2 = schedule_cdfg(t2, m2, 0);

    printf("Total Clock cycles for MP = %d %d\n", clk1 + 1 , clk2 + 1);

    return 0;





}