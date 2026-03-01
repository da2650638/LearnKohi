#include "logger.h"
#include "asserts.h"

// TODO: temporary
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 initianlize_logger() {
    return TRUE;
}

void shutdown_logger() {

}

void logger_output(log_level level, const char* message, ...)
{
    const char * log_level_strings[] = {
#define X(name) "[" #name "]  ",
        LOG_LEVEL(X)
#undef X
        "[NIL_LEVEL]  "
    };
    //b8 is_error = level < LOG_LEVEL_WARNING;

    char temp_output_message[8192];
    memset(temp_output_message, 0, 8192);
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(temp_output_message, 8192, message, arg_ptr);
    va_end(arg_ptr);

    char output_message[8192];
    snprintf(output_message, 8192, "%s%s\n", log_level_strings[level], temp_output_message);

    printf("%s", output_message);
}

 void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line)
 {
    logger_output(LOG_LEVEL_FATAL,"Assertion failed: %s message: '%s', file: %s, line: %d", expression, message, file, line);
 }
