#ifdef LIST_USE_INT
    typedef int list_elem_t;
    #define SPECIFICATOR "d"
    #define POISON 10928
    #define DATA_IS_POISON(var1, var2) var1 == var2
#elif LIST_USE_DOUBLE
    typedef double list_elem_t;
    #define SPECIFICATOR "lg"
    #define POISON 10928.0
    #define DATA_IS_POISON(var1, var2) compare_doubles(var1, var2)
#else
    #error "Error: Neither LIST_USE_INT nor LIST_USE_DOUBLE is defined!"
#endif

#define DELTA 10e-6

enum error_t
{
    OK                        = 0,
    NO_LIST_INITTED           = 1,
    NOT_EXSIST_ELEM           = 2,
    NEGATIVE_NODE             = 3,
    ZERO_NODE_TO_DELETE       = 4,
    NULL_POINTER_FROM_REALLOC = 5,
    LIST_NULL_POINTER         = 6,
    WRONG_SIZE                = 7,
    ZERO_ELEM_DAMAGED         = 8,
    WRONG_PREV                = 9,
    WRONG_NEXT                = 10,
    BAD_ALLOCATION            = 11,
    IDX_MORE_THAN_SIZE        = 12,
    NEGATIVE_IDX              = 13,
    WRONG_HEAD                = 14,
    WRONG_TAIL                = 15,
    WRONG_FREE                = 16,
    LOOP_EXISTS               = 17
};

struct node_t
{
    list_elem_t data;
    int next;
    int prev;
};

struct list_t
{
    #ifdef LIST_DEBUG
        size_t line;
        const char* file;
        const char* call_file;
        size_t call_line;
        const char* function;
        const char* birth_function;
        const char* val_name;
        error_t error_code;
        char* project_dir_path;
        char* log_dir_path;
        size_t dump_num;
        int add_or_del_status;
        int add_or_del_elem;
    #endif
    int head;
    int tail;
    int free;
    int size;
    size_t used_nodes;
    node_t* node_array;
};

#ifdef LIST_DEBUG
    void list_dump(struct list_t* list);
    void fwrite_txt_dump(const struct list_t* list, FILE* file);
    void fwrite_graph_dump(const struct list_t* list);
    void fwrite_html_dump(const struct list_t* list, FILE* file);
    char* create_file_buffer(const char* file_name, FILE* file);
    void define_log_file(void);
    size_t define_run_number(const char* file_name);
    void fwrite_stars(size_t number, FILE* file);
    void fprintf_invis_edges(FILE* file, const struct list_t* list);
    void fprintf_head_tail(FILE* file, const struct list_t* list);
    void fprintf_nodes(FILE* file, const struct list_t* list);
    void fprintf_beautiful(FILE* file_name, const char* string);
    void fprintf_error_inf(const struct list_t* list, FILE* file);
    void fprintf_list_data(const struct list_t* list, FILE* file);
    error_t node_veryficator(const struct list_t* list, int size);
    void fprintf_node_and_edge(FILE* file, int size, const struct list_t* list,
                               const char* edge_color, const char* label_color,
                               const char* stroke_color, const char* fill_color,
                               const char* line_status, double penwidth);

    struct error
    {
        const char* description;
        error_t error_name;
    };
    error_t list_veryficator(struct list_t* list);
    #define MAX_PATH_LEN 100
    #define MAX_FILE_NAME_LEN 15

    #define FILL_DBG(error)                \
        list -> line       = __LINE__ ;    \
        list -> file       = __FILE__;     \
        list -> function   = __FUNCTION__;

    #define LIST_VERYFICATION(list)            \
        error_status = list_veryficator(list); \
        if (error_status)                      \
        {                                      \
            return error_status;               \
        }

    #define ASSERT(list, error_status)              \
            if (error_status)                       \
            {                                       \
                list.error_code = error_status;     \
                list.call_file = __FILE__ ;         \
                list.call_line = __LINE__;          \
                list_dump(&list);                   \
                list_destroy(&list);                \
                return 0;                           \
            }
    #define DUMP_INIT(list)                     \
            list.val_name = #list;              \
            list.birth_function = __FUNCTION__; \

    #define DEFINE_LOG_FILE() define_log_file()
#else
    #define FILL_DBG(error)             ;
    #define LIST_VERYFICATION(list)     ;
    #define ASSERT(list, error_status)  ;
    #define DEFINE_LOG_FILE()           ;
    #define DUMP_INIT(list)             ;
#endif

error_t list_init(struct list_t* list, int size);
error_t list_destroy(struct list_t* list);
error_t list_insert(struct list_t* list, int node, list_elem_t data);
error_t list_delete(struct list_t* list, int node);
error_t resize_up(struct list_t* list);
int compare_doubles(double var1, double var2);

const int INCREMENT = 2;

