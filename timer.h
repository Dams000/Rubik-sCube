#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <stdbool.h>

typedef struct Timer {
  int minutes, seconds, milliseconds;
  struct timespec startTime;
  bool isRunning;
} Timer;

Timer Timer_make();

void Timer_start(Timer *timer);

void Timer_update(Timer *timer);

void Timer_stop(Timer *timer);

#endif // !TIMER_H
