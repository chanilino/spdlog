#include "cspdlog.h"

int main(void){
	clogger* sout = create_stdout_logger_mt("stdout");
	clog_info(sout, "Hola %s", "WORLD");
        cspdlog_drop_all();
	return 0;
}
