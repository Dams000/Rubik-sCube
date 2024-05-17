#include "average.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b) {
  char *time1 = *(char **)a;
  char *time2 = *(char **)b;

  int min1 = (time1[0] - '0') * 10 + time1[1] - '0';
  int sec1 = (time1[3] - '0') * 10 + time1[4] - '0';
  int mills1 = (time1[6] - '0') * 100 + (time1[7] - '0') * 10 + time1[8] - '0';

  int min2 = (time2[0] - '0') * 10 + time2[1] - '0';
  int sec2 = (time2[3] - '0') * 10 + time2[4] - '0';
  int mills2 = (time2[6] - '0') * 100 + (time2[7] - '0') * 10 + time2[8] - '0';

  if (min1 != min2) {
    return min1 - min2;
  } else if (sec1 != sec2)
    return sec1 - sec2;
  else
    return mills1 - mills2;
}

void getAverageOf5(char times[5][12], int cubeSize) {
  char filename[20];
  snprintf(filename, 20, "times/%d.time", cubeSize);
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("fopen in average.c");
    exit(1);
  }

  int count = 0;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, fp)) != -1) {
    count++;
  }

  if (count < 5) {
    for (int i = 0; i < 5; i++)
      strcpy(times[i], "-");
    return;
  }

  char **sortedTimes = malloc(5 * sizeof(char *));
  if (sortedTimes == NULL) {
    perror("malloc");
    exit(1);
  }

  rewind(fp);

  int startingCount = count - 5;
  count = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
    if (count < startingCount) {
      count++;
      continue;
    }

    line[strcspn(line, "\n")] = '\0';

    sortedTimes[count - startingCount] =
        malloc((strlen(line) + 1) * sizeof(char));
    if (sortedTimes[count - startingCount] == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
    snprintf(times[count - startingCount], 12, "%s", line);
    strcpy(sortedTimes[count - startingCount], line);
    count++;
  }

  qsort(sortedTimes, count - startingCount, sizeof(char *), compare);

  for (int i = 0; i < count - startingCount; i++) {
    if (strcmp(times[i], sortedTimes[0]) == 0)
      snprintf(times[i], 15, "(%s)", sortedTimes[0]);
    else if (strcmp(times[i], sortedTimes[4]) == 0)
      snprintf(times[i], 15, "(%s)", sortedTimes[4]);
  }

  for (int i = 0; i < count - startingCount; i++) {
    free(sortedTimes[i]);
  }
  free(sortedTimes);

  free(line);
  fclose(fp);

  return;
}
