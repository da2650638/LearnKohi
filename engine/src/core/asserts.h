#include <defines.h>

#define KASSERTIONS_ENABLE 

#ifdef _MSC_VER
#include <intrin.h>
#define debugbreak() __debugbreak()
#else
#define debugbreak() __builtin_trap()
#endif

KAPI void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

#define KASSERT(expr) \
    do { \
        if(expr){} \
        else { \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugbreak(); \
        } \
    } while(0)

#define KASSERT_MSG(expr, message)  \
    do { \
        if(expr) {}\
        else {\
            report_assertion_failure(#expr, message, __FILE__, __LINE__); \
            debugbreak(); \
        } \
    } while(0)

#ifdef _DEBUG
#define KASSERT_DEBUG(expr) KASSERT(expr)
#define KASSERT_DEBUG_MSG(expr, message) KASSERT_MSG(expr, message)
#else
#define KASSERT_DEBUG(expr) //Do nothing
#define KASSERT_DEBUG_MSG(expr, message) //Do nothing
#endif
