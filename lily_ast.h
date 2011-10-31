#ifndef LILY_AST_H
# define LILY_AST_H

#include "lily_lexer.h"
#include "lily_symtab.h"

/* Based off of http://lambda.uta.edu/cse5317/notes/node25.html. */
struct lily_ast_list;

typedef enum {
    expr_assign
} lily_expr_op;

typedef struct lily_ast_ {
    enum {
        func_call, var, binary
    } expr_type;
    lily_object *result;
    union {
        lily_object *object;
        struct {
            lily_symbol *sym;
            int num_args;
            struct lily_ast_list *args;
        } call;
        struct lily_bin_expr {
            int priority;
            lily_expr_op op;
            struct lily_ast_ *left;
            struct lily_ast_ *right;
            struct lily_ast_ *parent;
        } bin_expr;
    } data;
} lily_ast;

struct lily_ast_list {
    lily_ast *ast;
    struct lily_ast_list *next;
};

typedef struct {
    lily_ast **saved_trees;
    lily_ast **tree_pool;
    struct lily_ast_list **list_pool;
    lily_ast *root;
    lily_ast *active;
    int save_index;
    int save_size;
    int tree_index;
    int tree_size;
    int list_index;
    int list_size;
    lily_excep_data *error; 
} lily_ast_pool;

void lily_ast_add_arg(lily_ast_pool *, lily_ast *, lily_ast *);
void lily_ast_enter_func(lily_ast_pool *, lily_symbol *);
void lily_ast_free_pool(lily_ast_pool *);
lily_ast_pool *lily_ast_init_pool(lily_excep_data *, int);
void lily_ast_pop_tree(lily_ast_pool *);
void lily_ast_push_binary_op(lily_ast_pool *, lily_token);
void lily_ast_push_var(lily_ast_pool *, lily_object *);
void lily_ast_reset_pool(lily_ast_pool *);
void lily_save_active_ast(lily_ast_pool *);
#endif
