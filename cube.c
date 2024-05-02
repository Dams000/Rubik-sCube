#include "cube.h"
#include "cublet.h"
#include "include/raylib.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool isInnerCubie(float x, float y, float z) {
  return x != 0 && y != 0 && z != 0 && x != SIZE - 1 && y != SIZE - 1 &&
         z != SIZE - 1;
}

Cube Cube_make(float cubletSize) {
  Cube cube;
  for (int x = 0; x < SIZE; x++)
    for (int y = 0; y < SIZE; y++)
      for (int z = 0; z < SIZE; z++) {
        if (isInnerCubie(x, y, z))
          continue;
        cube.cube[x][y][z] = Cubie_make(x, y, z, cubletSize);
      }
  return cube;
}

Rotation getCorrespondingRotation(char c) {
  switch (c) {
  case 'U':
    return U;
  case 'u':
    return u;
  case 'D':
    return D;
  case 'd':
    return d;
  case 'R':
    return R;
  case 'r':
    return r;
  case 'L':
    return L;
  case 'l':
    return l;
  case 'F':
    return F;
  case 'f':
    return f;
  case 'B':
    return B;
  case 'b':
    return b;
  case 'M':
    return M;
  case 'm':
    return m;
  case 'E':
    return E;
  case 'e':
    return e;
  case 'S':
    return S;
  case 's':
    return s;
  default:
    return -1;
  }
}

void Cube_applyMoves(Cube *cube, char *move) {
  size_t len = strlen(move);
  char nbOfLayers = move[0] - '0';
  char face;
  if (move[len - 1] == '\'')
    face = tolower(move[len - 2]);
  else if (move[len - 1] == '2') {
    face = move[len - 2];
    Cube_rotate(cube, getCorrespondingRotation(face), nbOfLayers);
  } else
    face = move[len - 1];
  Cube_rotate(cube, getCorrespondingRotation(face), nbOfLayers);
}

void transposeMatrix(Cubie face[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++)
    for (int j = i + 1; j < SIZE; j++) {
      Cubie temp = face[i][j];
      face[i][j] = face[j][i];
      face[j][i] = temp;
    }
}

void reverseRows(Cubie face[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE / 2; j++) {
      Cubie temp = face[i][j];
      face[i][j] = face[i][SIZE - j - 1];
      face[i][SIZE - j - 1] = temp;
    }
}

void reverseColumns(Cubie face[SIZE][SIZE]) {
  for (int j = 0; j < SIZE; j++)
    for (int i = 0; i < SIZE / 2; i++) {
      Cubie temp = face[i][j];
      face[i][j] = face[SIZE - i - 1][j];
      face[SIZE - i - 1][j] = temp;
    }
}

/*----------------------------------------------------------------*/
// TODO: why is clockwise rotation equals to anti-clockwise on the cube ?
// depends on the side !
void rotate(Cube *cube, Vector3 dir, void (*cubieRotation)(Cubie *),
            bool antiClockwise) {
  Cubie face[SIZE][SIZE];
  int x, y, z;

  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++) {
      x = (dir.x == -1) ? i : dir.x;
      y = (dir.y == -1) ? (dir.x == -1) ? j : i : dir.y;
      z = (dir.z == -1) ? j : dir.z;
      cubieRotation(&cube->cube[x][y][z]);
      face[i][j] = cube->cube[x][y][z];
    }

  transposeMatrix(face);

  if (antiClockwise)
    reverseRows(face);
  else
    reverseColumns(face);

  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++) {
      x = (dir.x == -1) ? i : dir.x;
      y = (dir.y == -1) ? (dir.x == -1) ? j : i : dir.y;
      z = (dir.z == -1) ? j : dir.z;
      cube->cube[x][y][z] = face[i][j];
    }
}

