#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

void
tree_insert(struct tree **t, char *key, char *value)
{
    if (!*t) {
        (*t) = malloc(sizeof(**t));
        (*t)->left = 0;
        (*t)->right = 0;
        (*t)->key = key;
        (*t)->value = value;
    } else {
        int cmp = strcmp((*t)->key, key);
        if (cmp == 0)
            (*t)->value = value;
        else if (cmp < 0)
            tree_insert(&(*t)->left, key, value);
        else
            tree_insert(&(*t)->right, key, value);
    }
}

char *
tree_find(struct tree *t, char *key)
{
    if (t) {
        int cmp = strcmp(t->key, key);
        if (cmp == 0)
            return t->value;
        else if (cmp < 0)
            return tree_find(t->left, key);
        else
            return tree_find(t->right, key);
    }
    return 0;
}

void
tree_visit(struct tree *t, void (*f)(char *, char *))
{
    if (t) {
        f(t->key, t->value);
        tree_visit(t->left, f);
        tree_visit(t->right, f);
    }
}

void
tree_destroy(struct tree *t)
{
    if (t) {
        tree_destroy(t->left);
        tree_destroy(t->right);
        free(t);
    }
}
