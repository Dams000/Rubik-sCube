#ifndef CUBE_H
#define CUBE_H

#include "cublet.h"

#define SIZE 3

typedef enum Rotations { U, u, D, d, R, r, L, l, F, f, B, b, M, m, E, e, S, s} Rotation;

typedef enum CubePositions {
  BACK_BOTTOM_LEFT = 0,
  BACK_BOTTOM_MIDDLE,
  BACK_BOTTOM_RIGHT,
  BACK_MIDDLE_LEFT,
  BACK_MIDDLE_MIDDLE,
  BACK_MIDDLE_RIGHT,
  BACK_TOP_LEFT,
  BACK_TOP_MIDDLE,
  BACK_TOP_RIGHT,
  MIDDLE_BOTTOM_LEFT,
  MIDDLE_BOTTOM_MIDDLE,
  MIDDLE_BOTTOM_RIGHT,
  MIDDLE_MIDDLE_LEFT,
  MIDDLE_MIDDLE_MIDDLE,
  MIDDLE_MIDDLE_RIGHT,
  MIDDLE_TOP_LEFT,
  MIDDLE_TOP_MIDDLE,
  MIDDLE_TOP_RIGHT,
  FRONT_BOTTOM_LEFT,
  FRONT_BOTTOM_MIDDLE,
  FRONT_BOTTOM_RIGHT,
  FRONT_MIDDLE_LEFT,
  FRONT_MIDDLE_MIDDLE,
  FRONT_MIDDLE_RIGHT,
  FRONT_TOP_LEFT,
  FRONT_TOP_MIDDLE,
  FRONT_TOP_RIGHT
} CubePosition;

typedef struct Cube {
  Cubie cube[SIZE][SIZE][SIZE];
} Cube;

Cube Cube_make();

void Cube_rotate(Cube *cube, Rotation rotation);

#endif // !CUBE_H
