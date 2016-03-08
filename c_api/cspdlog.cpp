#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>

#include "spdlog/spdlog.h"
#include "cspdlog.h"
    
static std::unordered_map <void*, std::shared_ptr<spdlog::sinks::sink>> _sinks;

extern "C" {

clogger *cspd_get(const char *name) {
    return spdlog::get(name).get();
}

clogger *cspd_stdout_logger_mt(const char *name) {
   return  spdlog::stdout_logger_mt(name).get();
}

clogger *cspd_stdout_logger_st(const char *name) {
    return spdlog::stdout_logger_st(name).get();
}

clogger *cspd_stderr_logger_mt(const char *name) {
    return  spdlog::stderr_logger_mt(name).get();
}

clogger *cspd_stderr_logger_st(const char *name) {
    return spdlog::stderr_logger_st(name).get();
}

clogger *cspd_rotating_logger_mt(const char *logger_name, const char *filename,
                                 size_t max_file_size, size_t max_files,
                                 bool force_flush) {
    return spdlog::rotating_logger_mt(logger_name, filename, max_file_size,
                                           force_flush).get();
}
clogger *cspd_rotating_logger_st(const char *logger_name, const char *filename,
                                 size_t max_file_size, size_t max_files,
                                 bool force_flush) {
    return spdlog::rotating_logger_mt(logger_name, filename, max_file_size,
                                           force_flush).get();
}
clogger *cspd_daily_logger_mt(const char *logger_name, const char *filename,
                              int hour, int minute, bool force_flush) {
    return spdlog::daily_logger_mt(logger_name, filename, hour, minute,
                                        force_flush).get();
}
clogger *cspd_daily_logger_st(const char *logger_name, const char *filename,
                              int hour, int minute, bool force_flush) {
    return spdlog::daily_logger_mt(logger_name, filename, hour, minute,
                                        force_flush).get();
}

clogger *cspd_simple_file_logger_st(const char *name, const char *file,
                                    bool force_flush) {
    return spdlog::create<spdlog::sinks::simple_file_sink_st>(name, file,
                                                                   force_flush).get();
}

#if defined(__linux__) || defined(__APPLE__)
clogger *cspd_syslog_logger(const char *logger_name, const char *ident,
                            int syslog_option) {
    return spdlog::syslog_logger(logger_name, ident, syslog_option).get();
}
#endif
clogger *cpsd_create_logger_with_sink(const char* logger_name, csink * sink){
    auto found = _sinks.find(sink);
    auto _sink = found == _sinks.end() ? nullptr : found->second;
	assert(_sink != nullptr);
	auto logger  = std::make_shared<spdlog::logger>(logger_name, _sink);
	spdlog::register_logger(logger);
	return logger.get();
}

//We need support of hidden logger to maintain csink out

//Sinks
csink *cspd_rotating_file_sink_mt( const char *filename, const char* extension,
		size_t max_file_size, size_t max_files,
		bool force_flush){
	auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename,extension, max_file_size ,max_files,force_flush);
	_sinks[sink.get()] = sink;
	return sink.get();
}

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
void cspd_logger_set_pattern(clogger *clog, const char *pattern) {
  spdlog::logger* c= (spdlog::logger*) clog;
  c->set_pattern(pattern);
}

bool cspd_logger_set_level(clogger *clog, clevel_enum level) {
  spdlog::logger* c= (spdlog::logger*) clog;
  spdlog::level::level_enum level_spd;
  bool converted = cspd_convert_level(level, level_spd);
  if (converted) {
    c->set_level(level_spd);
  }
  return converted;
}

void cspd_logger_flush(clogger *clog) {
  spdlog::logger* c= (spdlog::logger*) clog;
       	c->flush(); 
}
void cspd_logger_drop(const char* name){
	spdlog::drop(name);
}

// TODO: check performance retuning bool
static inline void cspd_printf(char **msg,
                               spdlog::logger* logger,
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

void cspd_trace(clogger *clog, const char *std, ...) {
  spdlog::logger* c= (spdlog::logger*) clog;
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c, spdlog::level::info, std, args);
  va_end(args);
  if (msg) {
    c->trace() << msg;
    free(msg);
  }
}
void cspd_debug(clogger *clog, const char *std, ...) {
  spdlog::logger* c= (spdlog::logger*) clog;
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c, spdlog::level::info, std, args);
  va_end(args);
  if (msg) {
    c->debug() << msg;
    free(msg);
  }
}
void cspd_info(clogger *clog, const char *std, ...) {
  spdlog::logger* c= (spdlog::logger*) clog;
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c, spdlog::level::info, std, args);
  va_end(args);
  if (msg) {
    c->info() << msg;
    free(msg);
  }
}

void cspd_notice(clogger *clog, const char *std, ...) {
  spdlog::logger* c= (spdlog::logger*) clog;
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c, spdlog::level::notice, std, args);
  va_end(args);
  if (msg) {
    c->notice() << msg;
    free(msg);
  }
}
void cspd_warn(clogger *clog, const char *std, ...) {
  spdlog::logger* c= (spdlog::logger*) clog;
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c, spdlog::level::warn, std, args);
  va_end(args);
  if (msg) {
    c->warn() << msg;
    free(msg);
  }
}
void cspd_error(clogger *clog, const char *std, ...) {
  spdlog::logger* c= (spdlog::logger*) clog;
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c, spdlog::level::err, std, args);
  va_end(args);
  if (msg) {
    c->error() << msg;
    free(msg);
  }
}
void cspd_critical(clogger *clog, const char *std, ...) {
  spdlog::logger* c= (spdlog::logger*) clog;
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c, spdlog::level::critical, std, args);
  va_end(args);
  if (msg) {
    c->critical() << msg;
    free(msg);
  }
}
void cspd_alert(clogger *clog, const char *std, ...) {
  spdlog::logger* c= (spdlog::logger*) clog;
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c, spdlog::level::alert, std, args);
  va_end(args);
  if (msg) {
    c->alert() << msg;
    free(msg);
  }
}
void cspd_emerg(clogger *clog, const char *std, ...) {
  spdlog::logger* c= (spdlog::logger*) clog;
  va_list args;
  char *msg;
  va_start(args, std);
  cspd_printf(&msg, c, spdlog::level::emerg, std, args);
  va_end(args);
  if (msg) {
    c->emerg() << msg;
    free(msg);
  }
}

void cspd_drop_all(void) {
  // TODO: think about how to free memory allocated in cspd
  spdlog::drop_all();
  _sinks.clear();
}
}
