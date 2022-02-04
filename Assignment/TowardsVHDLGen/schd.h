/*
We need to assign for each data flow graph, the nodes to appropriate clock cycles.
Each node in the data-flow-graph will either use
    1) RAM - Load / Store (L/S)
    2) Adder - Plus  (A)
    3) Multiplier - Into (M) 
Note that constants do not require anything, hence do not bother assigning them to any clock cycle. 

For each resource above, we need to create a ready list, 
Each list specifying which nodes of dfg are ready to be scheduled with no pending dependencies 

*/

#include "cdfg.h"

void init_schd(gnode *);

int node_count_type(gnode *, char);

gnode** create_ready_list(gnode **, gnode *, int *, char, char*);

void print_ready_list(gnode **);

int set_schedule(gnode **, char, int);

void print_schedule(gnode *);

int schedule_cdfg(basic_block *, char **, int);

//get resource max count 
int get_max_res_dfg(gnode *, char, int);

//Get the max instances of a resource required for the program
int get_max_res_blck(basic_block *, char);




