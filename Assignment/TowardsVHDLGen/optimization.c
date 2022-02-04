
#include "optimization.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


void cse(reg **reg_list) {


    //Take one register
    //Find similar registers out there 
    //If found, set their reg number to the same register
    //Also, set their to->neighbor to the register's from node

    for(int i = 0; reg_list[i] != NULL; i++) {

        if(reg_list[i]->reg_number != i)
            continue;

        reg *r = reg_list[i];

        for(int j = i + 1; reg_list[j] != NULL; j++) {

            reg *t = reg_list[j];
            //Compare from element between the two registers 
            
            if(t->from->type == r->from->type) {
                switch (t->from->type)
                {
                case 'L':
                    //Check if they are the same load 
                    if(!strcmp(r->from->mem, t->from->mem) && r->from->index == t->from->index) {

                        t->reg_number = r->reg_number;
                        if(t->to->neighbors[0] == t->from) {
                            t->to->neighbors[0] = r->from;
                        }
                        else {
                            t->to->neighbors[1] = r->from;
                        }
                        t->from = r->from;
                    }

                    break;
                
                case 'A': case 'M':
                    //Check if they have the same set of neighbors
                    //This would happen if previously we did register allocation to all the registers
                    //Because our reg_list is created in order of the statements, we would need just one iteration
                    if(r->from->neighbors[0] == t->from->neighbors[0] && r->from->neighbors[1] == t->from->neighbors[1]) {
                        t->reg_number = r->reg_number;
                        //printf("Hello there \n");
                        if(t->to->neighbors[0] == t->from) {
                            t->to->neighbors[0] = r->from;
                        }
                        else {
                            t->to->neighbors[1] = r->from;
                        }
                        t->from = r->from;
                    }
                break;
                
                default:
                    break;
                }
            }

        }

    }


}