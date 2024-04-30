#include "cube.h"
#include "cublet.h"
#include "include/raylib.h"
#include <ctype.h>
#include <string.h>

Cube Cube_make(float cubletSize) {
  Cube cube;
  for (int x = 0; x < SIZE; x++)
    for (int y = 0; y < SIZE; y++)
      for (int z = 0; z < SIZE; z++) {
        if (x != 0 && y != 0 && z != 0 && x != SIZE - 1 && y != SIZE - 1 &&
            z != SIZE - 1)
          continue;
        cube.cube[x][y][z] =
            Cubie_make(x - SIZE / 2, y - SIZE / 2, z - SIZE / 2, cubletSize);
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

void Cube_applyMoves(Cube *cube, char *moves) {
  for (size_t i = 0; i < strlen(moves); i++) {
    char curr = (moves[i] == '2') ? moves[i - 1] : moves[i];
    if (moves[i + 1] == '\'') {
      curr = tolower(curr);
      i++;
    }
    Cube_rotate(cube, getCorrespondingRotation(curr));
  }
}

/*----------------------------------------------------------------*/

void swap(Cube *cube, int i1, int i2, int i3, int i4) {
  Cubie tmp = cube->cube[i1 % 3][i1 / 3 % 3][i1 / 9];
  cube->cube[i1 % 3][i1 / 3 % 3][i1 / 9] =
      cube->cube[i2 % 3][i2 / 3 % 3][i2 / 9];

  cube->cube[i2 % 3][i2 / 3 % 3][i2 / 9] =
      cube->cube[i3 % 3][i3 / 3 % 3][i3 / 9];

  cube->cube[i3 % 3][i3 / 3 % 3][i3 / 9] =
      cube->cube[i4 % 3][i4 / 3 % 3][i4 / 9];

  cube->cube[i4 % 3][i4 / 3 % 3][i4 / 9] = tmp;
}

// TODO: why is clockwise rotation equals to anti-clockwise on the cube ?
// depends on the side !
void rotateAntiClockwise(Cube *cube, Vector3 dir,
                         void (*cubieRotation)(Cubie *)) {
  Cubie face[SIZE][SIZE];
  int x, y, z;

  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++) {
      // x = (dir.x == 0) ? i : (dir.x == -1) ? 0 : SIZE - 1;
      // y = (dir.y == 0) ? (dir.x == 0) ? j : i : (dir.y == -1) ? 0 : SIZE - 1;
      // z = (dir.z == -1) ? 0 : (dir.z == 1) ? SIZE - 1 : j;
      x = (dir.x == -1) ? i : dir.x;
      y = (dir.y == -1) ? (dir.x == -1) ? j : i : dir.y;
      z = (dir.z == -1) ? j : dir.z;
      cubieRotation(&cube->cube[x][y][z]);
      face[i][j] = cube->cube[x][y][z];
    }

  //  Transpose the matrix
  for (int i = 0; i < SIZE; i++)
    for (int j = i + 1; j < SIZE; j++) {
      Cubie temp = face[i][j];
      face[i][j] = face[j][i];
      face[j][i] = temp;
    }

  // Reverse each row
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE / 2; j++) {
      Cubie temp = face[i][j];
      face[i][j] = face[i][SIZE - j - 1];
      face[i][SIZE - j - 1] = temp;
    }

  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++) {
      // x = (dir.x == 0) ? i : (dir.x == -1) ? 0 : SIZE - 1;
      // y = (dir.y == 0) ? (dir.x == 0) ? j : i : (dir.y == -1) ? 0 : SIZE - 1;
      // z = (dir.z == -1) ? 0 : (dir.z == 1) ? SIZE - 1 : j;
      x = (dir.x == -1) ? i : dir.x;
      y = (dir.y == -1) ? (dir.x == -1) ? j : i : dir.y;
      z = (dir.z == -1) ? j : dir.z;
      cube->cube[x][y][z] = face[i][j];
    }
}

