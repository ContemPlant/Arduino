#include "fff.h"
#include <stdio.h>
#include <stdint.h>
#include "timestamp.h"

int main(){
  int seconds = seconds_from_date(0,0,0,1,1,1970);
  printf("Sekunden 1970 - 1970 : %d\n", seconds);
  printf("Erwartet:              0\n");
  seconds = seconds_from_date(0,0,0,1,1,1971);
  printf("Sekunden 1970 - 1971 : %d\n", seconds);
  printf("Erwartet:              31536000\n");
  seconds = seconds_from_date(1,0,0,1,1,1971);
  printf("Sekunden 1970 - 1971 + eine Sekunde : %d\n", seconds);
  printf("Erwartet:                             31536001\n");
  seconds = seconds_from_date(1,3,0,1,1,1971);
  printf("Sekunden 1970 - 1971 + eine Sekunde + 3 Minuten : %d\n", seconds);
  printf("Erwartet:                                         31536181\n");
  seconds = seconds_from_date(0,0,2,1,1,1971);
  printf("Sekunden 1970 - 1971 + zwei Stunden : %d\n", seconds);
  printf("Erwartet:                             31543200\n");
  return 0;
}