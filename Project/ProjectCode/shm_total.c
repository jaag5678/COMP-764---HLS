#include"codegen.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
//#include<unistd.h>

int main() {

      /* 
            Files to store the total clock cyles and adders required based on different merging strategies
            It will be a four column format.
            Col1: Pre-Merge Worst case clock cycle
            Col2: On Merge Clock Cycles
            Col3: Pre-merge Adders 
            Col4: On Merge Adder resource.
        */
        FILE *f[6];

        f[0] = fopen("statistics/merge0.text", "w+");
        f[1] = fopen("statistics/merge1.text", "w+");
        f[2] = fopen("statistics/merge2.text", "w+");
        f[3] = fopen("statistics/merge3.text", "w+");
        f[4] = fopen("statistics/merge4.text", "w+");
        f[5] = fopen("statistics/merge5.text", "w+");

        /* In addition we need a few more stats. 
            a) How many programs did we save clock cycles  
            b) How many programs did we save resources 

            For (a), we would need exactly how many clock cycles were saved, and for each saved, the difference counts.
            From the previous stats, we can guage the upper limit and store arrays for it accordingly

            FOr (b) too, we observe the previously collected stats and do the same
        */
                
        int clk_stat[5] = {0, 0, 0, 0, 0};
        int add_stat[3] = {0, 0, 0};

        /*
            More stats: We also record for each Merge strat, how many clock cycles and resources were saved
            For this, we would need a 2d array (for each merge) for clock cycles and 2d array for adders 
        */

        int merge_clk_stats[6][5];
        int merge_add_stats[6][3];

        for(int i = 0; i < 6; i++) {
            for(int j = 0; j < 5; j++)
                merge_clk_stats[i][j] = 0;
            for(int j = 0; j < 3; j++)
                merge_add_stats[i][j] = 0;
        }

        /*  We open the files for the statistics here itself
        */

        FILE *mc[6];
        FILE *ma[6];

        //For each merge strat, we record, how many programs benefited by merging 

        mc[0] = fopen("statistics/merge_clk_saved0.txt", "w+");
        mc[1] = fopen("statistics/merge_clk_saved1.txt", "w+");
        mc[2] = fopen("statistics/merge_clk_saved2.txt", "w+");
        mc[3] = fopen("statistics/merge_clk_saved3.txt", "w+");
        mc[4] = fopen("statistics/merge_clk_saved4.txt", "w+");
        mc[5] = fopen("statistics/merge_clk_saved5.txt", "w+");

        ma[0] = fopen("statistics/merge_add_saved0.txt", "w+");
        ma[1] = fopen("statistics/merge_add_saved1.txt", "w+");
        ma[2] = fopen("statistics/merge_add_saved2.txt", "w+");
        ma[3] = fopen("statistics/merge_add_saved3.txt", "w+");
        ma[4] = fopen("statistics/merge_add_saved4.txt", "w+");
        ma[5] = fopen("statistics/merge_add_saved5.txt", "w+");




    /* 
        Given set {a1, b1, c1, d1, e1, f1, a2, b2, c2, d2, e2, f2}
        We need all subsets and using this set, we decide whether a memory access is shared or not/ 

        SEt size = 12
        Total subsets = 2**12
    */

    //Define the set
    char *mem[18] = {"c1", "d1", "e1", "f1", "a1", "b1", "c2", "d2", "e2", "f2", "a2", "b2", "as", "bs", "cs", "ds", "es", "fs"};

    /*
    for(int i = 0; i < 12; i++)
        printf("%s ", mem[i]);
    printf("\n");
    */

    //printf("Program s-T1 s-T2 a-T1 a-T2 s-M1 a-M1 s-M2 a-M2 s-M3 a-M3 s-M4 a-M4 s-M5 a-M5 s-M6 a-M6\n");
    
    for(int i = 0; i < 4096; i++) {

       
        printf("%d ", i);
        int j = i;
        //printf("%d ", j);
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


        if(j % 2)
            r1[0] = set_read("cs", 0);
        else 
            r1[0] = set_read("c1", 0);
        //printf("%d",j%2);
        j /= 2;

        if(j % 2)
            r1[1] = set_read("ds", 0);
        else 
            r1[1] = set_read("d1", 0);
        //printf("%d",j%2);
        j /= 2;

        
        if(j % 2)
            r1[2] = set_read("es", 0);
        else 
            r1[2] = set_read("e1", 0);
        //printf("%d",j%2);
        j /= 2;
        
        if(j % 2)
            r1[3] = set_read("fs", 0);
        else 
            r1[3] = set_read("f1", 0);
        //printf("%d",j%2);
        j /= 2;
        

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


        if(j % 2)
            w1[0] = set_write("as", 0, e1[4]);
        else 
            w1[0] = set_write("a1", 0, e1[4]);
        //printf("%d",j%2);
        j /= 2;    
        
        if(j % 2)
            w1[1] = set_write("bs", 0, e1[5]);
        else 
            w1[1] = set_write("b1", 0, e1[5]);
        //printf("%d",j%2);
        j /= 2;    
        
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


        if(j % 2)
            r2[0] = set_read("cs", 0);
        else 
            r2[0] = set_read("c2", 0);
        //printf("%d",j%2);
        j /= 2;

        if(j % 2)
            r2[1] = set_read("ds", 0);
        else 
            r2[1] = set_read("d2", 0);
        //printf("%d",j%2);
        j /= 2;
        
        if(j % 2)
            r2[2] = set_read("es", 0);
        else 
            r2[2] = set_read("e2", 0);
        //printf("%d",j%2);
        j /= 2;
        
        if(j % 2)
            r2[3] = set_read("fs", 0);
        else 
            r2[3] = set_read("f2", 0);
        //printf("%d",j%2);
        j /= 2;


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


         if(j % 2)
            w2[0] = set_write("as", 0, e2[4]);
        else 
            w2[0] = set_write("a2", 0, e2[4]);
        //printf("%d",j%2);
        j /= 2;    
        
        if(j % 2)
            w2[1] = set_write("bs", 0, e2[5]);
        else 
            w2[1] = set_write("b2", 0, e2[5]);
        //printf("%d ",j%2);
        j /= 2;    

        stmt_list2[0] = set_stmt(w2[0], 1);
        stmt_list2[1] = set_stmt(w2[1], 1);

        stmt *ast_t2 = set_block(stmt_list2, 2);

        basic_block *t2 = blck_create_from_ast(ast_t2);


        //-------------Basic Blocks ready---------------------


      
        


        char **m1 = mem;
        char **m2 = m1;


        //printf("T1 loc %d T2 loc %d \n", t1->loc, t2->loc);

        //ord_mem_list(t1);

        set_sc_order(ord_mem_list(t1));
        set_sc_order(ord_mem_list(t2));

        int clk1 = schedule_cdfg(t1, m1, 0);
        int clk2 = schedule_cdfg(t2, m2, 0);

        //Total Clock cycles to be needed for each thread separately
        printf("%d %d ", clk1, clk2);
        //Worst case clock cyles used 
        printf("%d ", clk1 + clk2);

        int a1_cnt = get_max_res_blck(t1, 'A');
        int a2_cnt = get_max_res_blck(t2, 'A');

        //Total adders used for each thread separately
        //printf("%d %d ", a1_cnt, a2_cnt);
        printf("%d ", a1_cnt + a2_cnt);

        

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

            gnode **mem_list = ord_mem_list(merge[j]);
            set_sc_order(mem_list);
            int clk_merge = schedule_cdfg(merge[j], m1, 0);

            //Total Clock cycles after merging 
            printf("%d ", clk_merge);

            int a_merge = get_max_res_blck(merge[j], 'A');

            //Total Adders utilized after merge
            printf("%d ", a_merge);

            //We want to ensure that we are always improving in the total amount of clock cyles that would be required.
            //Note that this is also fixed given the scheduling, whereas pre-merge it was WORST-CASE.
            //In contrast, the number of adders (Hardware Accelerators) needed will always be fixed before and after merge
            //So any improvement would be a good thing after merge 
            //We hypothesize that there will be always such an improvement.
            fprintf(f[j], "%d ", i);
            fprintf(f[j], "%d %d ", clk1 + clk2, clk1 + clk2 - clk_merge);
            fprintf(f[j], "%d %d\n", a1_cnt + a2_cnt, a1_cnt + a2_cnt - a_merge);

            //Here we record the count for programs based on clk cycles / resources saved
            clk_stat[clk1+clk2 - clk_merge]++;
            add_stat[a1_cnt + a2_cnt - a_merge]++;

            //Here we record the counts of programs for each clock cycles 
            merge_clk_stats[j][clk1+clk2 - clk_merge]++;
            merge_add_stats[j][a1_cnt + a2_cnt - a_merge]++;
            
        }   

        printf("\n");

    }

    /* Here we write out the stats overall
        -> We record the cases where the worst case clock cycles have been improved
        -> We also record the cases where we save adder resource.
        -> This statistics is to show us that merging is indeed a valuable optimization for HLS as opposed to normal programs
    */ 
    FILE *fc_cnt, *fa_cnt;
    
    fc_cnt = fopen("statistics/clk_count.txt", "w+");
    fa_cnt = fopen("statistics/add_count.txt", "w+");
    
    for(int i = 0; i < 5; i++) 
        fprintf(fc_cnt, "%d %d\n", i, clk_stat[i]);
    
    for(int i = 0; i < 3; i++)
        fprintf(fa_cnt, "%d %d\n", i, add_stat[i]);
    
    
    /* Here we write out the stats per merge the benefits in terms of clock cycles and add resources 
    */

    for(int i = 0; i < 6; i++) {

        for(int j = 0; j < 5; j++)
            fprintf(mc[i], "%d %d\n", j, merge_clk_stats[i][j]);
        
        for(int j = 0; j < 3; j++)
            fprintf(ma[i], "%d %d\n", j, merge_add_stats[i][j]);

    }



    //Close all the file descriptors open till now
    
    fclose(ma[0]);
    fclose(ma[1]);
    fclose(ma[2]);
    fclose(ma[3]);
    fclose(ma[4]);
    fclose(ma[5]);

    fclose(mc[0]);
    fclose(mc[1]);
    fclose(mc[2]);
    fclose(mc[3]);
    fclose(mc[4]);
    fclose(mc[5]);

    fclose(f[0]);
    fclose(f[1]);
    fclose(f[2]);
    fclose(f[3]);
    fclose(f[4]);
    fclose(f[5]);

    fclose(fc_cnt);
    fclose(fa_cnt);

    return 0;
}