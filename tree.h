#ifndef TREE_H
#define TREE_H

struct tree {
    struct tree *left;
    struct tree *right;
    char *key;
    char *value;
};

void  tree_insert(struct tree **, char *k, char *v);
char *tree_find(struct tree *, char *key);
void  tree_visit(struct tree *, void (*f)(char *, char *));
void  tree_destroy(struct tree *);

struct tree_it;

struct tree_it *tree_iterator(struct tree *);
int             tree_next(struct tree_it *, char **k, char **v);

#endif
