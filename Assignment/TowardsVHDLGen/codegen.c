#include "codegen.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void init_components() {

    //First a component for RAM block
    printf ("component int32dualportRAM is \n");
    printf("generic (addr_width: integer := 32);\n");
    printf("port (\n"); 
	printf("clk: IN std_logic; \n");
	printf("we: IN std_logic; \n");		
	printf("addr_w: IN std_logic_vector (addr_width - 1 downto 0); \n");		
	printf("addr_r: IN std_logic_vector (addr_width - 1 downto 0); \n");		
	printf("d_in: IN integer; \n ");		
	printf("d_out: OUT integer \n");			
	printf(");\n");
	printf("end component;\n\n");    


    //Component of adder 
    printf("component int32add is \n");
	printf("port (\n");	
	printf("a: IN integer;\n");		
	printf("b: IN integer;\n");		
	printf("sum: OUT integer\n");		
	printf(");\n");	
	printf("end component;\n\n");

    //Component of multiplier
    printf("component int32mult is \n");
	printf("port (\n");	
	printf("a: IN integer;\n");		
	printf("b: IN integer;\n");		
	printf("prod: OUT integer\n");		
	printf(");\n");	
	printf("end component;\n\n");

}

void init_mem_signal(char **mem_list) {
    printf("signal ");
    printf("sel_%s", mem_list[0]);
    for(int i = 1; mem_list[i] != NULL; i++) {
        printf(", sel_%s", mem_list[i]);
    }
    printf(": integer; \n");

    //Now the signals for other mem components
    printf("signal ");
    printf("we_%s", mem_list[0]);
    for(int i = 1; mem_list[i] != NULL; i++) {
        printf(", we_%s", mem_list[i]);
    }
    printf(": std_logic;\n");

    printf("signal ");
    printf("ar_%s, aw_%s", mem_list[0], mem_list[0]);
    for(int i = 1; mem_list[i] != NULL; i++) {
        printf(", ar_%s, aw_%s", mem_list[i], mem_list[i]);
    }
    printf(": std_logic_vector (31 downto 0);\n");

    printf("signal ");
    printf("din_%s, dout_%s", mem_list[0], mem_list[0]);
    for(int i = 1; mem_list[i] != NULL; i++) {
        printf(", din_%s, dout_%s", mem_list[i], mem_list[i]);
    }
    printf(": integer;\n");
}

void init_reg_signal(reg **reg_list) {

    printf("signal ");
    printf("%s", get_reg_name(reg_list[0]));
    for(int i = 1; reg_list[i] != NULL; i++) {
        if(reg_list[i]->reg_number != i)
            continue;
        printf(", %s", get_reg_name(reg_list[i]));

    }
    printf(": integer; \n\n");
}

void init_mux_sel_signal(mux **muxes) {

    if(muxes == NULL)
        return;
    printf("signal ");
    printf("sel_%c%d", muxes[0]->output->o[0]->type, muxes[0]->output->instance);
    
    //Since the signal for two muxes to same resource will be the same
    for(int i = 2; muxes[i] != NULL; i+=2) {
        printf(", sel_%c%d", muxes[i]->output->o[0]->type, muxes[i]->output->instance);
    }
    
    printf(": integer; \n\n");

}

void init_resrc_output_signal(resrc **res_list) {

    if(res_list == NULL)
        return;

    printf("signal ");
    printf("o%c%d", res_list[0]->o[0]->type, res_list[0]->instance);

    for(int i = 1; res_list[i] != NULL; i++) {
        printf(", o%c%d", res_list[i]->o[0]->type, res_list[i]->instance);
    }
    printf(": integer; \n\n");

}

void init_resrc_input_signal(mux **muxes) {

    if(muxes == NULL)
        return;

    printf("signal ");

    printf("i%c%d_0", muxes[0]->output->o[0]->type, muxes[0]->output->instance);

    //Note that each pair of consecutive muxes represent inputs of ONE instance of a resource. Hence we are able to do it this way
    for(int i = 1; muxes[i] != NULL; i++) {
        printf(", i%c%d_%d", muxes[0]->output->o[0]->type, muxes[i]->output->instance, i%2);
    }
    printf(": integer; \n\n");


}

void init_ram_signals(char **mems) {

    //Clock will be an input to our circuit

    //Each RAM will have we, ar, aw, din, dout
    for(int i = 0; mems[i] != NULL; i++) {
        printf("signal we_%s: std_logic ;\n", mems[i]);
        printf("signal aw_%s, ar_%s: std_logic_vector (31 downto 0); \n", mems[i], mems[i]);
        printf("signal din_%s, dout_%s: integer;\n", mems[i], mems[i]);
        printf("signal sel_%s: integer;\n\n", mems[i]);
        
    }

}

