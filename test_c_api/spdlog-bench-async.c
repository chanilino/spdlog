//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#include "cspdlog.h"


int main(int argc, char*argv[])
{
    int howmany = 1000000;
    int i;
    ///Create a file rotating logger with 5mb size max and 3 rotated files
    size_t q_size = 1048576; //queue size must be power of 2
    cspd_set_async_mode(q_size, DISCARD_LOG_MSG);
    clogger* logger =  cspd_simple_file_logger_st("file_logger", "logs/spd-bench-st.txt", false);
    cspd_logger_set_pattern(logger, "[%Y-%b-%d %T.%e]: %v");
    for( i  = 0 ; i < howmany; ++i)
	cspd_info(logger, "spdlog message #%d: This is some text for your pleasure", i);

    cspd_logger_flush(logger);
    cspd_drop_all();
    return 0;
}
