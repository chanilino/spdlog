#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>

#include "spdlog/spdlog.h"
#include "cspdlog.h"


struct _clogger{
	std::shared_ptr<spdlog::logger>	logger;
};
extern "C"
{
	static inline void clog_write(char **msg, std::shared_ptr<spdlog::logger> logger, 
			spdlog::level::level_enum level,  const char* std, va_list args){
		*msg = NULL;
		if(!logger->should_log(level)){
			return;
		}
		int n = vasprintf(msg, std , args);	
		if(n < 0){
			*msg = NULL;
		}
	}

	clogger* create_stdout_logger_mt(const char * name){
		clogger * c =  (clogger*)calloc(1, sizeof(*c));
		c->logger = spdlog::stdout_logger_mt(name);
		return c;

	}

	void clog_info(clogger* c, const char * std, ... ){
		va_list args;
		char * msg;
	  	va_start (args, std);
		clog_write(&msg, c->logger, spdlog::level::info,  std, args);
		va_end (args);
		if(msg){
	       		c->logger->info() << msg;
			free(msg);
		}
	}

	void clog_debug(clogger* c, const char * std, ... ){
		va_list args;
		char * msg;
	  	va_start (args, std);
		clog_write(&msg, c->logger, spdlog::level::info,  std, args);
		va_end (args);
		if(msg){
		       	c->logger->debug() << msg;
			free(msg);
		}
	}

	bool cspd_set_level(clevel_enum level){
		spdlog::level::level_enum  level_spd = spdlog::level::off;
		switch(level){
			case TRACE : level_spd = spdlog::level::trace;
						 break;
			case DEBUG : level_spd = spdlog::level::debug;
						 break;
			case INFO : level_spd = spdlog::level::info;
						break;
			case NOTICE : level_spd = spdlog::level::notice;
						  break;
			case WARN : level_spd = spdlog::level::warn;
						break;
			case ERR : level_spd = spdlog::level::err;
					   break;
			case CRITICAL :level_spd = spdlog::level::critical;
						   break;
			case ALERT : level_spd = spdlog::level::alert;
						 break;
			case EMERG : level_spd = spdlog::level::emerg;
						 break;
			case OFF : level_spd = spdlog::level::off;
					   break;

			default:
				 return false;
		}
		spdlog::set_level(level_spd);
		return true;
	}
	void cspd_set_pattern(const char* pattern){
		spdlog::set_pattern(pattern);
	}
      	void c_logger_set_pattern(clogger * c, const char* pattern){
		c->logger->set_pattern(pattern);	
	}

	clogger* cspd_rotating_logger_mt(const char* logger_name, const char* filename, 
			size_t max_file_size, size_t max_files, bool force_flush ){
		clogger * c =  (clogger*)calloc(1, sizeof(*c));
		c->logger = spdlog::rotating_logger_mt(logger_name, filename, max_file_size, force_flush);
		return c;
	}


	bool cspd_set_async_mode(size_t queue_size, const casync_overflow_policy overflow_policy){
		spdlog::async_overflow_policy async_policy;
		switch(overflow_policy){
			case BLOCK_RETRY: 
				async_policy = spdlog::async_overflow_policy::block_retry;
				 break;
			case DISCARD_LOG_MSG: 
				 async_policy =  spdlog::async_overflow_policy::discard_log_msg;
				 break;
			default:
				 return false;
		}
        spdlog::set_async_mode(queue_size, async_policy );
		return true;
	}

	clogger* create_simple_file_logger_st(const char * name, const char* file, bool force_flush){
		clogger * c =  (clogger*)calloc(1, sizeof(*c));
		c->logger = spdlog::create<spdlog::sinks::simple_file_sink_st>(name, file, force_flush);
		return c;
	}
	

	void cspd_drop_all(void){
		//TODO: think about how to free memory allocated in create
        	spdlog::drop_all();
	}

}
