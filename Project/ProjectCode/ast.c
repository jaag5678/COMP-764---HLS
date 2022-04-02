#include "ast.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<time.h>

//Implementation of functions defined in ast.h
read* set_read(char *m, int i) {
    read *r = malloc(sizeof(read));
    strcpy(r->memory, m);
    r->index = i;
    //Setting memory order for shared memory concurrency
    /*
        Make sure to keep this random, so that we can get many benchmarks
    */
    if(strchr(m, (int)('s')) != NULL)
        r->type =  0;
    else 
        r->type = 1;
    printf("Mem_type %d \n", r->type);

    return r;
}

expr_leaf* set_exp_leaf(void *rc, int ch) {

    expr_leaf *el = malloc(sizeof(expr_leaf));
    switch (ch)
    {
    case 0:
        el->ld = (read *)rc;   
        break;
    
    default:
        el->cnst = *(int*)rc;
        break;
    }
    return el;
}

expr* set_expr(char o, expr *el, expr *er, expr_leaf *l) {
    expr *ej = malloc(sizeof(expr));
    ej -> op = o;
    ej -> left = el;
    ej -> right = er;
    ej -> leaf = l;
    return ej;
}

write* set_write(char *mem, int i, expr *e) {
    write *w = malloc(sizeof(write));
    strcpy(w -> memory, mem);
    w -> index = i;
    w -> val = e;
    //Setting memory order for shared memory concurrency
    if(strchr(mem, (int)'s') != NULL)
        w->type = 0;
    else 
        w->type = 1;
    return w;
}

memalloc* set_memalloc(char *name, int size) {
    memalloc *m = malloc(sizeof(memalloc));
    strcpy(m->memory, name);
    m->size = size;
    return m;
}

stmt* set_stmt(void *type, int ch) {

    stmt *node = malloc(sizeof(stmt));
    switch (ch)
    {
    case 0:
        node ->alloc = (memalloc *)type;
        break;
    
    default:
        node -> st = (write *)type;
        break;
    }
    return node;
}

stmt* set_block(stmt **list, int size) {

    stmt* b = malloc(sizeof(stmt));
    b->alloc = NULL;
    b->st = NULL;
    b->block = malloc(sizeof(stmt*) * size+1);

    for(int i = 0; i < size; i++)
        b->block[i] = list[i];
    b->block[size] = NULL;

    return b;
}


void pretty_print_expr_leaf(expr_leaf *l, char op) {

    switch (op)
    {
    case 'v':
        printf(" %s[%d]", l->ld->memory, l->ld->index);
        break;
    
    default:
        printf(" %d", l->cnst);
        break;
    }
}


void pretty_print_expr(expr *e) {
    
    if(e == NULL)
        return;

    pretty_print_expr(e->left);
    if(e->op == '+' || e->op =='*')
        printf(" %c", e->op);
    else 
        pretty_print_expr_leaf(e->leaf, e->op);
        
    pretty_print_expr(e->right);
}

void pretty_print_prog(stmt *block) {

    if(block->alloc != NULL) {
        printf("%s", block->alloc->memory);
        printf(" = malloc(sizeof(int)*%d);\n", block->alloc->size);
        return;
    }
    if(block->st != NULL) {
        printf("%s[%d] = ",block->st->memory, block->st->index);
        pretty_print_expr(block->st->val);
        printf(";\n");
        return;
    }
    for(int i = 0; block->block[i] != NULL; i++)
        pretty_print_prog(block->block[i]);


}

void pretty_print_ast(ast_node *a) {

    switch (a->type)
    {
    case 0:
        printf("%s", a->cur->alloc->memory);
        printf(" = malloc(sizeof(int)*%d);\n", a->cur->alloc->size);
        break;
    
    default:
        printf("%s[%d] = ",a->cur->st->memory, a->cur->st->index);
        pretty_print_expr(a->cur->st->val);
        printf(";\n");
        break;
    }
}