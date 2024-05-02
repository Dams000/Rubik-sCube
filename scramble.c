#include "scramble.h"
#include "cube.h"
#include "include/raylib.h"
#include <stdio.h>
#include <string.h>

char *possibleMoves[] = {"R", "R'", "R2", "L", "L'", "L2", "U", "U'", "U2",
                         "D", "D'", "D2", "F", "F'", "F2", "B", "B'", "B2"};

bool areOppsitefaces(char c1, char c2) {
  return (c1 == 'R' && c2 == 'L') || (c1 == 'L' && c2 == 'R') ||
         (c1 == 'U' && c2 == 'D') || (c1 == 'D' && c2 == 'U') ||
         (c1 == 'F' && c2 == 'B') || (c1 == 'B' && c2 == 'F');
}

char **generateScramble(char *sequence[SCRAMBLE_SIZE]) {
  int sequenceLength = 0;

  while (sequenceLength < SCRAMBLE_SIZE) {
    int rand = GetRandomValue(0, 17);
    int n = GetRandomValue(1, SIZE / 2);
    char *currentMove = possibleMoves[rand];

    const char layers[] = "%dw%s";
    int size = snprintf(NULL, 0, layers, n, currentMove);
    char fullMove[size + 1];
    snprintf(fullMove, sizeof fullMove, layers, n, currentMove);

    if (sequenceLength < 1) {
      sequence[sequenceLength] = strdup(fullMove);
      sequenceLength++;
      continue;
    }

    char *lastMove = sequence[sequenceLength - 1];

    if (strcmp(fullMove, lastMove) == 0 || fullMove[2] == lastMove[2])
      continue;
    if (sequenceLength > 1) {
      char *secondtoLastMove = sequence[sequenceLength - 2];
      if (fullMove[2] == secondtoLastMove[2] &&
          areOppsitefaces(fullMove[2], lastMove[2]))
        continue;
    }

    sequence[sequenceLength] = strdup(fullMove);

    sequenceLength++;
  }

  for (int i = 0; i < sequenceLength; i++) {
    if (sequence[i][0] == '1') {
      int len = strlen(sequence[i]);
      for (int c = 2; c < len; c++) {
        printf("%c", sequence[i][c]);
      }
      printf(" ");
    } else
      printf("%s ", sequence[i]);
  }
  printf("\n");

  return sequence;
}
