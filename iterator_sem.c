#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "tree.h"

struct tree_it {
    struct tree *t;
    char *k;
    char *v;
    sem_t visitor;
    sem_t main;
    pthread_t thread;
};

static void
visit(struct tree *t, struct tree_it *it)
{
    if (t) {
        it->k = t->key;
        it->v = t->value;
        sem_post(&it->main);
        sem_wait(&it->visitor);
        visit(t->left, it);
        visit(t->right, it);
    }
}

static void *
thread_entrance(void *arg)
{
    struct tree_it *it = arg;
    sem_wait(&it->visitor);
    visit(it->t, it);
    sem_post(&it->main);
    return 0;
}

struct tree_it *
tree_iterator(struct tree *t)
{
    struct tree_it *it = malloc(sizeof(*it));
    it->t = t;
    sem_init(&it->visitor, 0, 0);
    sem_init(&it->main, 0, 0);
    pthread_create(&it->thread, 0, thread_entrance, it);
    return it;
}

int
tree_next(struct tree_it *it, char **k, char **v)
{
    it->k = 0;
    sem_post(&it->visitor);
    sem_wait(&it->main);
    if (it->k) {
        *k = it->k;
        *v = it->v;
        return 1;
    } else {
        pthread_join(it->thread, 0);
        sem_destroy(&it->main);
        sem_destroy(&it->visitor);
        free(it);
        return 0;
    }
}
