//Helper functions to convert datapath to vhdl

#include "optimization.h"

void init_components();

void init_mem_signal(char **);

void init_reg_signal(reg **);

void init_mux_sel_signal(mux **);

void init_resrc_output_signal(resrc **);

void init_resrc_input_signal(mux **);

void init_ram_signals(char **);

void port_map_ram(char **, reg **);

void port_map_mux(mux *, char *, char *);

void map_resrc_output(resrc *, reg **);

void port_map_resrc(resrc **, mux **, reg**);


gnode ** get_list_from_sch(gnode **, int *, gnode *, int);

void state_machine_simulate(basic_block *, mux **, mux **, resrc **, resrc **, char **);

void gen_vhdl(basic_block *, mux **, mux **, resrc **, resrc **, char **, reg **);