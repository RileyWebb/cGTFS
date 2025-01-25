#ifndef _DEBUG_H
#define _DEBUG_H

#define DEBUG_USE_COLOR
//#define DEBUG_ASSERT_EXITS

#define DEBUG_INFO(...)                                                        \
	debug_write(DEBUG_LOG_TYPE_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define DEBUG_WARN(...)                                                        \
	debug_write(DEBUG_LOG_TYPE_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define DEBUG_ERROR(...)                                                       \
	debug_write(DEBUG_LOG_TYPE_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define DEBUG_FATAL(...)                                                       \
	debug_write(DEBUG_LOG_TYPE_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#ifndef NDEBUG
#define DEBUG
#endif

#ifdef DEBUG
#define DEBUG_DEBUG(...)                                                       \
	debug_write(DEBUG_LOG_TYPE_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

#ifdef DEBUG_ASSERT_EXITS
#define DEBUG_ASSERT(x)                                                        \
	if (!x) {                                                                  \
	debug_write(DEBUG_LOG_TYPE_ASSERT, __FILE__, __LINE__,                 \
					"ASSERTION FAILED: %s", #x);                               \
    debug_flush_buffers();                                                     \
    exit(-1);                                                                  \
    }
#else
#define DEBUG_ASSERT(x)                                                        \
    if (!x)                                                                    \
	debug_write(DEBUG_LOG_TYPE_ASSERT, __FILE__, __LINE__,                 \
					"ASSERTION FAILED: %s", #x)
#endif

#define DEBUG_TRACE()                                                          \
	debug_write(DEBUG_LOG_TYPE_TRACE, __FILE__, __LINE__,                  \
					"FUNCTION: %s()", __FUNCTION__)
#ifdef STACKTRACE

#endif
#else
#define DEBUG_DEBUG(...)
#define DEBUG_ASSERT(x)
#define DEBUG_TRACE()
#endif

enum {
    DEBUG_LOG_TYPE_TRACE = 0,
    DEBUG_LOG_TYPE_DEBUG = 1,
    DEBUG_LOG_TYPE_INFO = 2,
    DEBUG_LOG_TYPE_WARN = 3,
    DEBUG_LOG_TYPE_ERROR = 4,
    DEBUG_LOG_TYPE_FATAL = 5,
    DEBUG_LOG_TYPE_ASSERT = 6,
    DEBUG_LOG_TYPE_CONSOLE = 7
};

void debug_write(int level, const char *file, int line, const char *fmt, ...);
int debug_register(void *buffer);
void debug_flush();
//void debug_init();

#endif
