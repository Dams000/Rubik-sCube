#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

bool colorsEqual(Color color1, Color color2) {
  return color1.a == color2.a && color1.r == color2.r && color1.g == color2.g &&
         color1.b == color2.b;
}

int Cnk(int n, int k) {
  int i, j, s;
  if (n < k)
    return 0;
  if (k > n / 2)
    k = n - k;
  for (s = 1, i = n, j = 1; i != n - k; i--, j++) {
    s *= i;
    s /= j;
  }
  return s;
}

void storeTime(char *time, int size) {
  char filename[15];
  snprintf(filename, 15, "times/%d.time", size);

  FILE *file = fopen(filename, "a+");
  if (file == NULL) {
    perror("fopen");
    exit(1);
  }

  fseek(file, 0, SEEK_SET);

  fputs(time, file);
  fputc('\n', file);

  fclose(file);
}
