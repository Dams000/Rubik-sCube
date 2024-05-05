#ifndef TIMER_H
#define TIMER_H

#include <pthread.h>
#include <stdbool.h>
#include <time.h>

typedef struct Timer {
  int minutes, seconds, milliseconds;
  struct timespec startTime;
  bool isRunning, justStopped;
  pthread_t thread;
} Timer;

Timer Timer_make();

void Timer_start(Timer *timer);

void Timer_update(Timer *timer);

void Timer_stop(Timer *timer);

#endif // !TIMER_H
