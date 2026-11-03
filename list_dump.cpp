// рид ми

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <io.h>

#include "listing.h"

#ifdef LIST_DEBUG
error_t list_veryficator(struct list_t* list)
{
    error_t error_status = OK;

    if (list == NULL)
        return NO_LIST_INITTED;

    if (list -> size <= 0)
    {
        FILL_DBG(WRONG_SIZE);
        return WRONG_SIZE;
    }

    if (list -> node_array == 0)
    {
        FILL_DBG(LIST_NULL_POINTER);
        return LIST_NULL_POINTER;
    }

    if (list -> head >= list -> size)
    {
        FILL_DBG(WRONG_HEAD);
        return WRONG_HEAD;
    }

    if (list -> tail >= list -> size)
    {
        FILL_DBG(WRONG_TAIL);
        return WRONG_TAIL;
    }

    if (list -> free >= list -> size)
    {
        FILL_DBG(WRONG_FREE);
        return WRONG_FREE;
    }

    for (int i = 0; i < list -> size; i++)
    {
        if (error_status = node_veryficator(list, i))
        {
            FILL_DBG(error_status);
            return error_status;
        }
    }

    return OK;
}

void list_dump(struct list_t* list)
{
    list -> dump_num = define_run_number("dump_num");

    printf("Hiiii!!!! LOOK AT MAIN.LOG!!!\n");

    FILE* file = fopen("main.log", "a+");
    fwrite_txt_dump(list, file);
    fclose(file);

    file = fopen("html_log.html", "a+");
    fwrite_html_dump(list, file);
    fclose(file);

}

void fwrite_stars(size_t number, FILE* file)
{
    for (size_t i = 0; i < number; i++)
        fputc('*', file);
}

int compare_doubles(double var1, double var2)
{
    return (fabs(var1 - var2) < DELTA);
}

void fwrite_txt_dump(const struct list_t* list, FILE* file)
{
    fprintf_beautiful(file, "\nMEET ERROR IN YOUR PROGRAMM READ ALL INFORMATION DOWN\n");
    fprintf(file,"\nDUMP %d\n", list -> dump_num);

    fprintf_error_inf(list, file);
    fprintf_list_data(list, file);

    fprintf_beautiful(file, "\nEND OF OUR MESSAGE\n");
    fputc('\n', file);
}

void fwrite_graph_dump(const struct list_t* list)
{
    FILE* graph_file = fopen("graphic_dump.txt", "w");

    fprintf(graph_file, "digraph List\n{\n");
    fprintf(graph_file, "rankdir = LR;\n");
    // splines = ortho;\n

    fprintf_invis_edges(graph_file, list);

    fprintf_nodes(graph_file, list);

    fprintf_head_tail(graph_file, list);

    fprintf(graph_file, "}\n");
    fclose(graph_file);

    char dot_cmd[MAX_PATH_LEN] = {};
    snprintf(dot_cmd, sizeof(dot_cmd), "dot \"graphic_dump.txt\" -Tpng -o grapic_dump_%d.png", list -> dump_num);

    system(dot_cmd);
}

size_t define_run_number(const char* file_name)
{
    size_t run_number = 1;

    FILE* file = fopen(file_name, "r");
    if (file == NULL)
    {
        file = fopen(file_name, "w+");
        fprintf(file, "%d", run_number + 1);
        fclose(file);
        return 1;
    }
    else
        fscanf(file, "%d", &run_number);
    fclose(file);

    file = fopen(file_name, "w+");
    fprintf(file, "%d", run_number + 1);
    fclose(file);
    return run_number;
}

void define_log_file(void)
{
    char* project_dir_path = (char*)calloc(MAX_PATH_LEN, sizeof(char));
    _getcwd(project_dir_path, MAX_PATH_LEN);

    char* log_dir_path = (char*)calloc(MAX_PATH_LEN, sizeof(char));
    sprintf(log_dir_path, "%s\\LOG", project_dir_path);

    _mkdir(log_dir_path);
    _chdir(log_dir_path);

    size_t run_number = define_run_number("run_number");
    char log_name[MAX_FILE_NAME_LEN] = {};
    snprintf(log_name, sizeof(log_name), "log_%d", run_number);

    _mkdir(log_name);
    _chdir(log_name);
}

