//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
//
// spdlog usage example
//
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifdef __linux__
#include <syslog.h>
#endif
#include "cspdlog.h"

int main(int argc, char *argv[]) {
  int i;
  // Create console, multithreaded logger
  clogger *console = cspd_stdout_logger_mt("console");
  cspd_info(console, "Welcome to spdlog!");
  cspd_info(console, "An info message example %d..", 1);

  //
  // Runtime log levels
  //
  cspd_set_level(CSPD_INFO); // Set global log level to info
  cspd_debug(console, "This message shold not be displayed!");
  cspd_logger_set_level(console, CSPD_DEBUG); // Set specific logger's log level
  cspd_debug(console, "Now it should..");

  //
  // Create a file rotating logger with 5mb size max and 3 rotated files
  //
  clogger *file_logger = cspd_rotating_logger_mt(
      "file_logger", "logs/mylogfile", 1048576 * 5, 3, false);
  for (i = 0; i < 10; ++i)
    cspd_info(file_logger, "%d * %d equals %-10d", i, i, i * i);

  //
  // Create a daily logger - a new file is created every day on 2:30am
  //
  clogger *daily_logger =
      cspd_daily_logger_mt("daily_logger", "logs/daily", 2, 30, false);
  cspd_info(daily_logger, "Daily logger hello!!!");

  //
  // Customize msg format for all messages
  //
  cspd_set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");
  cspd_info(file_logger, "This is another message with custom format");

  // TODO:
  //        spd::get("console")->info("loggers can be retrieved from a global
  //        registry using the spdlog::get(logger_name) function");
  clogger *get_console = cspd_get("console");
  cspd_info(get_console, "loggers can be retrieved from a global registry "
                         "using the cspd_get(logger_name) function");
  //
  // Compile time debug or trace macros.
  // Enabled #ifdef SPDLOG_CSPD_DEBUG_ON or #ifdef SPDLOG_CSPD_TRACE_ON
  //
  // SPDLOG_CSPD_TRACE(console, "Enabled only #ifdef SPDLOG_CSPD_TRACE_ON..{} ,{}", 1,
  // 3.23);
  // SPDLOG_CSPD_DEBUG(console, "Enabled only #ifdef SPDLOG_CSPD_DEBUG_ON.. {} ,{}", 1,
  // 3.23);

  //
  // Asynchronous logging is very fast..
  // Just call spdlog::set_async_mode(q_size) and all created loggers from now
  // on will be asynchronous..
  //
  size_t q_size = 1048576; // queue size must be power of 2
  cspd_set_async_mode(q_size, DISCARD_LOG_MSG);
  clogger *async_file = cspd_daily_logger_st("async_file_logger",
                                             "logs/async_log.txt", 0, 0, false);
  for (i = 0; i < 100; ++i)
    cspd_info(async_file, "Async message #%d", i);

  csink * sink_rotate = cspd_rotating_file_sink_mt("logs/async_sink", "txt", 1024*16, 2, false);
  clogger * log1 = cpsd_create_logger_with_sink("log1", sink_rotate);
  clogger * log2 = cpsd_create_logger_with_sink("log2", sink_rotate);
  for (i = 0; i < 100; ++i){
    	cspd_info(log1, "Async message #%d", i);
    	cspd_info(log2, "Async message #%d", i);
  		cspd_info(get_console, "Async message #%d", i);
  }

//
// syslog example. linux only..
//
#ifdef __linux__
  const char *ident = "cspdlog-example";
  clogger *syslog_logger = cspd_syslog_logger("syslog", ident, LOG_PID);
  cspd_warn(syslog_logger,
            "This is warning that will end up in syslog. This is Linux only!");
#endif

  //
  // Release and close all loggers
  //
  cspd_drop_all();

  return 0;
}

//// Example of user defined class with operator<<
// class some_class {};
// std::ostream& operator<<(std::ostream& os, const some_class&)
//{
//    return os << "some_class";
//}
//
// void custom_class_example()
//{
//    some_class c;
//    spdlog::get("console")->info("custom class with operator<<: {}..", c);
//    spdlog::get("console")->info() << "custom class with operator<<: " << c <<
//    "..";
//}
