#include <sys/time.h>

void getMillis(unsigned long long *t)
{
  int err;
  struct timeval tv;
  //struct timezone tz;

  err = gettimeofday(&tv, 0);
  *t = tv.tv_sec*1e3 + tv.tv_usec/1e3;
}
