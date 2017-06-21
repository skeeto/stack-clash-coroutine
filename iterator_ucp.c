#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <ucontext.h>
#include "tree.h"

struct tree_it {
    char *k;
    char *v;
    ucontext_t coroutine;
    ucontext_t yield;
};

static struct tree *tree_arg;
static struct tree_it *tree_it_arg;

static void
coroutine(struct tree *t, struct tree_it *it)
{
    if (t) {
        it->k = t->key;
        it->v = t->value;
        swapcontext(&it->coroutine, &it->yield);
        coroutine(t->left, it);
        coroutine(t->right, it);
    }
}

static void
coroutine_init(void)
{
    coroutine(tree_arg, tree_it_arg);
}

struct tree_it *
tree_iterator(struct tree *t)
{
    struct tree_it *it = malloc(sizeof(*it));
    it->coroutine.uc_stack.ss_sp = malloc(SIGSTKSZ);
    it->coroutine.uc_stack.ss_size = SIGSTKSZ;
    it->coroutine.uc_link = &it->yield;
    getcontext(&it->coroutine);
    makecontext(&it->coroutine, coroutine_init, 0);
    tree_arg = t;
    tree_it_arg = it;
    return it;
}

int
tree_next(struct tree_it *it, char **k, char **v)
{
    it->k = 0;
    swapcontext(&it->yield, &it->coroutine);
    if (it->k) {
        *k = it->k;
        *v = it->v;
        return 1;
    } else {
        free(it->coroutine.uc_stack.ss_sp);
        free(it);
        return 0;
    }
}
