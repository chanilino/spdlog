#include "cspdlog.h"

int main(void){
	int i;
	clogger* sout = create_stdout_logger_mt("stdout");
	clog_info(sout, "Hola %s", "WORLD");
        cspd_set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");
        for (i = 0; i < 10; ++i)
		clog_info(sout, "Hola %s: %d", "WORLD", i);
        cspdlog_drop_all();
	return 0;
}