void fwrite_html_dump(const struct list_t* list, FILE* file)
{
    fprintf(file, "<!DOCTYPE html>\n<html>\n<pre style = \"font-size: 20px;\">");

    if (list -> add_or_del_status == 1)
    {
        fputc('\n', file);
        fwrite_stars(25, file);
        fprintf(file, "\nADD after %d \n", list -> add_or_del_elem);
        fwrite_stars(25, file);
        fputc('\n', file);
    }
    else if (list -> add_or_del_status == -1)
    {
        fputc('\n', file);
        fwrite_stars(25, file);
        fprintf(file, "\nDELETE %d \n", list -> add_or_del_elem);
        fwrite_stars(25, file);
        fputc('\n', file);
    }
    else
    {
        fputc('\n', file);
        fwrite_stars(25, file);
        fprintf(file, "\nINITED \n");
        fwrite_stars(25, file);
        fputc('\n', file);
    }

    fprintf_error_inf(list, file);
    fprintf_list_data(list, file);
    fwrite_graph_dump(list);

    fputc('\n', file);
    fprintf(file, "Image:\n");
    fprintf(file, "<img src =\"grapic_dump_%d.png\">", list -> dump_num);
    fprintf_beautiful(file, "\nEND OF OUR MESSAGE\n");
    fputc('\n', file);

    fprintf(file, "</pre>\n</html>");
}

void fprintf_beautiful(FILE* file, const char* string)
{
    fputc('\n', file);
    fwrite_stars(25, file);
    fprintf(file,"%s", string);
    fwrite_stars(25, file);
    fputc('\n', file);
}

void fprintf_error_inf(const struct list_t* list, FILE* file)
{
    struct error error_array[] =
    {
        {"Everything is OK", OK},
        {"NULL pointer of node_array", LIST_NULL_POINTER},
        {"Realloc didn't find place to replace list", NULL_POINTER_FROM_REALLOC},
        {"Tou wanna delete negative node", NEGATIVE_NODE},
        {"NO list structure was inited", NO_LIST_INITTED},
        {"You wanna delete or add after non-existent node", NOT_EXSIST_ELEM},
        {"You wanna delete zero node", ZERO_NODE_TO_DELETE},
        {"Size less than ZERO", WRONG_SIZE},
        {"Zero element was damaged", ZERO_ELEM_DAMAGED},
        {"Wrong value of next member in node structure", WRONG_NEXT},
        {"Wrong value of prev member in node structure", WRONG_PREV},
        {"Calloc didn't work", BAD_ALLOCATION},
        {"Next or previous element index more than size of list",  IDX_MORE_THAN_SIZE},
        {"Next or previous element index less than zero", NEGATIVE_IDX},
        {"Tail index more than size of list", WRONG_TAIL},
        {"Head index more than size of list", WRONG_HEAD},
        {"Free index more than size of list", WRONG_FREE}
    };
    size_t structure = 0;
    while (structure < sizeof(error_array) / sizeof(error_array[0]))
    {
        if (error_array[structure].error_name == list -> error_code)
            break;
        structure++;
    }

    fprintf(file, "Error code: [%d] : %s called in: %s:%d in function: %s : %s:%d\n\n",
                   list -> error_code, error_array[structure].description, list -> call_file,
                   list -> call_line, list -> function, list -> file, list -> line);

    fflush(file);
}

void fprintf_list_data(const struct list_t* list, FILE* file)
{
    fprintf(file, "YOUR LIST: list_t %s = [%p] created by %s():\n"
                  "size = [%d], node_array = [%p]\n"
                  "head_index = %d\n"
                  "tail_index = %d\n"
                  "free_index = %d\n"
                  "POISON = FREE = %d\n",
                   list -> val_name, list, list -> birth_function,
                   list -> size, list -> node_array, list -> head, list -> tail, list -> free, POISON);

    fflush(file);

    fprintf(file,"%8s", "INDEX");
    for (int size = 0; size < list -> size; size++)
        fprintf(file, "%7d", size);
    fputc('\n', file);

    fprintf(file,"%8s", "DATA ");
    for (int size = 0; size < list -> size; size++)
    {
        if (DATA_IS_POISON(list -> node_array[size].data, POISON))
        {
            if (size == 0)
                fprintf(file, "%7s", "POISON");
            else
                fprintf(file, "%7s", "FREE");
        }
        else
            fprintf(file, "%7" SPECIFICATOR "", list -> node_array[size].data);
    }
    fputc('\n', file);

    fprintf(file,"%8s", "NEXT ");
    for (int size = 0; size < list -> size; size++)
        fprintf(file, "%7d", list -> node_array[size].next);
    fputc('\n', file);

    fprintf(file,"%8s", "PREV ");
    for (int size = 0; size < list -> size; size++)
        fprintf(file, "%7d", list -> node_array[size].prev);
    fputc('\n', file);

}

