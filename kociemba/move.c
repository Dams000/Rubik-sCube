#include "move.h"

Move Move_createMove(Colors orientation, Direction direction) {
  Move move;
  move.orientation = orientation;
  move.direction = direction;
  return move;
}
