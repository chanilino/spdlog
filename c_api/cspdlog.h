#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

typedef void clogger;
typedef void csink;
typedef enum {
  CSPD_TRACE = 0,
  CSPD_DEBUG = 1,
  CSPD_INFO = 2,
  CSPD_NOTICE = 3,
  CSPD_WARN = 4,
  CSPD_ERR = 5,
  CSPD_CRITICAL = 6,
  CSPD_ALERT = 7,
  CSPD_EMERG = 8,
  CSPD_OFF = 9
} clevel_enum;

typedef enum {
  BLOCK_RETRY,    // Block / yield / sleep until message can be enqueued
  DISCARD_LOG_MSG // Discard the message it enqueue fails
} casync_overflow_policy;

#ifdef __cplusplus
extern "C" {
#endif

clogger *cspd_get(const char *name);
// Constructors
clogger *cspd_stdout_logger_mt(const char *name);
clogger *cspd_stdout_logger_st(const char *name);
clogger *cspd_stderr_logger_mt(const char *name);
clogger *cspd_stderr_logger_st(const char *name);

// force_flush force flush at each log msg
clogger *cspd_simple_file_logger_st(const char *name, const char *file,
                                    bool force_flush);

clogger *cspd_rotating_logger_mt(const char *logger_name, const char *filename,
                                 size_t max_file_size, size_t max_files,
                                 bool force_flush);
clogger *cspd_rotating_logger_st(const char *logger_name, const char *filename,
                                 size_t max_file_size, size_t max_files,
                                 bool force_flush);

clogger *cspd_daily_logger_mt(const char *logger_name, const char *filename,
                              int hour, int minute, bool force_flush);
clogger *cspd_daily_logger_st(const char *logger_name, const char *filename,
                              int hour, int minute, bool force_flush);

#if defined(__linux__) || defined(__APPLE__)
clogger *cspd_syslog_logger(const char *logger_name, const char *ident,
                            int syslog_option);
#endif

clogger *cpsd_create_logger_with_sink(const char* logger_name, csink * sink);
clogger *cpsd_create_logger_with_sinks(const char* logger_name, ...);

// Create sinks
csink *cspd_rotating_file_sink_mt( const char *filename, const char* extension,
		size_t max_file_size, size_t max_files,
		bool force_flush);
// Global functions
void cspd_set_pattern(const char *pattern);
bool cspd_set_level(clevel_enum level);
bool cspd_set_async_mode(size_t queue_size,
                         const casync_overflow_policy overflow_policy);
void cspd_set_sync_mode(void);

// configuring logger
bool cspd_logger_should_log(clogger* c, clevel_enum level);
void cspd_logger_set_pattern(clogger *c, const char *pattern);
bool cspd_logger_set_level(clogger *c, clevel_enum level);
void cspd_logger_flush(clogger *c);

// logging
static void cspd_trace(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
static void cspd_debug(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
static void cspd_info(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
static void cspd_notice(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
static void cspd_warn(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
static void cspd_error(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
static void cspd_critical(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
static void cspd_alert(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
static void cspd_emerg(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));


void _cspd_trace(clogger *c, const char *msg);
void _cspd_debug(clogger *c, const char *msg);
void _cspd_info(clogger* c, const char* msg);
void _cspd_notice(clogger *c, const char *msg);
void _cspd_warn(clogger *c, const char *msg);
void _cspd_error(clogger *c, const char *msg);
void _cspd_critical(clogger *c, const char *msg);
void _cspd_alert(clogger *c, const char *msg);
void _cspd_emerg(clogger *c, const char *msg);

#define _VA_BODY_LOG_(clogger, format, va, level_log, func_log_msg)	{\
	char *msg;\
	if(!cspd_logger_should_log(clogger, level_log)){return;}\
	if(vasprintf(&msg, format, va ) < 0 ){ return;}\
	func_log_msg(c, msg);\
	free(msg);\
	}

static inline void cspd_trace_(clogger *c, const char *format, va_list va){
	_VA_BODY_LOG_(c, format, va, CSPD_TRACE, _cspd_trace);
}
static inline void cspd_debug_(clogger *c, const char *format, va_list va){
	_VA_BODY_LOG_(c, format, va, CSPD_DEBUG, _cspd_debug);
}
static inline void cspd_info_(clogger *c, const char *format, va_list va){
	_VA_BODY_LOG_(c, format, va, CSPD_INFO, _cspd_info);
}
static inline void cspd_notice_(clogger *c, const char *format, va_list va){
	_VA_BODY_LOG_(c, format, va, CSPD_NOTICE, _cspd_notice);
}
static inline void cspd_warn_(clogger *c, const char *format, va_list va){
	_VA_BODY_LOG_(c, format, va, CSPD_WARN, _cspd_warn);
}
static inline void cspd_error_(clogger *c, const char *format, va_list va){
	_VA_BODY_LOG_(c, format, va, CSPD_ERR, _cspd_error);
}
static inline void cspd_critical_(clogger *c, const char *format, va_list va){
	_VA_BODY_LOG_(c, format, va, CSPD_CRITICAL, _cspd_critical);
}
static inline void cspd_alert_(clogger *c, const char *format, va_list va){
	_VA_BODY_LOG_(c, format, va, CSPD_ALERT, _cspd_alert);
}
static inline void cspd_emerg_(clogger *c, const char *format, va_list va){
	_VA_BODY_LOG_(c, format, va, CSPD_EMERG, _cspd_emerg);
}


#define _LOG_VA_BODY( func_log_msg ,clogger, format) 	\
	va_list va;\
	va_start(va, format);\
	func_log_msg(c, format, va);\
	va_end(va);

static inline void cspd_trace(clogger *c, const char *format, ...){		_LOG_VA_BODY(cspd_trace_, c, format);}
static inline void cspd_debug(clogger *c, const char *format, ...){		_LOG_VA_BODY(cspd_debug_, c, format);}
static inline void cspd_info(clogger *c, const char *format, ...){		_LOG_VA_BODY(cspd_info_, c, format);}
static inline void cspd_notice(clogger *c, const char *format, ...){	_LOG_VA_BODY(cspd_notice_, c, format);}
static inline void cspd_warn(clogger *c, const char *format, ...){		_LOG_VA_BODY(cspd_warn_, c, format);}
static inline void cspd_error(clogger *c, const char *format, ...){		_LOG_VA_BODY(cspd_error_, c, format);}
static inline void cspd_critical(clogger *c, const char *format, ...){	_LOG_VA_BODY(cspd_critical_, c, format);}
static inline void cspd_alert(clogger *c, const char *format, ...){		_LOG_VA_BODY(cspd_alert_, c, format);}
static inline void cspd_emerg(clogger *c, const char *format, ...){		_LOG_VA_BODY(cspd_emerg_, c, format);}




void cspd_logger_drop(const char* name);
void cspd_drop_all(void);

#ifdef __cplusplus
}
#endif
