#include "timer.h"
#include <stdbool.h>
#include <time.h>

Timer Timer_make() {
  return (Timer){.minutes = 0,
                 .seconds = 0,
                 .milliseconds = 0,
                 .startTime = {0},
                 .isRunning = false};
}

void Timer_start(Timer *timer) {
  clock_gettime(CLOCK_MONOTONIC, &timer->startTime);
  timer->isRunning = true;
}

void Timer_update(Timer *timer) {
  if (!timer->isRunning)
    return;
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);

  long long elapsed_time_ns =
      (now.tv_sec - timer->startTime.tv_sec) * 1000000000LL +
      (now.tv_nsec - timer->startTime.tv_nsec);
  double elapsed_time_ms = (double)elapsed_time_ns / 1000000.0;

  timer->minutes = (int)elapsed_time_ms / 60000;
  timer->seconds = (int)elapsed_time_ms % 60000 / 1000;
  timer->milliseconds = (int)elapsed_time_ms % 1000;
}

void Timer_stop(Timer *timer) { timer->isRunning = false; }
