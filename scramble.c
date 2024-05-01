#include "scramble.h"
// #include "cube.h"
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
    char *currentMove = possibleMoves[rand];

    if (sequenceLength < 1) {
      sequence[sequenceLength] = currentMove;
      sequenceLength++;
      continue;
    }

    char *lastMove = sequence[sequenceLength - 1];

    if (strcmp(currentMove, lastMove) == 0 || currentMove[0] == lastMove[0])
      continue;
    if (sequenceLength > 1) {
      char *secondtoLastMove = sequence[sequenceLength - 2];
      if (currentMove[0] == secondtoLastMove[0] &&
          areOppsitefaces(currentMove[0], lastMove[0]))
        continue;
    }

    sequence[sequenceLength] = currentMove;
    sequenceLength++;
  }

  for (int i = 0; i < sequenceLength; i++)
    printf("%s ", sequence[i]);
  printf("\n");

  return sequence;
}
