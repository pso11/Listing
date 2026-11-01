#ifdef LIST_USE_INT
    typedef int list_elem_t;
    #define SPECIFICATOR "%-5d"
    #define POISON 10928
    #define DATA_IS_POISON(var1, var2) var1 == var2
#elif LIST_USE_DOUBLE
    typedef double list_elem_t;
    #define SPECIFICATOR "%-5lg"
    #define POISON 10928
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
    BAD_ALLOCATION            = 11
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
    #endif
    size_t head;
    size_t tail;
    size_t free;
    int size;
    node_t* node_array;
};

#ifdef LIST_DEBUG
    void list_dump(const struct list_t* list);
    void fwrite_txt_dump(const struct list_t* list, FILE* file);
    void fwrite_graph_dump(const struct list_t* list, FILE* file);
    void fwrite_stars(size_t number, FILE* file);
    struct error
    {
        const char* description;
        error_t error_name;
    };
    error_t list_veryficator(struct list_t* list);

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
                list.birth_function = __FUNCTION__; \
                list.call_file = __FILE__ ;         \
                list.call_line = __LINE__;          \
                list.val_name = #list;              \
                list_dump(&list);                   \
                list_destroy(&list);                \
                return 0;                           \
            }
#else
    #define FILL_DBG(error) ;
    #define LIST_VERYFICATION(list) ;
    #define ASSERT(list, error_status) ;
#endif

error_t list_init(struct list_t* list, int size);
error_t list_destroy(struct list_t* list);
error_t list_insert(struct list_t* list, int node, list_elem_t data);
error_t list_delete(struct list_t* list, int node);
error_t resize_up(struct list_t* list);
error_t resize_down(struct list_t* list);
int compare_doubles(double var1, double var2);

const int INCREMENT = 2;

