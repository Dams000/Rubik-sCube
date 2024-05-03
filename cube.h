#ifndef CUBE_H
#define CUBE_H

#include "cublet.h"

#define SIZE 48

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
  Cubie*** cube;
} Cube;

Cube Cube_make(float cubletSize);

void Cube_free(Cube cube);

void Cube_drawCube(Cube *cube);

Rotation getCorrespondingRotation(char c);

void Cube_applyMoves(Cube *cube, char *moves);

void Cube_rotate(Cube *cube, Rotation rotation, int numberOfLayers);

#endif // !CUBE_H
