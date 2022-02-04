#include "cdfg.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

gnode* expr_to_gnode(gnode *g, expr *e)  {

    switch (e->op)
    {
    case '+':
        g->type = 'A';
        g->neighbors = malloc(sizeof(gnode*) * 2);
        g->neighbors[0] = malloc(sizeof(gnode));
        g->neighbors[1] = malloc(sizeof(gnode));
        g->neighbors[0] = expr_to_gnode(g->neighbors[0],e->left);
        g->neighbors[1] = expr_to_gnode(g->neighbors[1],e->right);
        g->index = -1;
        strcpy(g->mem, "");
        break;
    
    case '*':
        g->type = 'M';
        g->neighbors = malloc(sizeof(gnode*) * 2);
        g->neighbors[0] = malloc(sizeof(gnode));
        g->neighbors[1] = malloc(sizeof(gnode));
        g->neighbors[0] = expr_to_gnode(g->neighbors[0],e->left);
        g->neighbors[1] = expr_to_gnode(g->neighbors[1],e->right);
        g->index = -1;
        strcpy(g->mem, "");
        break;
    case 'v':
        g->type = 'L';
        strcpy(g->mem, e->leaf->ld->memory);
        g->index = e->leaf->ld->index;
        g->neighbors = NULL;
        break;
    default:
        g->type = 'C'; 
        g->neighbors = NULL;
        break;
    }

    g->schd = -1;

    return g;
}

gnode* ast_to_dfg(gnode *g, stmt *ast) {
    
    if(ast->alloc != NULL) 
        return NULL;
  
    //If we reach here, then we know it is a store. 
    g->type = 'S';
    //g->reg_num = -1;

    strcpy(g->mem, ast->st->memory);

    g->index = ast->st->index;
    //For expression, create a new gnode
    g->neighbors = malloc(sizeof(gnode*) * 1);
    g->neighbors[0] = malloc(sizeof(gnode));
    g->neighbors[0] = expr_to_gnode(g->neighbors[0], ast->st->val);
    g->schd = -1;
    //printf("I%c", g->neighbors[0]->type);

    return g;

}

char ** get_mem_blocks_from_ast(stmt *block) {

    if(block->st != NULL)
        return NULL;
    if(block->alloc != NULL) {
        char **list = malloc(sizeof(char *));
        list[0] = malloc(sizeof(char) * 4);
        strcpy(list[0],block->alloc->memory);
        return list;
    }
    int cnt = 0; 
    int index = 0;
    char **mlist = malloc(sizeof(char *) * 100);
    for(int i = 0; block->block[i] != NULL; i++) {
        char **tmp = get_mem_blocks_from_ast(block->block[i]);
        if(tmp == NULL)
            continue;
        for(int j = 0; tmp[j] != NULL; j++) {
            mlist[index] = malloc(sizeof(char) * 4);
            strcpy(mlist[index++], tmp[j]);
        }
        
    }
    return mlist;

}

basic_block *blck_create_from_ast(stmt *blk) {

    basic_block *b = malloc(sizeof(basic_block));
    b->dfgs = malloc(sizeof(gnode) * 100);
    b->loc = 0;

    if(blk->alloc != NULL)
        return b;
    
    if(blk->st != NULL) {
        gnode *g = malloc(sizeof(gnode));
        g = ast_to_dfg(g, blk);
        b->dfgs[b->loc] = g;
        b->loc++;
        return b;
    }

    for(int i = 0; blk->block[i] != NULL; i++) {
        basic_block *tmp = blck_create_from_ast(blk->block[i]);
        for(int j = 0; j < tmp->loc; j++) {
            b->dfgs[b->loc] = tmp->dfgs[j];
            b->loc++;
        }
    }
    return b;

}

void pretty_print_gnode(gnode *g) {
   
    switch (g->type)
    {
    case 'S':
        printf("S");
        pretty_print_gnode(g->neighbors[0]);
        break;
    case 'A':
        printf("+");
        pretty_print_gnode(g->neighbors[0]);
        pretty_print_gnode(g->neighbors[1]);
        break;
    case 'M':
        printf("*");
        pretty_print_gnode(g->neighbors[0]);
        pretty_print_gnode(g->neighbors[1]);
        break;
    case 'L':
        printf("L");
        break;
    default:
        printf("C");
        break;
    }
}

void pretty_print_blck(basic_block *b) {

    //We have only one basic blcok for now, so ignore
   
    for(int i = 0; i < b->loc; i++) {
        pretty_print_gnode(b->dfgs[i]);
        printf("\n");
    }

}