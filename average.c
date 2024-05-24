#include "average.h"
#include "utils.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b) {
  char *time1 = *(char **)a;
  char *time2 = *(char **)b;
  return strcmp(time1, time2);
}

bool timeIsDNF(char time[20]) { return time[0] == 'D'; }

void getTimes(char times[5][20], int cubeSize) {
  char filename[20];
  getFileName(filename, cubeSize);
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
    snprintf(times[x], 20, "%s", line);
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
      if (!timeIsDNF(times[i]) && !found &&
          strcmp(times[i], sortedTimes[0]) == 0) {
        snprintf(times[i], 20, "(%s)", sortedTimes[0]);
        found = true;
      }
    found = false;
    for (int i = 0; i < x; i++)
      if (!timeIsDNF(times[i]) && !found &&
          strcmp(times[i], sortedTimes[4]) == 0) {
        snprintf(times[i], 20, "(%s)", sortedTimes[4]);
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

void getAverageOf5(char times[5][20], char avg[10]) {
  int dnfCount = 0;
  for (int i = 0; i < 5; i++) {
    if (timeIsDNF(times[i]))
      dnfCount++;
    if (dnfCount > 1) {
      strcpy(avg, "DNF");
      return;
    }
    if (times[i][0] == '-') {
      avg[0] = '-';
      avg[1] = '\0';
      return;
    }
  }

  char validTimes[3][20];
  int x = 0;
  for (int i = 0; i < 5; i++) {
    if (times[i][0] == '(' || timeIsDNF(times[i]))
      continue;
    strcpy(validTimes[x++], times[i]);
  }
  int millisecondsTotal = 0;
  for (int i = 0; i < 3; i++) {
    millisecondsTotal += timeToMillis(validTimes[i]);
  }
  millisecondsTotal /= 3;
  int minutes = getMinutesFromMillis(millisecondsTotal);
  int seconds = getSecondsFromMillis(millisecondsTotal);
  int milliseconds = getMillisFromMillis(millisecondsTotal);
  snprintf(avg, 17, "%02d:%02d.%03d", minutes, seconds, milliseconds);
}

void setDNF(int index, int cubeSize) {
  char filename[20];
  getFileName(filename, cubeSize);

  FILE *fp = fopen(filename, "r");
  int lineNumber = countLines(fp);
  fclose(fp);

  int targetLine = lineNumber < 5 ? index + 1 : lineNumber - 4 + index;

  char command[64];
  snprintf(command, 64, "sed -i '%d {/^D/! s/.*/DNF(&)/}' times/%d.time",
           targetLine, cubeSize);

  system(command);
}
void setPlusTwo(int index, int cubeSize) {
  char filename[20], command[86], path[1024];
  getFileName(filename, cubeSize);

  FILE *fp = fopen(filename, "r");
  int lineNumber = countLines(fp);
  fclose(fp);

  int targetLine = lineNumber < 5 ? index + 1 : lineNumber - 4 + index;
  snprintf(command, sizeof(command), "sed -n '%dp' %s", targetLine, filename);

  fp = popen(command, "r");
  if (fp == NULL) {
    printf("Failed to run command\n");
    exit(1);
  }

  if (fgets(path, sizeof(path), fp) != NULL) {
    int time = timeToMillis(path) + 2000;
    int minutes = getMinutesFromMillis(time);
    int seconds = getSecondsFromMillis(time);
    int milliseconds = getMillisFromMillis(time);

    char newTime[20];
    snprintf(newTime, sizeof(newTime), "%02d:%02d.%03d+", minutes, seconds,
             milliseconds);

    snprintf(command, sizeof(command), "sed -i '%ds/.*/%s/' %s", targetLine,
             newTime, filename);
    system(command);
  } else
    printf("No output from command\n");

  pclose(fp);
}
