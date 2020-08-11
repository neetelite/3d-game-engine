#if defined(ERROR)
#undef ERROR
#endif

/* TODO(lungu): Look into corange to implement debug messages */
#define ERROR(a, ...) fprintf(stderr, "[ERROR] %s", (a), __VA_ARGS__)
#define FATAL(a, ...) fprintf(stderr, "[FATAL] %s", (a), __VA_ARGS__)
#define DEBUG(a, ...) fprintf(stderr, "[DEBUG] %s", (a), __VA_ARGS__)
#define WARNING(a, ...) fprintf(stderr, "[DEBUG] %s", (a), __VA_ARGS__)
