#include "dpath.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int get_max_reg_cnt_dfg(gnode *g) {

    switch (g->type)
    {
    case 'S':
        return 1 + get_max_reg_cnt_dfg(g->neighbors[0]);
        break;
    case 'A': case 'M':
        return 2 + get_max_reg_cnt_dfg(g->neighbors[0]) + get_max_reg_cnt_dfg(g->neighbors[1]);
        break;
    default:
        return 0;
        break;
    }
}

int get_max_reg_cnt(basic_block *b) {

    int total = 0;
    for(int i = 0; i < b->loc; i++) {
        total = total + get_max_reg_cnt_dfg(b->dfgs[i]);
    }
    return total;
}

char* get_reg_name(reg *r) {

    char *name = malloc(sizeof(char) * 10);

    sprintf(name, "r%d", r->reg_number);    

    return name;
}

reg* get_reg_from_node(gnode *g, reg **reg_list) {

    for(int i = 0; reg_list[i] != NULL; i++) {
        if(reg_list[i]->from == g)
            return reg_list[i];
    }

    return NULL;

}


int reg_alloc_dfg(reg **reg_list, int index, gnode *g, int cycle) {

    switch (g->type)
    {

    case 'S':
        if(g->schd == cycle) {
            reg_list[index] -> to = g;
            reg_list[index] -> from = g->neighbors[0];
            reg_list[index] -> reg_number = index;
            index ++;
        }
        index = reg_alloc_dfg(reg_list, index, g->neighbors[0], cycle);
        break;

    case 'A': case 'M':
        if(g->schd == cycle) {
           //Two inputs to them and one output 
            reg_list[index] -> to = g;
            reg_list[index] -> from = g->neighbors[0];
            reg_list[index] -> reg_number = index;

            index ++;
            reg_list[index] -> to = g;
            reg_list[index] -> from = g->neighbors[1];
            reg_list[index] -> reg_number = index;

            index ++;
            //printf("Type for reg alloc%c\n", g->type);
        }
        index = reg_alloc_dfg(reg_list, index, g->neighbors[0], cycle);
        index = reg_alloc_dfg(reg_list, index, g->neighbors[1], cycle);
            
        return index;
        break;
    
    default:
        return index;
        break;
    }

}

reg ** reg_alloc(reg** reg_list, int size, basic_block *b) {

    //Allocate register list based on size
    reg_list = malloc(sizeof(reg *) * size + 1);
    for(int i = 0; i < size; i++) 
            reg_list[i] = malloc(sizeof(reg));

    int reg_index = 0;
    //Now for each clock cycle, allocate registers
    for(int i = 1; i < b->min_cycles + 1; i++) {
        for(int j = 0; j < b->loc; j++) {
            //Allocate register nodes 
            reg_index = reg_alloc_dfg(reg_list, reg_index, b->dfgs[j], i); 
 
        }        
    }

    reg_list[size] = NULL;
    return reg_list;

}

int inst_upd_dfg(resrc **res, int inst, char type, gnode *g, int cycle) {

    
    switch (g->type)
    {
    case 'A': case 'M':
        if(g->schd == cycle) {
            if(type == g->type) {
                res[inst]->instance = inst;
                res[inst]->i1[res[inst]->use] = g->neighbors[0];
                res[inst]->i2[res[inst]->use] = g->neighbors[1];
                res[inst]->o[res[inst]->use] = g;
                res[inst]->use++;
                //printf("Here in Add\n");
                inst++;
            }
            //inst_upd_dfg(res, inst + 1, type, g->neighbors[0], cycle);
            //inst_upd_dfg(res, inst + 1, type, g->neighbors[1], cycle);
        }
        inst = inst_upd_dfg(res, inst, type, g->neighbors[0], cycle);
        inst = inst_upd_dfg(res, inst, type, g->neighbors[1], cycle);
        break;
    
    break;

    case 'S':     
        inst = inst_upd_dfg(res, inst, type, g->neighbors[0], cycle);
        break;
    default:
        break;
    }

    return inst;
       
}