void port_map_ram(char **mems, reg **reg_list) {


    

    for(int i = 0; mems[i] != NULL; i++) {
        char *m = mems[i];
        int sel = 0;
        //Need to map output of ram to registers first.
        //We are assuming the registers corresponding to load outputs are issued from left to right in the reg_list for each RAM block.
        //This is because we allocate the registers in a depth first fashion. 
        for(int j = 0; reg_list[j] != NULL; j++) {
            
            if(!strcmp(reg_list[j]->from->mem, m) && reg_list[j]->reg_number == j) {
                printf("%s <= dout_%s when sel_%s <= %d;\n", get_reg_name(reg_list[j]), m, m, sel);
                sel++;
            }
        }
        printf("%s: int32dualportRAM port map(clk, we_%s, aw_%s, ar_%s, din_%s, dout_%s);\n\n", m, m, m, m, m, m);
    }

}

void port_map_mux(mux *m, char *sel, char *sig) {

    printf("%s <= ", sig);
    for(int i = 0; m->inputs[i] != NULL; i++) {
        printf("%s when %s <= %d ", get_reg_name(m->inputs[i]),sel, i);
        if(m->inputs[i+1] != NULL)
            printf("else \n");
    }
    printf(";\n");

}

void map_resrc_output(resrc *res, reg **list) {

    char t = res->o[0]->type;
    char inst = res->instance;
    for(int j = 0; res->o[j] != NULL; j++) {

        char *c = get_reg_name(get_reg_from_node(res->o[j], list));
        printf("%s <= o%c%d when sel_%c%d <= %d;\n", c, t, inst, t, inst, j);
        //printf("Here sir \n");
    } 


}

void port_map_resrc(resrc **res, mux **muxes, reg **reg_list) {


    if(res == NULL)
        return;

    char *logic = malloc(sizeof(char) * 4);
    if(res[0]->o[0]->type == 'A')
        strcpy(logic, "add");
    else 
        strcpy(logic, "mult");

    char *sel = malloc(sizeof(char) * 8);
    char *sig = malloc(sizeof(char) * 8);

    for(int i = 0; res[i] != NULL; i++) {

        sprintf(sel, "sel_%c%d", res[i]->o[0]->type, res[i]->instance);
        sprintf(sig, "i%c%d_%d", res[i]->o[0]->type, res[i]->instance, 0);
        //First port map mux both inputs
        port_map_mux(muxes[i*2], sel, sig);
        sprintf(sig, "i%c%d_%d", res[i]->o[0]->type, res[i]->instance, 1);
        port_map_mux(muxes[i*2 + 1], sel, sig);
        
        
        //Map regs to output
        map_resrc_output(res[i], reg_list);

        printf("\n");
        
        
    }

    //Port map resource now
    for(int i = 0; res[i] != NULL; i++) {
        
        printf("%c%d: int32%s port map", res[i]->o[0]->type, res[i]->instance, logic);

        printf("(i%c%d_0, i%c%d_1, o%c%d);\n\n", res[i]->o[0]->type, res[i]->instance, res[i]->o[0]->type, res[i]->instance, res[i]->o[0]->type, res[i]->instance);
    }

}

gnode ** get_list_from_sch(gnode **list, int *insert, gnode *g, int schd) {

    if(g == NULL)
        return list;

    
    if(g->schd == schd) {
            
            //We first want to check if the node has already been scheduled before.
            //This can happen after common subexpression elimination
            int t = 1;
            for(int i = 0; i < *insert; i++) {
                if(list[i] == g) {
                    t = 0;
                    break;
                }
            }
            if(t) {
                list[*insert] = g;
                *insert = *insert + 1;
            } 
    }

    switch (g->type)
    {
    case 'S': 
        get_list_from_sch(list, insert, g->neighbors[0], schd);
        break;
    
    case 'A': case 'M':    
        get_list_from_sch(list, insert, g->neighbors[0], schd);
        get_list_from_sch(list, insert, g->neighbors[1], schd);
        break;
    
    default:
        break;
    }

    return list;

}

