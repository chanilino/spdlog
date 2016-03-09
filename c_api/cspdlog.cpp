#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdlib.h>
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

clogger *cpsd_create_logger_with_sinks(const char* logger_name, ...){
//TODO: see tuple cat
	//	va_list args;
//  	va_start(args, logger_name);
//	auto t;
//	int i = 3;
//	std::tuple<spdlog::sinks::sink> t0(sink);
//	t = t0;
//	for(i; i < 3; i++){
//		auto t =  at(t, ti);
//	}
//	va_end(args);
	return NULL;
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
  case CSPD_TRACE:
    level_spd = spdlog::level::trace;
    break;
  case CSPD_DEBUG:
    level_spd = spdlog::level::debug;
    break;
  case CSPD_INFO:
    level_spd = spdlog::level::info;
    break;
  case CSPD_NOTICE:
    level_spd = spdlog::level::notice;
    break;
  case CSPD_WARN:
    level_spd = spdlog::level::warn;
    break;
  case CSPD_ERR:
    level_spd = spdlog::level::err;
    break;
  case CSPD_CRITICAL:
    level_spd = spdlog::level::critical;
    break;
  case CSPD_ALERT:
    level_spd = spdlog::level::alert;
    break;
  case CSPD_EMERG:
    level_spd = spdlog::level::emerg;
    break;
  case CSPD_OFF:
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

bool cspd_logger_should_log(clogger* c, clevel_enum level){
	spdlog::logger* logger= (spdlog::logger*)c;
  	spdlog::level::level_enum level_spd;
	if(!cspd_convert_level(level, level_spd)){
		return false;
	}
	return logger->should_log(level_spd);
}

void _cspd_trace(clogger *c, const char *msg){
	((spdlog::logger*)c)->trace() << msg;
}

void _cspd_debug(clogger *c, const char *msg){
	((spdlog::logger*)c)->debug() << msg;
}

void _cspd_info(clogger* c, const char* msg){
	((spdlog::logger*)c)->info() << msg;
}

void _cspd_notice(clogger *c, const char *msg){
	((spdlog::logger*)c)->notice() << msg;
}

void _cspd_warn(clogger *c, const char *msg){
	((spdlog::logger*)c)->warn() << msg;
}

void _cspd_error(clogger *c, const char *msg){
	((spdlog::logger*)c)->error() << msg;
}

void _cspd_critical(clogger *c, const char *msg){
	((spdlog::logger*)c)->critical() << msg;
}

void _cspd_alert(clogger *c, const char *msg){
	((spdlog::logger*)c)->alert() << msg;
}

void _cspd_emerg(clogger *c, const char *msg){
	((spdlog::logger*)c)->emerg() << msg;
}



void cspd_drop_all(void) {
  // TODO: think about how to free memory allocated in cspd
  spdlog::drop_all();
  _sinks.clear();
}
}
