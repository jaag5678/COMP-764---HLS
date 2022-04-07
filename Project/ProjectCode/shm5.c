#include"codegen.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int main() {

    /* 
        Example code 
        as = cs + ds; bs = es + fs;
        ||
        as = cs + ds; bs = es + fs;

    */

   //Set all memory in a separate prog 

    memalloc *mem_list[18];

    mem_list[0] = set_memalloc("a1", 1);
    mem_list[1] = set_memalloc("b1", 1);
    mem_list[2] = set_memalloc("c1", 1);
    mem_list[3] = set_memalloc("d1", 1);
    mem_list[4] = set_memalloc("e1", 1);
    mem_list[5] = set_memalloc("f1", 1);
    mem_list[6] = set_memalloc("a2", 1);
    mem_list[7] = set_memalloc("b2", 1);
    mem_list[8] = set_memalloc("c2", 1);
    mem_list[9] = set_memalloc("d2", 1);
    mem_list[10] = set_memalloc("e2", 1);
    mem_list[11] = set_memalloc("f2", 1);
    mem_list[12] = set_memalloc("as", 1);
    mem_list[13] = set_memalloc("bs", 1);
    mem_list[14] = set_memalloc("cs", 1);
    mem_list[15] = set_memalloc("ds", 1);
    mem_list[16] = set_memalloc("es", 1);
    mem_list[17] = set_memalloc("fs", 1);
    
    
    stmt *stmt_list[12];

    for(int i = 0; i < 18; i++)
        stmt_list[i] = set_stmt(mem_list[i], 0);

    stmt *mem = set_block(stmt_list, 18);
    
    


    //We have two threads, eachh with one 2 statements 
    /*
        a1 = c1 + d1; 
        b1 = e1 + f1;
    */
    stmt *stmt_list1[2], *stmt_list2[2];

    //-------------------Thread 1-------------------------

    //Two writes
    write *w1[2];

    //Four reads 
    read *r1[4];

    //Six expressions 
    expr *e1[6];

    //Four leaves
    expr_leaf *leaves1[4];


    r1[0] = set_read("cs", 0);
    r1[1] = set_read("ds", 0);
    r1[2] = set_read("es", 0);
    r1[3] = set_read("fs", 0);


    leaves1[0] = set_exp_leaf(r1[0], 0);
    leaves1[1] = set_exp_leaf(r1[1], 0);
    leaves1[2] = set_exp_leaf(r1[2], 0);
    leaves1[3] = set_exp_leaf(r1[3], 0);
    

    e1[0] = set_expr('v', NULL, NULL, leaves1[0]);
    e1[1] = set_expr('v', NULL, NULL, leaves1[1]);
    e1[2] = set_expr('v', NULL, NULL, leaves1[2]);
    e1[3] = set_expr('v', NULL, NULL, leaves1[3]);
    
    e1[4] = set_expr('+', e1[0], e1[1], NULL);
    e1[5] = set_expr('+', e1[2], e1[3], NULL);


    w1[0] = set_write("as", 0, e1[4]);
    w1[1] = set_write("bs", 0, e1[5]);

    stmt_list1[0] = set_stmt(w1[0], 1);
    stmt_list1[1] = set_stmt(w1[1], 1);

    stmt *ast_t1 = set_block(stmt_list1, 2);

    basic_block *t1 = blck_create_from_ast(ast_t1);


    //------------Thread 2-------------------------------
    
    //Two writes
    write *w2[2];

    //Four reads 
    read *r2[4];

    //Six expressions 
    expr *e2[6];

    //Four leaves
    expr_leaf *leaves2[4];


    r2[0] = set_read("cs", 0);
    r2[1] = set_read("ds", 0);
    r2[2] = set_read("es", 0);
    r2[3] = set_read("fs", 0);


    leaves2[0] = set_exp_leaf(r2[0], 0);
    leaves2[1] = set_exp_leaf(r2[1], 0);
    leaves2[2] = set_exp_leaf(r2[2], 0);
    leaves2[3] = set_exp_leaf(r2[3], 0);
    

    e2[0] = set_expr('v', NULL, NULL, leaves2[0]);
    e2[1] = set_expr('v', NULL, NULL, leaves2[1]);
    e2[2] = set_expr('v', NULL, NULL, leaves2[2]);
    e2[3] = set_expr('v', NULL, NULL, leaves2[3]);
    
    e2[4] = set_expr('+', e2[0], e2[1], NULL);
    e2[5] = set_expr('+', e2[2], e2[3], NULL);


    w2[0] = set_write("as", 0, e2[4]);
    w2[1] = set_write("bs", 0, e2[5]);

    stmt_list2[0] = set_stmt(w2[0], 1);
    stmt_list2[1] = set_stmt(w2[1], 1);

    stmt *ast_t2 = set_block(stmt_list2, 2);

    basic_block *t2 = blck_create_from_ast(ast_t2);


    //-------------Basic Blocks ready---------------------
    
    char **m1 = get_mem_blocks_from_ast(mem);
    char **m2 = m1;

    
    //printf("T1 loc %d T2 loc %d \n", t1->loc, t2->loc);
    
    //ord_mem_list(t1);
    
    set_sc_order(ord_mem_list(t1));
    set_sc_order(ord_mem_list(t2));
    
    int clk1 = schedule_cdfg(t1, m1, 0);
    int clk2 = schedule_cdfg(t2, m2, 0);

    printf("Total Clock cycles for MP = %d %d\n", clk1, clk2);

    int a1_cnt = get_max_res_blck(t1, 'A');
    int a2_cnt = get_max_res_blck(t2, 'A');

    printf("Total adders used = %d %d\n", a1_cnt, a2_cnt);

    //Merge the two dfgs 

     //Merge the two dfgs in different ways-------------------------------

    
    basic_block *merge[6]; 

    for(int i = 0; i < 6; i++) {
        merge[i] = malloc(sizeof(basic_block));


        merge[i]->loc = t1->loc + t2->loc;
        merge[i]->dfgs = malloc(sizeof(gnode*));
        merge[i]->min_cycles = 0;
        for(int j = 0; j < merge[i]->loc; j++) {

            merge[i]->dfgs[j] = malloc(sizeof(gnode));
        }
    }
    
    //Merge 1 t1-t1-t2-t2
    for(int i = 0; i < t1->loc; i++) 
        merge[0]->dfgs[i] = t1->dfgs[i];
    for(int i = t1->loc; i < merge[0]->loc; i++)
        merge[0]->dfgs[i] = t2->dfgs[i-t1->loc];

    //Merge 2 t1-t2-t1-t2
    merge[1]->dfgs[0] = t1->dfgs[0];
    merge[1]->dfgs[1] = t2->dfgs[0];
    merge[1]->dfgs[2] = t1->dfgs[1];
    merge[1]->dfgs[3] = t2->dfgs[1];

    //Merge 3 t1-t2-t2-t1
    merge[2]->dfgs[0] = t1->dfgs[0];
    merge[2]->dfgs[1] = t2->dfgs[0];
    merge[2]->dfgs[2] = t2->dfgs[1];
    merge[2]->dfgs[3] = t1->dfgs[1];

    //Merge 4 t2-t2-t1-t1
    merge[3]->dfgs[0] = t2->dfgs[0];
    merge[3]->dfgs[1] = t2->dfgs[1];
    merge[3]->dfgs[2] = t1->dfgs[0];
    merge[3]->dfgs[3] = t1->dfgs[1];

    //Merge 5 t2-t1-t2-t1
    merge[4]->dfgs[0] = t2->dfgs[0];
    merge[4]->dfgs[1] = t1->dfgs[0];
    merge[4]->dfgs[2] = t2->dfgs[1];
    merge[4]->dfgs[3] = t1->dfgs[1];

    //Merge 6 t2-t1-t1-t2
    merge[5]->dfgs[0] = t2->dfgs[0];
    merge[5]->dfgs[1] = t1->dfgs[0];
    merge[5]->dfgs[2] = t1->dfgs[1];
    merge[5]->dfgs[3] = t2->dfgs[1];


    //printf("Lines of code after merging %d \n", merge->loc);
    
    for(int j = 0; j < 6; j++) {

        for(int i = 0; i < merge[j]->loc; i++) {
            init_schd(merge[j]->dfgs[i]);
        }

        set_sc_order(ord_mem_list(merge[j]));
        int clk_merge = schedule_cdfg(merge[j], m1, 0);

        printf("Total Clock cycles after merging %d \n", clk_merge);

        int a_merge = get_max_res_blck(merge[j], 'A');

        printf("Total Adders utilized after merge %d \n", a_merge);

        /*
        print_schedule(merge->dfgs[0]);
        print_schedule(merge->dfgs[1]);
        print_schedule(merge->dfgs[2]);
        print_schedule(merge->dfgs[3]);
        */
    }
    

    return 0;





}