void state_machine_simulate(basic_block *b, mux **add_mux, mux **mul_mux, resrc **add, resrc **mul, char **mems) {

    //Get the changes
    int add_cnt = 0;
    int mul_cnt = 0;
    int mem_cnt = 0;
    
    for(int i = 0; i < b->loc; i++) {
        add_cnt += node_count_type(b->dfgs[i], 'A');
        mul_cnt += node_count_type(b->dfgs[i], 'M');
        mem_cnt += (node_count_type(b->dfgs[i], 'L') + node_count_type(b->dfgs[i], 'S'));
    }

    int total_count = add_cnt + mul_cnt + mem_cnt;

    mem_cnt = 0;
    for(int i = 0; mems[i]!=NULL; i++)
        mem_cnt++;

    int mem_sel[mem_cnt];

    for(int i = 0; i < mem_cnt; i++)
        mem_sel[i] = 0;

    gnode **list = malloc(sizeof(gnode*) * total_count);
    int insert = 0;

     //Now loop cycle times
     for(int i = 0; i < b->min_cycles + 1; i++) {
         
        printf("if (state = %d) then \n", i);
        
        for(int j = 0; j < b->loc; j++) {
            
            get_list_from_sch(list, &insert, b->dfgs[j], i);

        }

        for(int k = 0; k < insert; k++) {

            switch (list[k]->type)
            {
            case 'S':
                //Set the correct RAM block write address
                printf("aw_%s <= std_logic_vector(to_unsigned(%d, 32));\n", list[k]->mem, list[k]->index);
                printf("we_%s <= '1';\n", list[k]->mem);
                break;
            
            case 'L':
                //Set the correct select signal of read block
                //if(list[k]->)
                printf("ar_%s <= std_logic_vector(to_unsigned(%d, 32));\n", list[k]->mem, list[k]->index);
                for(int l = 0; l < mem_cnt; l++) {
                    if(!strcmp(list[k]->mem, mems[l])) {
                        printf("sel_%s <= %d;\n", list[k]->mem, mem_sel[l]);
                        mem_sel[l]++;
                        break;
                    }
                }
                break;

            case 'A':
                
                for(int i1 = 0; add[i1] != NULL; i1++) {
                    
                    for(int i2 = 0; add[i1]->o[i2] != NULL; i2++) {

                        if(add[i1]->o[i2] == list[k]) {

                            //search for the right mUX signal
                            for(int i3 = 0; add_mux[i3] != NULL; i3++) {

                                if(add_mux[i3]->output == add[i1]) {

                                    //Update the select signal in mux
                                    add_mux[i3]->sel++;
                                    add_mux[i3+1]->sel++;
                                    printf("sel_%c%d <= %d;\n", list[k]->type, add[i1]->instance, add_mux[i3]->sel);
                                    break;
                                    
                                }

                            }
                            break;

                        }
                    }
                }
                break;

            case 'M':
                for(int i1 = 0; mul[i1] != NULL; i1++) {
                    
                    for(int i2 = 0; mul[i1]->o[i2] != NULL; i2++) {

                        if(mul[i1]->o[i2] == list[k]) {

                            //search for the right mUX signal
                            for(int i3 = 0; mul_mux[i3] != NULL; i3++) {

                                if(mul_mux[i3]->output == mul[i1]) {

                                    //Update the select signal in mux
                                    mul_mux[i3]->sel++;
                                    mul_mux[i3+1]->sel++;
                                    printf("sel_%c%d <= %d;\n", list[k]->type, mul[i1]->instance, mul_mux[i3]->sel);
                                    break;
                                    
                                }

                            }
                            break;

                        }
                    }
                }
                break;

            default:
                break;
            }


        }


        printf("state <= %d;\n", i+1);
        if(i != b->min_cycles)
            printf("\nels");

        insert = 0;
    
    }
    printf("else \n \nend if; \n"); 

}

void gen_vhdl(basic_block *b, mux **add_mux, mux **mul_mux, resrc **add, resrc **mul, char **mems, reg **reg_list) {

    printf("library IEEE;\n");
    printf("use IEEE.std_logic_1164.all;\n");
    printf("use IEEE.numeric_std.all;\n\n");

    printf("entity circuit is \n");
    printf("port ( \n");
    printf("clk: IN std_logic;\n");
    printf("q: OUT integer\n");
    printf(");\n");
    printf("end circuit;\n\n");

    printf("architecture hls of circuit is \n\n");

    init_components();
    init_mem_signal(mems);
    init_reg_signal(reg_list);
    init_mux_sel_signal(add_mux);
    init_mux_sel_signal(mul_mux);
    init_resrc_input_signal(add_mux);
    init_resrc_output_signal(add);
    init_resrc_input_signal(mul_mux);
    init_resrc_output_signal(mul);

    printf("signal state: integer := 0;\n");
    printf("\n\n");

    printf("begin\n");

    port_map_ram(mems, reg_list);
    port_map_resrc(add, add_mux, reg_list);
    port_map_resrc(mul, mul_mux, reg_list);

    printf("\n\n");
    printf("process(clk)\nbegin\n\n");

    printf("if (clk'event and clk='1') then \n");

    state_machine_simulate(b, add_mux, mul_mux, add, mul, mems);

    printf("\n\n");
    printf("end if;\n");

    printf("end process;\n\n");

    printf("q <= 1;\n\n");

    printf("end hls;\n");



}