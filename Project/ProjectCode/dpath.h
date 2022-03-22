/*
    The datapath graph will have nodes that are
    1) Registers
    2) Add 
    3) Multiply
    4) Multiplexer

    To construct such a graph, we need to go through the dfg of each stmt in our basic block

    The tricky part will be to insert multiplexers in the graph where required.


*/ 

#include "schd.h"

typedef struct reg {

    int reg_number;
    gnode *from; 
    gnode *to;

}reg;


typedef struct resource {

    //Instance id
    int instance;
    // The number of times this instance is used.
    int use;  
    //Each input is a list of inputs that can be there.
    //The list index number would indicate which set of inputs and outputs are a pair.
    //Implicit assumption that each resource takes two inputs and gives one output ONLY
    gnode **i1;
    gnode **i2;
    gnode **o;

}resrc;

typedef struct mux {

    reg **inputs;
    resrc *output;
    //This will get changed by state, default is -1
    int sel;

}mux;

/*
typedef struct state {

    //Muxes to trigger select 
    mux **act;
    //RAM block to enable for writing
    char **mem_enable;

    //RAM block to disable writing
    //Here a read may be done hence.
    //By this stage the write would have been done
    char **mem_disable; 

}state;
*/


char* get_reg_name(reg *);

reg* get_reg_from_node(gnode*, reg**);

int get_max_reg_cnt_dfg(gnode *);
int get_max_reg_cnt(basic_block *);

int reg_alloc_dfg(reg **, int, gnode *, int);

reg ** reg_alloc(reg **, int, basic_block *);

int inst_upd_dfg(resrc **, int, char, gnode *, int);

resrc ** inst_upd(int, char, basic_block *);

mux** get_mux_for_resrc(resrc **, reg**);