void rotateClockwise(Cube *cube, Vector3 dir, void (*cubieRotation)(Cubie *)) {

  Cubie face[SIZE][SIZE];
  int x, y, z;

  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++) {
      // x = (dir.x == 0) ? i : (dir.x == -1) ? 0 : SIZE - 1;
      // y = (dir.y == 0) ? (dir.x == 0) ? j : i : (dir.y == -1) ? 0 : SIZE - 1;
      // z = (dir.z == -1) ? 0 : (dir.z == 1) ? SIZE - 1 : j;
      x = (dir.x == -1) ? i : dir.x;
      y = (dir.y == -1) ? (dir.x == -1) ? j : i : dir.y;
      z = (dir.z == -1) ? j : dir.z;
      cubieRotation(&cube->cube[x][y][z]);
      face[i][j] = cube->cube[x][y][z];
    }

  //  Transpose the matrix
  for (int i = 0; i < SIZE; i++)
    for (int j = i + 1; j < SIZE; j++) {
      Cubie temp = face[i][j];
      face[i][j] = face[j][i];
      face[j][i] = temp;
    }

  // Reverse each row
  for (int j = 0; j < SIZE; j++)
    for (int i = 0; i < SIZE / 2; i++) {
      Cubie temp = face[i][j];
      face[i][j] = face[SIZE - i - 1][j];
      face[SIZE - i - 1][j] = temp;
    }

  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++) {
      // x = (dir.x == 0) ? i : (dir.x == -1) ? 0 : SIZE - 1;
      // y = (dir.y == 0) ? (dir.x == 0) ? j : i : (dir.y == -1) ? 0 : SIZE - 1;
      // z = (dir.z == -1) ? 0 : (dir.z == 1) ? SIZE - 1 : j;
      x = (dir.x == -1) ? i : dir.x;
      y = (dir.y == -1) ? (dir.x == -1) ? j : i : dir.y;
      z = (dir.z == -1) ? j : dir.z;
      cube->cube[x][y][z] = face[i][j];
    }
}

void Cube_rotate(Cube *cube, Rotation rotation) {
  switch (rotation) {
  case U: {
    rotateClockwise(cube, (Vector3){-1, SIZE - 1, -1}, Cubie_rotateLeft);
    break;
  }
  case u: {
    rotateAntiClockwise(cube, (Vector3){-1, SIZE - 1, -1}, Cubie_rotateRight);
    break;
  }
  case D: {
    rotateAntiClockwise(cube, (Vector3){-1, 0, -1}, Cubie_rotateRight);
    break;
  }
  case d: {
    rotateClockwise(cube, (Vector3){-1, 0, -1}, Cubie_rotateLeft);
    break;
  }
  case R: {
    rotateAntiClockwise(cube, (Vector3){SIZE - 1, -1, -1}, Cubie_rotateUp);
    break;
  }
  case r: {
    rotateClockwise(cube, (Vector3){SIZE - 1, -1, -1}, Cubie_rotateDown);
    break;
  }
  case L: {
    rotateClockwise(cube, (Vector3){0, -1, -1}, Cubie_rotateDown);
    break;
  }
  case l: {
    rotateAntiClockwise(cube, (Vector3){0, -1, -1}, Cubie_rotateUp);
    break;
  }
  case F: {
    rotateAntiClockwise(cube, (Vector3){-1, -1, SIZE - 1},
                        Cubie_rotateClockWise);
    break;
  }
  case f: {
    rotateClockwise(cube, (Vector3){-1, -1, SIZE - 1},
                    Cubie_rotateAntiClockWise);
    break;
  }
  case B: {
    rotateClockwise(cube, (Vector3){-1, -1, 0}, Cubie_rotateAntiClockWise);
    break;
  }
  case b: {
    rotateAntiClockwise(cube, (Vector3){-1, -1, 0}, Cubie_rotateClockWise);
    break;
  }
  case M: {
    rotateClockwise(cube, (Vector3){(int)(SIZE / 2), -1, -1}, Cubie_rotateDown);
    break;
  }
  case m: {
    rotateAntiClockwise(cube, (Vector3){(int)(SIZE / 2), -1, -1},
                        Cubie_rotateUp);
    break;
  }
  case E: {
    rotateAntiClockwise(cube, (Vector3){-1, (int)(SIZE / 2), -1},
                        Cubie_rotateRight);
    break;
  }
  case e: {
    rotateClockwise(cube, (Vector3){-1, (int)(SIZE / 2), -1}, Cubie_rotateLeft);
    break;
  }
  case S: {
    rotateAntiClockwise(cube, (Vector3){-1, -1, (int)(SIZE / 2)},
                        Cubie_rotateClockWise);
    break;
  }
  case s: {
    rotateClockwise(cube, (Vector3){-1, -1, (int)(SIZE / 2)},
                    Cubie_rotateAntiClockWise);
    break;
  }
  case X: {
    Cube_rotate(cube, R);
    Cube_rotate(cube, m);
    Cube_rotate(cube, l);
    break;
  }
  case x: {
    Cube_rotate(cube, r);
    Cube_rotate(cube, M);
    Cube_rotate(cube, L);
    break;
  }
  case Y: {
    Cube_rotate(cube, U);
    Cube_rotate(cube, e);
    Cube_rotate(cube, d);
    break;
  }
  case y: {
    Cube_rotate(cube, u);
    Cube_rotate(cube, E);
    Cube_rotate(cube, D);
    break;
  }
  case Z: {
    Cube_rotate(cube, F);
    Cube_rotate(cube, S);
    Cube_rotate(cube, b);
    break;
  }
  case z: {
    Cube_rotate(cube, f);
    Cube_rotate(cube, s);
    Cube_rotate(cube, B);
    break;
  }
  }
}