error_t node_veryficator(const struct list_t* list, int size)
{
    if (list -> node_array[size].next >= list -> size)
        return IDX_MORE_THAN_SIZE;
    if (list -> node_array[size].prev >= list -> size)
        return IDX_MORE_THAN_SIZE;
    if ((list -> node_array[size].prev) < 0 && (list -> node_array[size].data != POISON))
        return NEGATIVE_IDX;
    if ((list -> node_array[size].next) < 0 && (list -> node_array[size].data != POISON))
        return NEGATIVE_IDX;
    if ((list -> node_array[list -> node_array[size].next].prev != size) && list -> node_array[size].data != POISON)
        return WRONG_NEXT;
    if ((list -> node_array[list -> node_array[size].prev].next != size) && list -> node_array[size].data != POISON)
        return WRONG_PREV;

    return OK;
}

void fprintf_node_and_edge(FILE* file, int size, const struct list_t* list, const char* edge_color,
                           const char* label_color, const char* stroke_color, const char* fill_color, const char* line_status)
{
    fprintf(file,  "   %d -> %d [%s, color = \"%s\"];\n", size, list -> node_array[size].next, line_status, edge_color);
    fprintf(file,  "   %d [style = filled, fontcolor = \"%s\", fillcolor = \"%s\", color = \"%s\", shape = record, label = \" "
                        "index = %d | data = %" SPECIFICATOR " | {next = %d | prev = %d} \"];\n",
                        size, label_color, fill_color, stroke_color, size, list -> node_array[size].data,
                        list -> node_array[size].next, list -> node_array[size].prev);
}

void fprintf_invis_edges(FILE* file, const struct list_t* list)
{
    fprintf(file, "0");
    for (int size = 1; size < list -> size; size++)
        fprintf(file, " -> %d", size);
    fprintf(file, " [style=invis, weight=1000];");
}

void fprintf_nodes(FILE* file, const struct list_t* list)
{
    for (int size = 0; size < list -> size; size++)
    {
        if (node_veryficator(list, size))
            fprintf_node_and_edge(file, size, list, "#e90a0a", "#FFFFFF", "#d76767", "#700909", "dir = both");

        else if (size == 0)
            fprintf_node_and_edge(file, size, list, "#000000", "#000000", "#000000", "#919191", "dir = both");

        else if (DATA_IS_POISON(list -> node_array[size].data, POISON))
            fprintf_node_and_edge(file, size, list, "#57cc57", "#000000", "#083308", "#57cc57", "dir = forward");

        else if (size == list -> tail)
            fprintf_node_and_edge(file, size, list, "#000000", "#000000", "#310431", "#c034c0", "dir = forward");

        else if (size == list -> head)
            fprintf_node_and_edge(file, size, list, "#000000", "#000000", "#543a0a", "#FFA500", "dir = forward");

        else
            fprintf_node_and_edge(file, size, list, "#000000", "#000000", "#083308", "#FFFFFF", "dir = both");

        fflush(file);
    }
}

void fprintf_head_tail(FILE* file, const struct list_t* list)
{
    fprintf(file, "   head -> %d [color = \"#ebae3c\"]; \n", list -> head);
    fprintf(file, "   head [style = filled, fillcolor = \"#ebae3c\", color = \"#543a0a\", shape = record, label = \"HEAD\"];\n");

    fprintf(file, "   tail -> %d [color = \"#e54fe5\"];\n", list -> tail);
    fprintf(file, "   tail [style = filled, fillcolor = \"#e54fe5\", color = \"#543a0a\", shape = record, label = \"TAIL\"];\n");

    fprintf(file, "  free -> %d [color = \"#065906\"];\n", list -> free);
    fprintf(file, "  free [style = filled, fillcolor = \"#93e393\", color = \"#065906\", shape = record, label = \"FREE\"];\n");

    fflush(file);
}

#endif


