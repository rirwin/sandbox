#include <time.h>
#include <stdlib.h>
#include <stdio.h>
 
int main(void)
{
  time_t t;
  char * t_str;
  t = 1387315390.882033;
  t_str = ctime(&t);
  printf("TIME: %s\n",t_str);
}
