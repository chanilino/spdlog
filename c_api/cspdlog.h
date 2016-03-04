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

	//Constructors	
	clogger* create_stdout_logger_mt(const char * name);
	clogger* create_simple_file_logger_st(const char * name, const char* file, bool force_flush);
	clogger* cspd_rotating_logger_mt(const char* logger_name, const char* filename, size_t max_file_size, size_t max_files, bool force_flush );
      
	// Global functions	
      	void cspd_set_pattern(const char* pattern);
	bool cspd_set_level(clevel_enum level);
	bool cspd_set_async_mode(size_t queue_size, const casync_overflow_policy overflow_policy);
	
	//logging
	void clog_info(clogger* c, const char * std, ... );
	void clog_debug(clogger* c, const char * std, ... );
        void cspd_drop_all(void);
	
	//configuring logger	
	void c_logger_set_pattern(clogger * c, const char* pattern);

#ifdef __cplusplus
}
#endif