void Cube_rotate(Cube *cube, Rotation rotation, int numberOfLayers) {
  switch (rotation) {
  case U: {
    for (int i = 0; i < numberOfLayers; i++)
      rotate(cube, (Vector3){-1, SIZE - i - 1, -1}, Cubie_rotateLeft,
             false);
    break;
  }
  case u: {
    for (int i = 0; i < numberOfLayers; i++)
      rotate(cube, (Vector3){-1, SIZE - i - 1, -1}, Cubie_rotateRight,
             true);
    break;
  }
  case D: {
    for (int i = 0; i < numberOfLayers; i++)
      rotate(cube, (Vector3){-1, i, -1}, Cubie_rotateRight, true);
    break;
  }
  case d: {
    for (int i = 0; i < numberOfLayers; i++)
      rotate(cube, (Vector3){-1, i, -1}, Cubie_rotateLeft, false);
    break;
  }
  case R: {
    for (int i = 0; i < numberOfLayers; i++)
      rotate(cube, (Vector3){SIZE - i - 1, -1, -1}, Cubie_rotateUp,
             true);
    break;
  }
  case r: {
    for (int i = 0; i < numberOfLayers; i++)
      rotate(cube, (Vector3){SIZE - i - 1, -1, -1}, Cubie_rotateDown,
             false);
    break;
  }
  case L: {
    for (int i = 0; i < numberOfLayers; i++)
      rotate(cube, (Vector3){i, -1, -1}, Cubie_rotateDown, false);
    break;
  }
  case l: {
    for (int i = 0; i < numberOfLayers; i++)
      rotate(cube, (Vector3){i, -1, -1}, Cubie_rotateUp, true);
    break;
  }
  case F: {
    for (int i = 0; i < numberOfLayers; i++)
      rotate(cube, (Vector3){-1, -1, SIZE - i - 1},
             Cubie_rotateClockWise, true);
    break;
  }
  case f: {
    for (int i = 0; i < numberOfLayers; i++)
      rotate(cube, (Vector3){-1, -1, SIZE - i - 1},
             Cubie_rotateAntiClockWise, false);
    break;
  }
  case B: {
    for (int i = 0; i < numberOfLayers; i++)
      rotate(cube, (Vector3){-1, -1, i}, Cubie_rotateAntiClockWise, false);
    break;
  }
  case b: {
    for (int i = 0; i < numberOfLayers; i++)
      rotate(cube, (Vector3){-1, -1, i}, Cubie_rotateClockWise, true);
    break;
  }
  case M: {
    for (int i = numberOfLayers; i < SIZE - numberOfLayers; i++)
      rotate(cube, (Vector3){i, -1, -1}, Cubie_rotateDown, false);
    break;
  }
  case m: {
    for (int i = numberOfLayers; i < SIZE - numberOfLayers; i++)
      rotate(cube, (Vector3){i, -1, -1}, Cubie_rotateUp, true);
    break;
  }
  case E: {
    for (int i = numberOfLayers; i < SIZE - numberOfLayers; i++)
      rotate(cube, (Vector3){-1, i, -1}, Cubie_rotateRight, true);
    break;
  }
  case e: {
    for (int i = numberOfLayers; i < SIZE - numberOfLayers; i++)
      rotate(cube, (Vector3){-1, i, -1}, Cubie_rotateLeft, false);
    break;
  }
  case S: {
    for (int i = numberOfLayers; i < SIZE - numberOfLayers; i++)
      rotate(cube, (Vector3){-1, -1, i}, Cubie_rotateClockWise, true);
    break;
  }
  case s: {
    for (int i = numberOfLayers; i < SIZE - numberOfLayers; i++)
      rotate(cube, (Vector3){-1, -1, i}, Cubie_rotateAntiClockWise, false);
    break;
  }
  case X: {
    Cube_rotate(cube, R, 1);
    Cube_rotate(cube, m, 1);
    Cube_rotate(cube, l, 1);
    break;
  }
  case x: {
    Cube_rotate(cube, r, 1);
    Cube_rotate(cube, M, 1);
    Cube_rotate(cube, L, 1);
    break;
  }
  case Y: {
    Cube_rotate(cube, U, 1);
    Cube_rotate(cube, e, 1);
    Cube_rotate(cube, d, 1);
    break;
  }
  case y: {
    Cube_rotate(cube, u, 1);
    Cube_rotate(cube, E, 1);
    Cube_rotate(cube, D, 1);
    break;
  }
  case Z: {
    Cube_rotate(cube, F, 1);
    Cube_rotate(cube, S, 1);
    Cube_rotate(cube, b, 1);
    break;
  }
  case z: {
    Cube_rotate(cube, f, 1);
    Cube_rotate(cube, s, 1);
    Cube_rotate(cube, B, 1);
    break;
  }
  }
}
