#include "average.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b) {
  char *time1 = *(char **)a;
  char *time2 = *(char **)b;
  return strcmp(time1, time2);
}

int countLines(FILE *fp) {
  int count = 0;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, fp)) != -1)
    count++;

  rewind(fp);
  return count;
}

void getTimes(char times[5][12], int cubeSize) {
  char filename[20];
  snprintf(filename, 20, "times/%d.time", cubeSize);
  FILE *fp = fopen(filename, "a+");
  if (fp == NULL) {
    perror("fopen in average.c");
    exit(1);
  }

  int count = countLines(fp);
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  char **sortedTimes = malloc(fmax(count, 5) * sizeof(char *));
  if (sortedTimes == NULL) {
    perror("malloc");
    free(line);
    fclose(fp);
    exit(1);
  }

  rewind(fp);

  int i = 0, x = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
    if (i < count - 5) {
      i++;
      continue;
    }

    line[strcspn(line, "\n")] = '\0';

    sortedTimes[x] = malloc((strlen(line) + 1) * sizeof(char));
    if (sortedTimes[x] == NULL) {
      perror("malloc");
      free(sortedTimes);
      free(line);
      fclose(fp);
      exit(1);
    }
    snprintf(times[x], 12, "%s", line);
    strcpy(sortedTimes[x], line);
    x++;
  }

  if (count < 5) {
    for (int i = count; i < 5; i++)
      strcpy(times[i], "-");
  } else {

    qsort(sortedTimes, x, sizeof(char *), compare);

    bool found = false;
    for (int i = 0; i < x; i++)
      if (!found && strcmp(times[i], sortedTimes[0]) == 0) {
        snprintf(times[i], 15, "(%s)", sortedTimes[0]);
        found = true;
      }
    found = false;
    for (int i = 0; i < x; i++)
      if (!found && strcmp(times[i], sortedTimes[4]) == 0) {
        snprintf(times[i], 15, "(%s)", sortedTimes[4]);
        found = true;
      }
  }

  for (int i = 0; i < x; i++) {
    free(sortedTimes[i]);
  }
  free(sortedTimes);

  free(line);
  fclose(fp);

  return;
}

int timeToSeconds(char time[10]) {
  return 600 * (time[0] - '0') + 60 * (time[1] - '0') + 10 * (time[3] - '0') +
         time[4] - '0';
}

int timeToMillis(char time[10]) {
  return 1000 * timeToSeconds(time) + 100 * (time[6] - '0') +
         10 * (time[7] - '0') + (time[8] - '0');
}

void getAverageOf5(char times[5][12], char avg[10]) {
  for (int i = 0; i < 5; i++)
    if (times[i][0] == '-')
      return;

  char validTimes[3][12];
  int x = 0;
  for (int i = 0; i < 5; i++) {
    if (times[i][0] == '(')
      continue;
    strcpy(validTimes[x++], times[i]);
  }
  int millisecondsTotal = 0;
  for (int i = 0; i < 3; i++) {
    millisecondsTotal += timeToMillis(validTimes[i]);
  }
  millisecondsTotal /= 3;
  int minutes = millisecondsTotal / (1000 * 60);
  int seconds = (millisecondsTotal / 1000) % 60;
  int milliseconds = millisecondsTotal % 1000;
  snprintf(avg, 17, "%02d:%02d.%03d", minutes, seconds, milliseconds);
}
