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
    clogger* logger =  create_simple_file_logger_st("file_logger", "logs/spd-bench-st.txt", false);
    c_logger_set_pattern(logger, "[%Y-%b-%d %T.%e]: %v");
    for( i  = 0 ; i < howmany; ++i)
	clog_info(logger, "spdlog message #%d: This is some text for your pleasure", i);

    return 0;
}
