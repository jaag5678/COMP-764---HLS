//Data structures for control flow graph whose elements are data flow graphs

//Firstly, note that we need to capture the syntactic order of operations. 
//Because we do not have conditionals and loops, a linked list would suffice to capture our control flow graph. 

//Secondly, each ast_node must be converted to a data-flow graph. 
//The difference?, we are only concered with the information to distinguish between index
//Assumption is also that the whole RAM is represented by one name, so we do not need that. 
//Also, we may not need to indices, think about where one might need this meta inforamation? Perhaps in register allocation? 

//So as a minimal construction, we can just remove need for loads to contain from which index as well as store to which index. 
//This would make our graph quite simple 

#include "ast.h"



typedef struct gnode {
    char type; //For now it is either 'A', 'M', 'S', 'L', 'C' 
    char mem[4]; // THis will refer to a particular memory, if the node represents a memory access
    int index; //THe exact memory location taken
    struct gnode **neighbors; //This will be fixed to two for the time being of our implementation
    int schd; // which clock cylce this node should be scheduled
}gnode;

typedef struct basic_block {
    int loc; //lines of code
    gnode **dfgs; //Allocate the size = loc. 
    int min_cycles; // How many clock cycles will it take to execute
}basic_block;

gnode* expr_to_gnode(gnode *, expr*);
gnode* ast_to_dfg(gnode*, stmt*); 

//char ** get_mem_blocks(ast_node **, int);

char** get_mem_blocks_from_ast(stmt *);

//basic_block* blck_creat(ast_node **, int);

basic_block* blck_create_from_ast(stmt *);

void pretty_print_gnode(gnode *);
void pretty_print_blck(basic_block *);