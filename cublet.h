#ifndef CUBLET_H
#define CUBLET_H

#include "include/raylib.h"
#include "include/raymath.h"

typedef struct Cubie {
  Vector3 position;
  Color colors[6];
  float sideLength;
} Cubie;

Cubie Cubie_make(int x, int y, int z, float sideLength);

void Cubie_setCoordinates(Cubie *cubie, float x, float y, float z);

void Cubie_rotateRight(Cubie *cubie);

void Cubie_rotateLeft(Cubie *cubie);

void Cubie_rotateUp(Cubie *cubie);

void Cubie_rotateDown(Cubie *cubie);

void Cubie_rotateClockWise(Cubie *cubie);

void Cubie_rotateAntiClockWise(Cubie *cubie);

void Cubie_drawCubie(Cubie *cubie, Vector3 position);

#endif // !CUBLET_H
