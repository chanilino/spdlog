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
	static inline void clog_write(char **msg, std::shared_ptr<spdlog::logger> logger, spdlog::level::level_enum level,  const char* std, va_list args){
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
		}
	}

	void clog_debug(clogger* c, const char * std, ... ){
        	c->logger->debug("This is another message with custom format");
	}

	void cspd_set_level(clevel_enum level){
		throw  std::logic_error("Not implemented");
	}

	clogger* cspd_rotating_logger_mt(const char* logger_name, const char* filename, size_t max_file_size, size_t max_files, bool force_flush ){
		throw  std::logic_error("Not implemented");
	}

	void cspd_set_pattern(const char* pattern){
		throw  std::logic_error("Not implemented");
	}

	void cspd_set_async_mode(size_t queue_size, const casync_overflow_policy overflow_policy){
		throw  std::logic_error("Not implemented");
	}

	void cspdlog_set_async_mode(size_t q_size){
		throw  std::logic_error("Not implemented");
	}

	void cspdlog_drop_all(void){
		//TODO: think about how to free memory allocated in create
        	spdlog::drop_all();
	}

}