resrc ** inst_upd(int inst, char type, basic_block *b) {

    if(inst == 0)
        return NULL;

    int max_use = 0; 
    for(int i = 0; i < b->loc; i++) 
        max_use += node_count_type(b->dfgs[i], type);
    
    //printf("Max Use %d\n", max_use);
    resrc **res = malloc(sizeof(resrc *) * (inst + 1));
    for(int i = 0; i < inst; i ++) {
        res[i] = malloc(sizeof(resrc));
        res[i]->use = 0;
        res[i]->instance = i;
        res[i]->i1 = malloc(sizeof(gnode *) * max_use + 1);
        res[i]->i2 = malloc(sizeof(gnode *) * max_use + 1);
        res[i]->o = malloc(sizeof(gnode *) * max_use + 1);
        res[i]->i1[max_use] = NULL;
        res[i]->i2[max_use] = NULL;
        res[i]->o[max_use] = NULL;
    }
    
   int tmp_inst; 
    
    for(int i = 0; i < b->min_cycles; i++) {
        tmp_inst = 0;
        for(int j = 0; j < b->loc; j++) {
            tmp_inst = inst_upd_dfg(res, tmp_inst, type, b->dfgs[j], i);
        }

    }
    //printf("Resrc %c\n", res[0]->o[0]->type);
    //printf("Type %c Instance %d Usage %d\n", type, res[0]->instance, res[0]->use);

    //printf("instance %d\n", inst);
    res[inst] = NULL;
    return res;

}

mux** get_mux_for_resrc(resrc **res_list, reg **reg_list) {

    if(res_list == NULL)
        return NULL;

    int mux_cnt = 0;

    for(int i = 0; res_list[i] != NULL; i++) {
        mux_cnt++;
    }

    mux **resrc_mux = malloc(sizeof(mux*) * 2 * mux_cnt);
    

    for(int i = 0; i < mux_cnt * 2; i++) 
        resrc_mux[i] = malloc(sizeof(mux));

    for(int i = 0; res_list[i] != NULL; i++) {

        //Each instance has use number of registers as input
        resrc_mux[2*i]->inputs = malloc(sizeof(reg*) * res_list[i]->use + 1);
        resrc_mux[2*i]->output = malloc(sizeof(resrc));
        resrc_mux[2*i]->output = res_list[i];
        resrc_mux[2*i]->sel = -1;

        resrc_mux[2*i+1]->inputs = malloc(sizeof(reg*) * res_list[i]->use + 1);
        resrc_mux[2*i+1]->output = malloc(sizeof(resrc));
        resrc_mux[2*i+1]->output = res_list[i];
        resrc_mux[2*i+1]->sel = -1;

        resrc_mux[2*i]->inputs[res_list[i]->use] = NULL;
        resrc_mux[2*i+1]->inputs[res_list[i]->use] = NULL;
        
        //printf("Inside here %d\n", res_list[i]->use);
        
        for(int j = 0; j < res_list[i]->use; j++) {

            //Each instance resrc use means one more register mapped to mux
            resrc_mux[2*i]->inputs[j] = malloc(sizeof(reg));
            resrc_mux[2*i+1]->inputs[j] = malloc(sizeof(reg));
            
            //Find the register that represents the edge i1[j] is in
            for(int k = 0; reg_list[k] != NULL; k++) {
               
                if(reg_list[k]->from == res_list[i]->i1[j]) {
                    //printf("Reg number %d\n", reg_list[k]->reg_number);
                    resrc_mux[2*i]->inputs[j] = reg_list[k];
                    continue;
                }
                else if(reg_list[k]->from == res_list[i]->i2[j]) {
                    //printf("Reg number %d\n", reg_list[k]->reg_number);
                    resrc_mux[2*i+1]->inputs[j] = reg_list[k];
                    //printf("Reg number %d\n", resrc_mux[i+1]->inputs[j]->reg_number);
                    continue;
                }
            }
           

        }
    }
    return resrc_mux;

}