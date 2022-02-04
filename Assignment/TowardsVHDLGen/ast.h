/*This header file will contain all the structures and substructures required to build an AST for a sample program

    The program consists of 
        1) Memory Allocation
        2) Load from Memory 
        3) Store to Memory 
        4) Expression -- composed of Multiplication(*) and Addition(+)
        5) Constants 

    Among each of the above program elements, 
        a) (1), (3) can be considered as statements.
        b) (4) a part of (3) -- Expressions are a part of store.
        c) (2) a part of (4) -- An expression contains loads.
        d) (5) a part of (4) -- An expression contains constants.
    
    Our AST then would be a sequence of statements. 

*/ 


/* Structure for a load
    1) memory -- the name of the RAM block from where it is read
    2) index -- the location in the RAM block from where data is read
*/
typedef struct read {
    char memory[4];
    int index; 
}read;

/*Union for leaves of expression
    1) ld -- the leaf can be a read from RAM block
    2) cnst -- the leaf can be just a constant
*/
typedef union expr_leaf {
    read *ld;
    int cnst;
}expr_leaf;

/* Now we can define expression structure as a binary tree
    1) op -- type of expression node 
        a) 'v' -- node is a leaf which is a read
        b) 'c' -- node is a leaf which is a constant
        c) '+' -- node represents addition
        d) '*' -- node represents multiplication
    2) left, right -- the two descendants of current node
    3) leaf -- expr represents a leaf (op = 'v' or 'c')
*/
typedef struct expr {
    char op; 
    struct expr *left; 
    struct expr *right; 
    expr_leaf *leaf;     
}expr;

/*Now we can define a write
    1) memory -- name of RAM block to write to
    2) index -- the location in the RAM block to write to
    3) val -- the data to be written (is an expr node) 
*/
typedef struct write{
    char memory[4];
    int index;
    expr *val;
}write;

/*Now to define a memory allocation 
    1) memory -- name of RAM block to instantiate
    2) size -- size of the RAM block to instantiate
*/ 
typedef struct memalloc{
    char memory[4];
    int size; 
}memalloc;

/*Now we can define a statement 
    It is  a union of 
    1) block -- represents a list of statements
    2) st -- a write operation 
    3) alloc -- a RAM block instantiation
*/ 
typedef struct stmt {
    struct stmt **block; 
    write *st;
    memalloc *alloc;
}stmt;


/* Now we can define our AST node 
    1) type -- type of statement
        a) 0 -- ast node represents a memalloc
        b) 1 -- ast node represents a write
        c) 2 -- ast node represents a block
    2) cur -- statement the ast node represents 
*/
typedef struct ast_node{
    int type; 
    stmt *cur;
}ast_node; 

//Helper functions to set up instances of above to desired values

/*To set a read node
    Input:
        1) the memory representing the RAM block
        2) the location in the RAM block to read from
    Output: 
        Read node
*/
read* set_read(char *, int);
expr_leaf* set_exp_leaf(void *, int);
expr* set_expr(char, expr *, expr *, expr_leaf *);
write* set_write(char *, int, expr *);
memalloc* set_memalloc(char *, int);
stmt* set_stmt(void *, int);
stmt* set_block(stmt**, int);
ast_node* set_ast_node(stmt *, int);

//Pretty printing 
void pretty_print_prog(stmt *);
void pretty_print_ast(ast_node *);
void pretty_print_expr(expr *);
void pretty_print_expr_leaf(expr_leaf *, char);