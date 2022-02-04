#include "schd.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>


void init_schd(gnode *g) {
    if(g == NULL)
        return;
    g->schd = -1;

    if(g->neighbors == NULL)
        return;
    
    switch (g->type)
    {
    case 'S':
        init_schd(g->neighbors[0]);
        break;
    
    default:
        init_schd(g->neighbors[0]);
        init_schd(g->neighbors[1]);
        break;
    }

}

int node_count_type(gnode *g, char t) {

    if(g == NULL)
        return 0;
    if((g->neighbors == NULL)) {
        if(g->type == t)
            return 1;
        return 0;
    }
   
    switch (g->type)
    {
    case 'S':
        if(g->type == t)
            return 1 + node_count_type(g->neighbors[0], t);
        else 
            return node_count_type(g->neighbors[0], t);
        break;
    
    default:
        if(g->type == t)
            return 1 + node_count_type(g->neighbors[0], t) + node_count_type(g->neighbors[1], t); 
        else 
            return node_count_type(g->neighbors[0], t) + node_count_type(g->neighbors[1], t);     
        break;
    }

    return 1;

}

//Place NULL checks for mem
gnode ** create_ready_list(gnode **list, gnode *g, int *ins_loc, char type, char *mem) {

    if(g->neighbors == NULL) {
        if(g->type == type && g->schd == -1 && !strcmp(g->mem, mem)) {
            //printf("%s %s %d\n", mem, g->mem, strcmp(g->mem, mem));
            list[*ins_loc] = g;
            (*ins_loc)++;
        }
        //printf("here \n");
        return list;
    }

    switch (g->type)
    {
    case 'S':
        if(g->neighbors[0]->schd != -1 && g->schd == -1) {
            if(g->type == type  && !strcmp(g->mem, mem)) {
                //printf("%s %s\n", mem , g->mem);
                list[*ins_loc] = g;
                (*ins_loc)++;
            }
            return list;
        }
        list = create_ready_list(list, g->neighbors[0], ins_loc, type, mem);
        break;
    
    default:
        if(g->neighbors[0]->schd != -1 && g->neighbors[1]->schd != -1 && g->schd == -1)
            if(g->type == type) {
            list[*ins_loc] = g;
            (*ins_loc)++;
            return list;
        }
        list = create_ready_list(list, g->neighbors[0], ins_loc, type, mem);
        list = create_ready_list(list, g->neighbors[1], ins_loc, type, mem);
        break;
    }

    return list;

}

void print_ready_list(gnode **list) {


    for(int i = 0; list[i] != NULL; i++) {
        printf("%c\n", list[i]->type);
    }

}

int set_schedule(gnode **list, char type, int clk_cycle) {

    int i = 0;
    switch (type)
    {
    case 'S': case 'L':
        if(list[0] != NULL && list[0]->schd == -1) {
            list[0]->schd = clk_cycle;
            return 1;
        }
        break;
    
    default:
        for(;list[i] != NULL; i++) {
            if(list[i]->schd != -1)
                break;
            list[i]->schd = clk_cycle;
        }
        return i;
        break;
    }

    return 0;
}

void print_schedule(gnode *g) {

    if(g == NULL)
        return;

    printf("Type: %c Sch: %d \n", g->type, g->schd);

    if(g->neighbors == NULL)   
        return;

    switch (g->type)
    {
    case 'S':
        print_schedule(g->neighbors[0]);
        break;
    
    default:
        print_schedule(g->neighbors[0]);
        print_schedule(g->neighbors[1]);
        break;
    }
    
}

int schedule_cdfg(basic_block *b, char **mem_cnt, int st_clk) {
    
    //First, get total nodes in the entire block
    int add_nodes = 0;
    int mul_nodes = 0;
    int mem_nodes = 0;

    //get how many mem elements
    int mcnt = 0;
    while(mem_cnt[mcnt] != NULL)
        mcnt++;
    
    /*
    printf("MEM CNT %d \n", mcnt);
    for(int i = 0; i < mcnt; i++)
        printf("%s ", mem_cnt[i]);
    printf("\n");
    */
    for(int i = 0; i < b->loc; i ++) {
        add_nodes += node_count_type(b->dfgs[i], 'A');
        mul_nodes += node_count_type(b->dfgs[i], 'M');
        mem_nodes += (node_count_type(b->dfgs[i], 'S') + node_count_type(b->dfgs[i], 'L'));
    }

     //Initialize the ready list for each op
    gnode **add_list = malloc(sizeof(gnode *) * add_nodes);
    gnode **mul_list = malloc(sizeof(gnode *) * mul_nodes);
    gnode ***mem_list = malloc(sizeof(gnode **) * mcnt);

    for(int i = 0; i < mcnt; i++) 
        mem_list[i] = malloc(sizeof(gnode *) * mem_nodes);

    int total_nodes = add_nodes + mul_nodes + mem_nodes;
    //printf("total nodes %d \n", total_nodes);
    
    while(total_nodes) {

        int x, y, z[mcnt];
        x = 0;
        y = 0;
        for(int i = 0; i < mcnt; i++)
            z[i] = 0;
        
        for(int i = 0; i < b->loc; i++) { 
            //printf("here %d \n", b->loc);
            create_ready_list(add_list, b->dfgs[i], &x, 'A', NULL);
            create_ready_list(mul_list, b->dfgs[i], &y, 'M', NULL);

            for(int j = 0; j < mcnt; j++) {
                create_ready_list(mem_list[j], b->dfgs[i], &z[j], 'L', mem_cnt[j]);
                create_ready_list(mem_list[j], b->dfgs[i], &z[j], 'S', mem_cnt[j]);
            }
        }

        int sch_nodes = 0;
        sch_nodes += set_schedule(add_list, 'A', st_clk);
        sch_nodes += set_schedule(mul_list, 'M', st_clk);
        for(int j = 0; j < mcnt; j++) {
            sch_nodes += set_schedule(mem_list[j], 'L', st_clk); //L or S does not matter
        }
        //printf("Nodes schd %d\n", sch_nodes);
        
        if(total_nodes == total_nodes - sch_nodes)
            break;
        
        //Inc the clk_cycle by 1
        st_clk++;

        //Decrement the total nodes by that scheduled
        total_nodes -= sch_nodes;

        //printf("total nodes %d \n", total_nodes);
    }

    b->min_cycles = st_clk - 1;

    return st_clk;
}


int get_max_res_dfg(gnode *g, char r_type, int cycle) {

    int tmp_cnt = 0; 

    switch (g->type)
    {
    case 'A': case 'M':
        tmp_cnt = get_max_res_dfg(g->neighbors[0], r_type, cycle) + get_max_res_dfg(g->neighbors[1], r_type, cycle);
        if(g->schd == cycle) {
            if(g->type == r_type) {
                return 1 + tmp_cnt;
            }
        } 
        return tmp_cnt;
    
    case 'S':
        tmp_cnt = get_max_res_dfg(g->neighbors[0], r_type, cycle);
        return tmp_cnt;

    default:
        return tmp_cnt; //Will be 0
    }


}

int get_max_res_blck(basic_block *b, char r_type) {


    int res_cnt_max = 0;
    int tmp_cnt = 0;
    for(int i = 1; i < b->min_cycles + 1; i++) {
        
        for(int j = 0; j < b->loc; j++) 
            tmp_cnt += get_max_res_dfg(b->dfgs[j], r_type, i);
        if(tmp_cnt > res_cnt_max)
            res_cnt_max = tmp_cnt;
        tmp_cnt = 0;
    }

    return res_cnt_max;

}
