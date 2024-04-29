#include "cube.h"
#include "cublet.h"
#include "include/raylib.h"

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
      x = (dir.x == 0) ? i : (dir.x == -1) ? 0 : SIZE - 1;
      y = (dir.y == 0) ? (dir.x == 0) ? j : i : (dir.y == -1) ? 0 : SIZE - 1;
      z = (dir.z == -1) ? 0 : (dir.z == 1) ? SIZE - 1 : j;
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
      x = (dir.x == 0) ? i : (dir.x == -1) ? 0 : SIZE - 1;
      y = (dir.y == 0) ? (dir.x == 0) ? j : i : (dir.y == -1) ? 0 : SIZE - 1;
      z = (dir.z == -1) ? 0 : (dir.z == 1) ? SIZE - 1 : j;
      cube->cube[x][y][z] = face[i][j];
    }
}

void rotateClockwise(Cube *cube, Vector3 dir, void (*cubieRotation)(Cubie *)) {

  Cubie face[SIZE][SIZE];
  int x, y, z;

  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++) {
      x = (dir.x == 0) ? i : (dir.x == -1) ? 0 : SIZE - 1;
      y = (dir.y == 0) ? (dir.x == 0) ? j : i : (dir.y == -1) ? 0 : SIZE - 1;
      z = (dir.z == -1) ? 0 : (dir.z == 1) ? SIZE - 1 : j;
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
      x = (dir.x == 0) ? i : (dir.x == -1) ? 0 : SIZE - 1;
      y = (dir.y == 0) ? (dir.x == 0) ? j : i : (dir.y == -1) ? 0 : SIZE - 1;
      z = (dir.z == -1) ? 0 : (dir.z == 1) ? SIZE - 1 : j;
      cube->cube[x][y][z] = face[i][j];
    }
}

void Cube_rotate(Cube *cube, Rotation rotation) {
  switch (rotation) {
  case U: {
    rotateClockwise(cube, (Vector3){0, 1, 0}, Cubie_rotateLeft);
    break;
  }
  case u: {
    rotateAntiClockwise(cube, (Vector3){0, 1, 0}, Cubie_rotateRight);
    break;
  }
  case D: {
    rotateAntiClockwise(cube, (Vector3){0, -1, 0}, Cubie_rotateRight);
    break;
  }
  case d: {
    rotateClockwise(cube, (Vector3){0, -1, 0}, Cubie_rotateLeft);
    break;
  }
  case R: {
    rotateAntiClockwise(cube, (Vector3){1, 0, 0}, Cubie_rotateUp);
    break;
  }
  case r: {
    rotateClockwise(cube, (Vector3){1, 0, 0}, Cubie_rotateDown);
    break;
  }
  case L: {
    rotateClockwise(cube, (Vector3){-1, 0, 0}, Cubie_rotateDown);
    break;
  }
  case l: {
    rotateAntiClockwise(cube, (Vector3){-1, 0, 0}, Cubie_rotateUp);
    break;
  }
  case F: {
    rotateAntiClockwise(cube, (Vector3){0, 0, 1}, Cubie_rotateClockWise);
    break;
  }
  case f: {
    rotateClockwise(cube, (Vector3){0, 0, 1}, Cubie_rotateAntiClockWise);
    break;
  }
  case B: {
    rotateClockwise(cube, (Vector3){0, 0, -1}, Cubie_rotateAntiClockWise);
    break;
  }
  case b: {
    rotateAntiClockwise(cube, (Vector3){0, 0, -1}, Cubie_rotateClockWise);
    break;
  }
  case M: {
    int indices[] = {2, 5, 8, 11, 14, 17, 20, 23, 26};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateDown(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, BACK_TOP_MIDDLE, BACK_BOTTOM_MIDDLE, FRONT_BOTTOM_MIDDLE,
         FRONT_TOP_MIDDLE);
    swap(cube, MIDDLE_TOP_MIDDLE, BACK_MIDDLE_MIDDLE, MIDDLE_BOTTOM_MIDDLE,
         FRONT_MIDDLE_MIDDLE);
    break;
  }
  case m: {
    int indices[] = {2, 5, 8, 11, 14, 17, 20, 23, 26};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateUp(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, BACK_TOP_MIDDLE, FRONT_TOP_MIDDLE, FRONT_BOTTOM_MIDDLE,
         BACK_BOTTOM_MIDDLE);
    swap(cube, MIDDLE_TOP_MIDDLE, FRONT_MIDDLE_MIDDLE, MIDDLE_BOTTOM_MIDDLE,
         BACK_MIDDLE_MIDDLE);
    break;
  }
  case E: {
    int indices[] = {4, 5, 6, 13, 14, 15, 22, 23, 24};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateRight(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, BACK_MIDDLE_LEFT, BACK_MIDDLE_RIGHT, FRONT_MIDDLE_RIGHT,
         FRONT_MIDDLE_LEFT);
    swap(cube, BACK_MIDDLE_MIDDLE, MIDDLE_MIDDLE_RIGHT, FRONT_MIDDLE_MIDDLE,
         MIDDLE_MIDDLE_LEFT);
    break;
  }
  case e: {
    int indices[] = {4, 5, 6, 13, 14, 15, 22, 23, 24};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateLeft(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, BACK_MIDDLE_LEFT, FRONT_MIDDLE_LEFT, FRONT_MIDDLE_RIGHT,
         BACK_MIDDLE_RIGHT);
    swap(cube, BACK_MIDDLE_MIDDLE, MIDDLE_MIDDLE_LEFT, FRONT_MIDDLE_MIDDLE,
         MIDDLE_MIDDLE_RIGHT);
    break;
  }
  case S: {
    int indices[] = {10, 11, 12, 13, 14, 15, 16, 17, 18};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateClockWise(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, MIDDLE_TOP_LEFT, MIDDLE_BOTTOM_LEFT, MIDDLE_BOTTOM_RIGHT,
         MIDDLE_TOP_RIGHT);
    swap(cube, MIDDLE_TOP_MIDDLE, MIDDLE_MIDDLE_LEFT, MIDDLE_BOTTOM_MIDDLE,
         MIDDLE_MIDDLE_RIGHT);
    break;
  }
  case s: {
    int indices[] = {10, 11, 12, 13, 14, 15, 16, 17, 18};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateAntiClockWise(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, MIDDLE_TOP_LEFT, MIDDLE_TOP_RIGHT, MIDDLE_BOTTOM_RIGHT,
         MIDDLE_BOTTOM_LEFT);
    swap(cube, MIDDLE_TOP_MIDDLE, MIDDLE_MIDDLE_RIGHT, MIDDLE_BOTTOM_MIDDLE,
         MIDDLE_MIDDLE_LEFT);
    break;
  }
  }
}
