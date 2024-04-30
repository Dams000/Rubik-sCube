#ifndef CUBE_H
#define CUBE_H

#include "cublet.h"

#define SIZE 3

typedef enum Rotations {
  U,
  u,
  D,
  d,
  R,
  r,
  L,
  l,
  F,
  f,
  B,
  b,
  M,
  m,
  E,
  e,
  S,
  s,
  X,
  x,
  Y,
  y,
  Z,
  z
} Rotation;

typedef struct Cube {
  Cubie cube[SIZE][SIZE][SIZE];
} Cube;

Cube Cube_make(float cubletSize);

Rotation getCorrespondingRotation(char c);

void Cube_rotate(Cube *cube, Rotation rotation);

#endif // !CUBE_H