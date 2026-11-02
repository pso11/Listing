\\ норм верификатор
\\ рид ми

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <io.h>

#include "listing.h"

#ifdef LIST_DEBUG
error_t list_veryficator(struct list_t* list)
{
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


    fprintf_beautiful(file, "\nEND OF OUR MESSAGE\n");
    fputc('\n', file);
}

void fwrite_graph_dump(const struct list_t* list)
{
    FILE* graph_file = fopen("graphic_dump.txt", "w");

    fprintf(graph_file, "digraph List\n{\n");

    fprintf(graph_file, "rankdir = LR;\n");
    for (int size = 0; size < list -> size; size++)
    {
        fprintf(graph_file, "   %d -> %d;\n", size, list -> node_array[size].next);
        fprintf(graph_file, "   %d [shape = record, label = \" index = %d | data = %d | {next = %d | prev = %d} \"];\n",
                  size, size, list -> node_array[size].data, list -> node_array[size].next, list -> node_array[size].prev);
    }

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

    fprintf_list_data(list, file);
    fwrite_graph_dump(list);

    fputc('\n', file);
    fprintf(file, "Image:\n");
    fprintf(file, "<img src =\"grapic_dump_%d.png\">", list -> dump_num);

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
        {"Calloc didn't work", BAD_ALLOCATION}
    };
    size_t structure = 0;
    while (structure < sizeof(error_array) / sizeof(error_array[0]))
    {
        if (error_array[structure].error_name == list -> error_code)
            break;
        structure++;
    }

    fprintf(file, "Error code: [%d] : <%s> called in: %s:%d in function: <%s> : <%s>:%d\n\n",
                   list -> error_code, error_array[structure].description, list -> call_file,
                   list -> call_line, list -> function, list -> file, list -> line);
}

void fprintf_list_data(const struct list_t* list, FILE* file)
{
    fprintf(file, "YOUR LIST: list_t %s = [%p] created by %s():\n"
                  "size = [%d], node_array = [%p]\n"
                  "head_index = %d head = %d\n"
                  "free_index = %d\n"
                  "POISON = FREE = %d\n",
                   list -> val_name, list, list -> birth_function,
                   list -> size, list -> node_array, list -> head, list -> node_array[list -> head].data, list -> free, POISON);

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
            fprintf(file, "%7d", list -> node_array[size].data);
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

#endif


