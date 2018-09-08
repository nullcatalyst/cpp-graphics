#ifndef util_debug_hpp
#define util_debug_hpp

#ifdef DEBUG
    #include <cstdio> // fprintf, stderr

    #define ASSERT(cond, msg)           \
        do {                            \
            if (!(cond)) {              \
                fprintf(stderr, "[%s:%d] Assert failed in %s(): %s\n", __FILE__, __LINE__, __func__, message); \
                abort();                \
            }                           \
        } while (0)

    #define UNREACHABLE()               \
        do {                            \
            fprintf(stderr, "[%s:%d] This code should not be reached in %s()\n", __FILE__, __LINE__, __func__); \
            abort();                    \
        } while (0)
#else
    #define ASSERT(cond, msg) ((void) 0)

    #if __has_builtin(__builtin_unreachable)
        #define UNREACHABLE() __builtin_unreachable()
    #elif defined(_MSC_VER)
        #define UNREACHABLE() __assume(0)
    #else
        #define UNREACHABLE() ((void) 0)
    #endif
#endif

#endif /* util_debug_hpp */
