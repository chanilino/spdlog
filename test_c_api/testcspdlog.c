#include "cspdlog.h"

int main(void) {
  int i;
  clogger *sout = cspd_stdout_logger_mt("stdout");
  cspd_info(sout, "Hola %s", "WORLD");
  cspd_set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");
  for (i = 0; i < 10; ++i)
    cspd_info(sout, "Hola %s: %d", "WORLD", i);
  cspd_drop_all();
  return 0;
}
