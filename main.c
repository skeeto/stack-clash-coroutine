#include <stdio.h>
#include "tree.h"

static char *teams[] = {
    "Pittsburgh",   "Penguins",
    "Washington",   "Capitals",
    "Philadelphia", "Flyers",
    "New York",     "Rangers",
    "Tampa Bay",    "Lightning",
    "Boston",       "Bruins",
    "Columbus",     "Blue Jackets",
};
static const int nteams = sizeof(teams) / sizeof(*teams) / 2;

void
printer(char *k, char *v)
{
    printf("%-12s = %s\n", k, v);
}

int
main(void)
{
    struct tree *tree = 0;
    for (int i = 0; i < nteams; i++)
        tree_insert(&tree, teams[i * 2 + 0], teams[i * 2 + 1]);

    struct tree_it *it = tree_iterator(tree);
    char *k, *v;
    while (tree_next(it, &k, &v))
        printf("%-12s : %s\n", k, v);

#ifdef CALLBACK_EXAMPLE
    puts("---");
    tree_visit(tree, printer);
#endif

    tree_destroy(tree);
}
