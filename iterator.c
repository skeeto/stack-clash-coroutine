#include <stdlib.h>
#include "tree.h"

struct tree_it {
    struct tree *stack[32];
    unsigned long long state;
    int nstack;
};

struct tree_it *
tree_iterator(struct tree *t)
{
    struct tree_it *it = malloc(sizeof(*it));
    it->stack[0] = t;
    it->state = 0;
    it->nstack = 1;
    return it;
}

int
tree_next(struct tree_it *it, char **k, char **v)
{
    while (it->nstack) {
        int shift = (it->nstack - 1) * 2;
        int state = 3u & (it->state >> shift);
        struct tree *t = it->stack[it->nstack - 1];
        it->state += 1ull << shift;
        switch (state) {
            case 0:
                *k = t->key;
                *v = t->value;
                if (t->left) {
                    it->stack[it->nstack++] = t->left;
                    it->state &= ~(3ull << (shift + 2));
                }
                return 1;
            case 1:
                if (t->right) {
                    it->stack[it->nstack++] = t->right;
                    it->state &= ~(3ull << (shift + 2));
                }
                break;
            case 2:
                it->nstack--;
                break;
        }
    }
    free(it);
    return 0;
}
