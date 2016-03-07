#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef void clogger;
typedef enum {
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

// Global functions
void cspd_set_pattern(const char *pattern);
bool cspd_set_level(clevel_enum level);
bool cspd_set_async_mode(size_t queue_size,
                         const casync_overflow_policy overflow_policy);
void cspd_set_sync_mode(void);

// configuring logger
void cspd_logger_set_pattern(clogger *c, const char *pattern);
bool cspd_logger_set_level(clogger *c, clevel_enum level);
void cspd_logger_flush(clogger *c);

// logging
void cspd_trace(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
void cspd_debug(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
void cspd_info(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
void cspd_notice(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
void cspd_warn(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
void cspd_error(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
void cspd_critical(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
void cspd_alert(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));
void cspd_emerg(clogger *c, const char *std, ...)
    __attribute__((format(printf, 2, 3)));

void cspd_drop_all(void);

#ifdef __cplusplus
}
#endif
