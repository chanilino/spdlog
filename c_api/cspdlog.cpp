#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>

#include "spdlog/spdlog.h"
#include "cspdlog.h"

struct _clogger {
  std::shared_ptr<spdlog::logger> logger;
};
extern "C" {
static inline clogger *alloc_clogger(void) {
  clogger *c = (clogger *)calloc(1, sizeof(*c));
  return c;
}

clogger *cspd_get(const char *name) {
  clogger *c = alloc_clogger();
  if (c != NULL) {
    c->logger = spdlog::get(name);
  }
  return c;
}

clogger *cspd_stdout_logger_mt(const char *name) {
  clogger *c = alloc_clogger();
  if (c != NULL) {
    c->logger = spdlog::stdout_logger_mt(name);
  }
  return c;
}

clogger *cspd_stdout_logger_st(const char *name) {
  clogger *c = alloc_clogger();
  if (c != NULL) {
    c->logger = spdlog::stdout_logger_st(name);
  }
  return c;
}

clogger *cspd_stderr_logger_mt(const char *name) {
  clogger *c = alloc_clogger();
  if (c != NULL) {
    c->logger = spdlog::stderr_logger_mt(name);
  }
  return c;
}

clogger *cspd_stderr_logger_st(const char *name) {
  clogger *c = alloc_clogger();
  if (c != NULL) {
    c->logger = spdlog::stderr_logger_st(name);
  }
  return c;
}

clogger *cspd_rotating_logger_mt(const char *logger_name, const char *filename,
                                 size_t max_file_size, size_t max_files,
                                 bool force_flush) {
  clogger *c = alloc_clogger();
  if (c != NULL) {
    c->logger = spdlog::rotating_logger_mt(logger_name, filename, max_file_size,
                                           force_flush);
  }
  return c;
}
clogger *cspd_rotating_logger_st(const char *logger_name, const char *filename,
                                 size_t max_file_size, size_t max_files,
                                 bool force_flush) {
  clogger *c = alloc_clogger();
  if (c != NULL) {
    c->logger = spdlog::rotating_logger_mt(logger_name, filename, max_file_size,
                                           force_flush);
  }
  return c;
}
clogger *cspd_daily_logger_mt(const char *logger_name, const char *filename,
                              int hour, int minute, bool force_flush) {
  clogger *c = alloc_clogger();
  if (c != NULL) {
    c->logger = spdlog::daily_logger_mt(logger_name, filename, hour, minute,
                                        force_flush);
  }
  return c;
}
clogger *cspd_daily_logger_st(const char *logger_name, const char *filename,
                              int hour, int minute, bool force_flush) {
  clogger *c = alloc_clogger();
  if (c != NULL) {
    c->logger = spdlog::daily_logger_mt(logger_name, filename, hour, minute,
                                        force_flush);
  }
  return c;
}

clogger *cspd_simple_file_logger_st(const char *name, const char *file,
                                    bool force_flush) {
  clogger *c = alloc_clogger();
  if (c != NULL) {
    c->logger = spdlog::create<spdlog::sinks::simple_file_sink_st>(name, file,
                                                                   force_flush);
  }
  return c;
}

#if defined(__linux__) || defined(__APPLE__)
clogger *cspd_syslog_logger(const char *logger_name, const char *ident,
                            int syslog_option) {
  clogger *c = alloc_clogger();
  if (c != NULL) {
    c->logger = spdlog::syslog_logger(logger_name, ident, syslog_option);
  }
  return c;
}
#endif

static inline bool cspd_convert_level(clevel_enum level,
                                      spdlog::level::level_enum &level_spd) {
  level_spd = spdlog::level::off;
  switch (level) {
  case TRACE:
    level_spd = spdlog::level::trace;
    break;
  case DEBUG:
    level_spd = spdlog::level::debug;
    break;
  case INFO:
    level_spd = spdlog::level::info;
    break;
  case NOTICE:
    level_spd = spdlog::level::notice;
    break;
  case WARN:
    level_spd = spdlog::level::warn;
    break;
  case ERR:
    level_spd = spdlog::level::err;
    break;
  case CRITICAL:
    level_spd = spdlog::level::critical;
    break;
  case ALERT:
    level_spd = spdlog::level::alert;
    break;
  case EMERG:
    level_spd = spdlog::level::emerg;
    break;
  case OFF:
    level_spd = spdlog::level::off;
    break;

  default:
    return false;
  }
  return true;
}

bool cspd_set_level(clevel_enum level) {
  spdlog::level::level_enum level_spd;
  bool converted = cspd_convert_level(level, level_spd);
  if (converted) {
    spdlog::set_level(level_spd);
  }
  return converted;
}

void cspd_set_pattern(const char *pattern) { spdlog::set_pattern(pattern); }

bool cspd_set_async_mode(size_t queue_size,
                         const casync_overflow_policy overflow_policy) {
  spdlog::async_overflow_policy async_policy;
  switch (overflow_policy) {
  case BLOCK_RETRY:
    async_policy = spdlog::async_overflow_policy::block_retry;
    break;
  case DISCARD_LOG_MSG:
    async_policy = spdlog::async_overflow_policy::discard_log_msg;
    break;
  default:
    return false;
  }
  spdlog::set_async_mode(queue_size, async_policy);
  return true;
}

void cspd_set_sync_mode(void) { spdlog::set_sync_mode(); }

// by instance
void cspd_logger_set_pattern(clogger *c, const char *pattern) {
  c->logger->set_pattern(pattern);
}

bool cspd_logger_set_level(clogger *c, clevel_enum level) {
  spdlog::level::level_enum level_spd;
  bool converted = cspd_convert_level(level, level_spd);
  if (converted) {
    c->logger->set_level(level_spd);
  }
  return converted;
}

void cspd_logger_flush(clogger *c) { c->logger->flush(); }

// TODO: check performance retuning bool
static inline void cspd_printf(char **msg,
                               std::shared_ptr<spdlog::logger> logger,
                               spdlog::level::level_enum level, const char *std,
                               va_list args) {
  *msg = NULL;
  if (!logger->should_log(level)) {
    return;
  }
  int n = vasprintf(msg, std, args);
  if (n < 0) {
    *msg = NULL;
  }
}

void cspd_trace(clogger *c, const char *std, ...) {
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c->logger, spdlog::level::info, std, args);
  va_end(args);
  if (msg) {
    c->logger->trace() << msg;
    free(msg);
  }
}
void cspd_debug(clogger *c, const char *std, ...) {
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c->logger, spdlog::level::info, std, args);
  va_end(args);
  if (msg) {
    c->logger->debug() << msg;
    free(msg);
  }
}
void cspd_info(clogger *c, const char *std, ...) {
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c->logger, spdlog::level::info, std, args);
  va_end(args);
  if (msg) {
    c->logger->info() << msg;
    free(msg);
  }
}
void cspd_notice(clogger *c, const char *std, ...) {
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c->logger, spdlog::level::notice, std, args);
  va_end(args);
  if (msg) {
    c->logger->notice() << msg;
    free(msg);
  }
}
void cspd_warn(clogger *c, const char *std, ...) {
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c->logger, spdlog::level::warn, std, args);
  va_end(args);
  if (msg) {
    c->logger->warn() << msg;
    free(msg);
  }
}
void cspd_error(clogger *c, const char *std, ...) {
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c->logger, spdlog::level::err, std, args);
  va_end(args);
  if (msg) {
    c->logger->error() << msg;
    free(msg);
  }
}
void cspd_critical(clogger *c, const char *std, ...) {
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c->logger, spdlog::level::critical, std, args);
  va_end(args);
  if (msg) {
    c->logger->critical() << msg;
    free(msg);
  }
}
void cspd_alert(clogger *c, const char *std, ...) {
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c->logger, spdlog::level::alert, std, args);
  va_end(args);
  if (msg) {
    c->logger->alert() << msg;
    free(msg);
  }
}
void cspd_emerg(clogger *c, const char *std, ...) {
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c->logger, spdlog::level::emerg, std, args);
  va_end(args);
  if (msg) {
    c->logger->emerg() << msg;
    free(msg);
  }
}

void cspd_drop_all(void) {
  // TODO: think about how to free memory allocated in cspd
  spdlog::drop_all();
}
}
