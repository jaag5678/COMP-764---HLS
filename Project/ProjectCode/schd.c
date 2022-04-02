#include "schd.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>


void init_schd(gnode *g) {
    if(g == NULL)
        return;
    if(g->type != 'C')
        g->schd = -1;
    else 
        g->schd = 0;

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

/*
The ready list ensures the order in which we collect the actions to be done 
In particular we are interested in the order of memory operations.
While this definitely does not matter for sequential setting (as we will not get the elements ready to be scheduled until its ighbours), we have to take care of the case when we want to maintain certain memory dependencies in a concurrent context.
For instance, in SC semantics.
But on observing, I can see that the ready list is indeed created in-order when it comes to memory accesses/
This should suffice for our requirement, in that memory actions are ordered.
However, this would only give us coherence.
While we need SC semantics, we would need to create a unified list for all memory actions
In the most strict sense, this set would be an ordered list (total)
However, relaxations that explain for program transformations will account for the fact that they are indeed a partial order.
*/

gnode ** create_ready_list(gnode **list, gnode *g, int *ins_loc, char type, char *mem) {

    if(g->neighbors == NULL) {
        if(g->type == type && g->schd == -1 && !strcmp(g->mem, mem)) {
            //printf("%s %s %d\n", mem, g->mem, strcmp(g->mem, mem));

            //Here it only goes into ready list if all the memory dependencies are already scheduled
            if(g->mem_dep != NULL) {
                for(int i = 0; g->mem_dep[i] != NULL; i++) {
                    if(g->mem_dep[i]->schd == -1)
                        return list;
                }
            }
            //If we reach here, then even the memory dependencies are satisfied.
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
        
        /*
            Objective: Make this part sensitive to memory dependencies
                Make sure that the code does not break on other cases 
                Make sure that every example created so far is tested this way
                Just loop through the memory dependency array to make sure that nothing funny is happening 
        */

        //Check if all previous memory dependencies have already been scheduled
        //for(int i = 0; list[0]->mem_dep[i] != NULL; i++)
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

        //Here whether to call 'L' or 'S' would matter only when we are having multiple Writes and Reads to same RAM blcok in the code.
        //This could happen, and surely must be tested to ensure that WR/WW/RW dependencies are maintained.
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

//Strong ordering of all memory accesses.
//Memory accesses as though respecting sequentially consistent semantics
gnode** create_ready_list_mem_sc(gnode **list, gnode *g, int *ins_loc) {

    if(g->neighbors == NULL) {
        if(g->schd == -1) {
            list[*ins_loc] = g;
            (*ins_loc)++;
        }
        return list;
    }

    switch (g->type) {
        case 'S':
            if(g->neighbors[0]->schd != -1 && g->schd == -1) {
                list[*ins_loc] = g;
                (*ins_loc)++;
                return list; 
            }
            list = create_ready_list_mem_sc(list, g->neighbors[0], ins_loc);
            break;
        default: 
            list = create_ready_list_mem_sc(list, g->neighbors[0], ins_loc);
            list = create_ready_list_mem_sc(list, g->neighbors[1], ins_loc);
            break;
    }
    return list;

}

//Defining here new scheduling strategy that respects sequential consistency
int schedule_cdfg_sc(basic_block *b, int st_clk) {

    //First, get total nodes in the entire block
    int add_nodes = 0;
    int mul_nodes = 0;
    int mem_nodes = 0;


    for(int i = 0; i < b->loc; i ++) {
        add_nodes += node_count_type(b->dfgs[i], 'A');
        mul_nodes += node_count_type(b->dfgs[i], 'M');
        mem_nodes += (node_count_type(b->dfgs[i], 'S') + node_count_type(b->dfgs[i], 'L'));
    }

    //Note that mem_nodes here represents how many memory operations exist
    gnode **add_list = malloc(sizeof(gnode *) * add_nodes);
    gnode **mul_list = malloc(sizeof(gnode *) * mul_nodes);
    gnode **mem_list = malloc(sizeof(gnode *) * mem_nodes);

    int total_nodes = add_nodes + mul_nodes + mem_nodes;

    //Main loop for scheduling 
    while(total_nodes) {

        int x = 0;
        int y = 0;
        int z = 0;

        for(int i = 0; i < b->loc; i++) { 
            //printf("here %d \n", b->loc);
            create_ready_list(add_list, b->dfgs[i], &x, 'A', NULL);
            create_ready_list(mul_list, b->dfgs[i], &y, 'M', NULL);
            create_ready_list_mem_sc(mem_list, b->dfgs[i], &z);
            //At least one node has been scheduled in the line of code 
            //This means, either the write is scheduled, or an operation or a read
            //We need to ensure that memory accesses are ordered, so this condition ensures that 
            if(x != 0 || y != 0 || z != 0) {
                break;
            }
            printf("x %d y %d z %d \n", x, y, z);
        
        }

        int sch_nodes = 0;
        if(x)
            sch_nodes += set_schedule(add_list, 'A', st_clk);
        if(y)
            sch_nodes += set_schedule(mul_list, 'M', st_clk);
        if(z)
            sch_nodes += set_schedule(mem_list, 'L', st_clk);
        
        
        //if(x == 0 && y == 0 && z == 0)
        //    break;
        //Inc the clk_cycle by 1
        st_clk++;

        //Decrement the total nodes by that scheduled
        total_nodes -= sch_nodes;

    }

    b->min_cycles = st_clk - 1;

    return st_clk;

}


/*
    We need to create dependencies between memory operations 
    For this we can create an ordered list of memory operations
    This ordered list will be the strict program order 
    However, based on the memory order, we will add dependencies to memory operations 
    Because we have the list which already has an implicit total order, we will modify the gnode of the memory accesses to enforce orderings
    Additionally, we need to also ensure we order same mmeory operations to avoid coherence violations (RR violation that is)
*/

//First we write a function that gives us the total count of memory operations given a statement (ikr, C memory management issues)
int get_mem_cnt(gnode *g) {

    if(g->type == 'L')
        return 1;
    
    switch (g->type)
    {
    case 'S':
        return 1 + get_mem_cnt(g->neighbors[0]);
        break;
    default:
        return get_mem_cnt(g->neighbors[0]) + get_mem_cnt(g->neighbors[1]);
        break;
    }

}

gnode **ord_mem_list_stmt(gnode **list, gnode *g, int *ins_loc) {

    if(g->type == 'L') {
        if(strchr(g->mem, (int)'s') != NULL) {
            list[*ins_loc] = g;
            (*ins_loc)++;
        }
        return list;
    }

    switch (g->type)
    {
    case 'S':
        list = ord_mem_list_stmt(list, g->neighbors[0], ins_loc);
        if(strchr(g->mem, (int)'s') != NULL) {
            list[*ins_loc] = g;
            (*ins_loc)++;
        }
        break;
    
    default:
        list = ord_mem_list_stmt(list, g->neighbors[0], ins_loc);
        list = ord_mem_list_stmt(list, g->neighbors[1], ins_loc);
        break;
    }

    return list;

}

//First we write a function that returns an ordered list of memory events given a basic bloc
gnode** ord_mem_list(basic_block *b) {

    int total_mem_cnt = 0;

    for(int i = 0; i < b->loc; i++) {
        total_mem_cnt += get_mem_cnt(b->dfgs[i]);
    }

    printf("Total memory operations %d \n", total_mem_cnt);

    gnode **list = malloc(sizeof(gnode *) * total_mem_cnt + 1);

    int ins_loc = 0;
    for(int i = 0; i < b->loc; i++) {
        list = ord_mem_list_stmt(list, b->dfgs[i], &ins_loc);
    }

    list[ins_loc] = NULL;

    //Worst case dependency list allocate memory
    for(int i = 0; i < ins_loc; i++) {
        list[i]->mem_dep = malloc(sizeof(gnode *) * (i + 1));
        //Set it all to NULL for the time being
        for(int j = 0; j < i + 1; j++)
            list[i]->mem_dep[j] = NULL;
    }

    
    return list;
}

void set_sc_order(gnode **mem_list) {

    //Setting Sc order is simple, we just add the gnode which is in the previous location 
    for(int i = 0; mem_list[i] != NULL; i++) {
        for(int j = i+1; mem_list[j] != NULL; j++) {
            int k;
            for(k = 0; mem_list[j]->mem_dep[k] != NULL; k++);
            //Now k references to the index where we can insert another memory dependency
            mem_list[j]->mem_dep[k] = mem_list[i];
        }
    }

}

void print_mem_dep(gnode **mem_list) {

    for(int i = 0; mem_list[i] != NULL; i++) {
        printf("Mem %s ", mem_list[i]->mem);
        for(int j = 0; mem_list[i]->mem_dep[j] != NULL; j++)
            printf("-> %s", mem_list[i]->mem_dep[j]->mem);
        printf("\n");
    }
}

