#ifndef MOVE_H
#define MOVE_H

#include "enums.h"

typedef struct Move {
  Colors orientation;
  Direction direction;
} Move;

Move Move_createMove(Colors orientation, Direction direction);

#endif // !MOVE_H
