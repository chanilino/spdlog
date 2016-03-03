#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct _clogger clogger;
typedef enum
{
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    NOTICE = 3,
    WARN = 4,
    ERR = 5,
    CRITICAL = 6,
    ALERT = 7,
    EMERG = 8,
    OFF = 9
} clevel_enum;


typedef enum
{
    BLOCK_RETRY, // Block / yield / sleep until message can be enqueued
    DISCARD_LOG_MSG // Discard the message it enqueue fails
}casync_overflow_policy;

#ifdef __cplusplus
extern "C" {
#endif
	
	clogger* create_stdout_logger_mt(const char * name);
	void clog_info(clogger* c, const char * std, ... );
	void clog_debug(clogger* c, const char * std, ... );
	void cspd_set_level(clevel_enum level);
	clogger* cspd_rotating_logger_mt(const char* logger_name, const char* filename, size_t max_file_size, size_t max_files, bool force_flush );
      	void cspd_set_pattern(const char* pattern);
	void cspd_set_async_mode(size_t queue_size, const casync_overflow_policy overflow_policy);
        void cspdlog_set_async_mode(size_t q_size);
        void cspdlog_drop_all(void);
	

#ifdef __cplusplus
}
#endif

