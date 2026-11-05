#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "listing.h"

error_t list_init(struct list_t* list, int size)
{
    error_t error_status = OK;

    if (list == NULL)
        return NO_LIST_INITTED;

    #ifdef LIST_DEBUG
    list -> add_or_del_status = 0;
    #endif

    list -> node_array = (node_t*)calloc(size, sizeof(node_t));

    if (list -> node_array == NULL)
    {
        error_status = BAD_ALLOCATION;
        FILL_DBG(error_status);
        return error_status;
    }

    for (int i = 1; i < size; i++)
    {
        if (i == size - 1)
            list -> node_array[i].next = i;
        else
            list -> node_array[i].next = i + 1;

        list -> node_array[i].prev = -1;
        list -> node_array[i].data = POISON;
    }
    list -> node_array[0].data = POISON;

    list -> head       = 0;
    list -> tail       = 0;
    list -> free       = 1;
    list -> size       = size;
    list -> used_nodes = 0;

    LIST_VERYFICATION(list);

    return error_status;
}

error_t list_destroy(struct list_t* list)
{
    error_t error_status = OK;

    if (list == NULL)
        return NO_LIST_INITTED;

    LIST_VERYFICATION(list);

    for (int i = 0; i < list -> size; i++)
    {
        list -> node_array[i].data = POISON;
        list -> node_array[i].next = -1;
        list -> node_array[i].prev = -1;
    }

    free(list -> node_array);

    #ifdef LIST_DEBUG
    free(list -> project_dir_path);
    free(list -> log_dir_path);
    #endif

    list -> node_array = NULL;
    list = NULL;

    return error_status;
}

error_t list_insert(struct list_t* list, int node, list_elem_t data)
{
    error_t error_status = OK;

    if (list == NULL)
        return NO_LIST_INITTED;

    if (node < 0)
    {
        FILL_DBG(NEGATIVE_NODE);
        return NEGATIVE_NODE;
    }

    if (list -> node_array[node].prev == -1)
    {
        FILL_DBG(NOT_EXSIST_ELEM);
        return NOT_EXSIST_ELEM;
    }

    #ifdef LIST_DEBUG
    list -> add_or_del_status = 1;
    list -> add_or_del_elem   = node;
    #endif

    LIST_VERYFICATION(list);

    (list -> used_nodes)++;

    if (list -> node_array[list -> free].next == list -> free)
        resize_up(list);

    size_t free_head = list -> node_array[list -> free].next;

    list -> node_array[list -> free].data = data;
    list -> node_array[list -> free].next = list -> node_array[node].next;
    list -> node_array[list -> node_array[node].next].prev = list -> free;
    list -> node_array[list -> free].prev = node;
    list -> node_array[node].next = list -> free;

    list -> free = free_head;
    list -> head = list -> node_array[0].next;
    list -> tail = list -> node_array[0].prev;

    LIST_VERYFICATION(list);

    return error_status;
}

error_t list_delete(struct list_t* list, int node)
{
    error_t error_status = OK;

    if (list == NULL)
        return NO_LIST_INITTED;

    if (node < 0)
    {
        FILL_DBG(NEGATIVE_NODE);
        return NEGATIVE_NODE;
    }

    if (node == 0)
    {
        FILL_DBG(ZERO_NODE_TO_DELETE);
        return ZERO_NODE_TO_DELETE;
    }

    if (list -> node_array[node].prev == -1)
    {
        FILL_DBG(NOT_EXSIST_ELEM);
        return NOT_EXSIST_ELEM;
    }

    #ifdef LIST_DEBUG
    list -> add_or_del_status = -1;
    list -> add_or_del_elem   = node;
    #endif

    LIST_VERYFICATION(list);

    (list -> used_nodes)--;

    list -> node_array[node].data = POISON;
    list -> node_array[list -> node_array[node].prev].next = list -> node_array[node].next;
    list -> node_array[list -> node_array[node].next].prev = list -> node_array[node].prev;

    list -> node_array[node].prev = -1;

    list -> node_array[node].next = list -> free;
    list -> free = node;


    LIST_VERYFICATION(list);

    return error_status;
}

error_t resize_up(struct list_t* list)
{
    if (list == NULL)
        return NO_LIST_INITTED;

    size_t temp_size = list -> size;
    list -> size *= INCREMENT;

    node_t* temp_pointer = (node_t*)realloc(list -> node_array, sizeof(node_t) * (list -> size));

    if (temp_pointer != NULL)
    {
        list -> node_array = temp_pointer;

        list -> node_array[list -> free].next = temp_size;
        for (int i = temp_size; i < list -> size; i++)
        {
            if (i == list -> size - 1)
                list -> node_array[i].next = i;
            else
                list -> node_array[i].next = i + 1;

            list -> node_array[i].prev = -1;
            list -> node_array[i].data = POISON;
        }
    }
    else
        return NULL_POINTER_FROM_REALLOC;

    return OK;

}

