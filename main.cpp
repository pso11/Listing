#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "listing.h"

int main(void)
{
    error_t error_status = OK;
    struct list_t list = {};
    DUMP_INIT(list);
    DEFINE_LOG_FILE();

    error_status = list_init(&list, 5);
    ASSERT(list, error_status);

    error_status = list_insert(&list, 0, 20);
    ASSERT(list, error_status);

    error_status = list_insert(&list, 1, 30);
    ASSERT(list, error_status);

    error_status = list_insert(&list, 2, 40);
    ASSERT(list, error_status);

    error_status = list_insert(&list, 3, 50);
    ASSERT(list, error_status);

    error_status = list_insert(&list, 4, 60);
    ASSERT(list, error_status);

    list.node_array[0].prev = 2;
    list.node_array[2].next = 0;

    list.node_array[3].prev = 5;
    list.node_array[5].next = 3;

    error_status = list_insert(&list, 4, 60);
    ASSERT(list, error_status);

    error_status = list_destroy(&list);
    ASSERT(list, error_status);

    return 0;
}


