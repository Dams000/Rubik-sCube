#include "utils.h"

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
