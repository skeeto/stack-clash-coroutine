#include <stdlib.h>
#include <setjmp.h>
#include "tree.h"

#define STACK_SIZE (1UL << 20)

struct tree_it {
    char *k;
    char *v;
    char *stack;
    char *gap;
    jmp_buf coroutine;
    jmp_buf yield;
};

static void
coroutine(struct tree *t, struct tree_it *it)
{
    if (t) {
        it->k = t->key;
        it->v = t->value;
        if (!setjmp(it->coroutine))
            longjmp(it->yield, 1);
        coroutine(t->left, it);
        coroutine(t->right, it);
    }
}

static void
coroutine_init(struct tree *t, struct tree_it *it)
{
    if (!setjmp(it->coroutine))
        longjmp(it->yield, 1);
    coroutine(t, it);
    longjmp(it->yield, 1);
}

struct tree_it *
tree_iterator(struct tree *t)
{
    struct tree_it *it = malloc(sizeof(*it));
    it->stack = malloc(STACK_SIZE);
    char marker;
    char gap[&marker - it->stack - STACK_SIZE];
    it->gap = gap; // prevent optimization
    if (!setjmp(it->yield))
        coroutine_init(t, it);
    return it;
}

int
tree_next(struct tree_it *it, char **k, char **v)
{
    it->k = 0;
    if (!setjmp(it->yield))
        longjmp(it->coroutine, 1);
    if (it->k) {
        *k = it->k;
        *v = it->v;
        return 1;
    } else {
        free(it->stack);
        free(it);
        return 0;
    }
}
