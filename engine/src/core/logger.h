#pragma once

#include <defines.h>

#define LOG_WARN_ENABLE 1
#define LOG_ERROR_INFO 1
#define LOG_DEBUG_ENABLE 1
#define LOG_TRACE_ENABLE 1

#if KRELEASE == 1
#define LOG_DEBUG_ENABLE 0
#define LOG_TRACE_ENABLE 0
#endif

#define LOG_LEVEL(FUNC) \
    FUNC(FATAL)   \
    FUNC(ERROR)   \
    FUNC(WARNING) \
    FUNC(INFO)    \
    FUNC(DEBUG)   \
    FUNC(TRACE)

#define LOG_LEVEL_WITH_NUM(FUNC) \
    FUNC(LOG_LEVEL_FATAL = 0)   \
    FUNC(LOG_LEVEL_ERROR = 1)   \
    FUNC(LOG_LEVEL_WARNING = 2) \
    FUNC(LOG_LEVEL_INFO = 3)    \
    FUNC(LOG_LEVEL_DEBUG = 4)   \
    FUNC(LOG_LEVEL_TRACE = 5)

typedef enum log_level {
#define X(name) name,
    LOG_LEVEL_WITH_NUM(X)
#undef X
    LOG_LEVEL_COUNT
}log_level;

b8 initianlize_logger();
void shutdown_logger();

KAPI void logger_output(log_level level, const char* message, ...);

#define KFATAL(message, ...) logger_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

#ifndef KERROR
#define KERROR(message, ...) logger_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLE == 1
#define KWARN(message, ...) logger_output(LOG_LEVEL_WARNING, message, ##__VA_ARGS__);
#else
#define KWARN(message, ...)
#endif

#if LOG_INFO_ENABLE == 1
#define KINFO(message, ...) logger_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define KINFO(message, ...)
#endif

#if LOG_DEBUG_ENABLE == 1
#define KDEBUG(message, ...) logger_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else 
#define KDEBUG(message, ...) 
#endif

#if LOG_TRACE_ENABLE == 1
#define KTRACE(message, ...) logger_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define KTRACE(message, ...)
#endif