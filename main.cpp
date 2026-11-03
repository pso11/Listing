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
    list_dump(&list);

    error_status = list_insert(&list, 0, 20);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_insert(&list, 1, 30);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_insert(&list, 2, 40);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_insert(&list, 1, 70);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_delete(&list, 2);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_insert(&list, 3, 50);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_delete(&list, 3);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_insert(&list, 0, 20);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_insert(&list, 1, 30);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_insert(&list, 2, 40);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_insert(&list, 1, 70);
    ASSERT(list, error_status);
    list_dump(&list);
    error_status = list_insert(&list, 0, 20);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_insert(&list, 1, 30);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_insert(&list, 2, 40);
    ASSERT(list, error_status);
    list_dump(&list);

    list.node_array[1].next = 500;

    error_status = list_insert(&list, 1, 70);
    ASSERT(list, error_status);
    list_dump(&list);

    error_status = list_destroy(&list);
    ASSERT(list, error_status);

    return 0;
}


