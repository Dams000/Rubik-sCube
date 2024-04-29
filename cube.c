#include "cube.h"
#include "cublet.h"

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

void rotate(Cube *cube) {

  Cubie right_face[SIZE][SIZE];

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      right_face[i][j] = cube->cube[SIZE - 1][i][j];
    }
  }

  // Transpose the matrix
  for (int i = 0; i < 3; i++) {
    for (int j = i + 1; j < 3; j++) {
      Cubie temp = right_face[i][j];
      right_face[i][j] = right_face[j][i];
      right_face[j][i] = temp;
    }
  }

  // Reverse each row
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3 / 2; j++) {
      Cubie temp = right_face[i][j];
      right_face[i][j] = right_face[i][3 - j - 1];
      right_face[i][3 - j - 1] = temp;
    }
  }
}

void Cube_rotate(Cube *cube, Rotation rotation) {
  switch (rotation) {
  case U: {
    int indices[] = {7, 8, 9, 16, 17, 18, 25, 26, 27};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateLeft(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, BACK_TOP_LEFT, FRONT_TOP_LEFT, FRONT_TOP_RIGHT, BACK_TOP_RIGHT);
    swap(cube, BACK_TOP_MIDDLE, MIDDLE_TOP_LEFT, FRONT_TOP_MIDDLE,
         MIDDLE_TOP_RIGHT);
    break;
  }
  case u: {
    int indices[] = {7, 8, 9, 16, 17, 18, 25, 26, 27};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateRight(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, FRONT_TOP_LEFT, BACK_TOP_LEFT, BACK_TOP_RIGHT, FRONT_TOP_RIGHT);
    swap(cube, FRONT_TOP_MIDDLE, MIDDLE_TOP_LEFT, BACK_TOP_MIDDLE,
         MIDDLE_TOP_RIGHT);
    break;
  }
  case D: {
    int indices[] = {1, 2, 3, 10, 11, 12, 19, 20, 21};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateRight(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, FRONT_BOTTOM_LEFT, BACK_BOTTOM_LEFT, BACK_BOTTOM_RIGHT,
         FRONT_BOTTOM_RIGHT);
    swap(cube, FRONT_BOTTOM_MIDDLE, MIDDLE_BOTTOM_LEFT, BACK_BOTTOM_MIDDLE,
         MIDDLE_BOTTOM_RIGHT);
    break;
  }
  case d: {
    int indices[] = {1, 2, 3, 10, 11, 12, 19, 20, 21};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateLeft(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, BACK_BOTTOM_LEFT, FRONT_BOTTOM_LEFT, FRONT_BOTTOM_RIGHT,
         BACK_BOTTOM_RIGHT);
    swap(cube, BACK_BOTTOM_MIDDLE, MIDDLE_BOTTOM_LEFT, FRONT_BOTTOM_MIDDLE,
         MIDDLE_BOTTOM_RIGHT);
    break;
  }
  case R: {
    int indices[] = {3, 6, 9, 12, 15, 18, 21, 24, 27};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateUp(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, FRONT_TOP_RIGHT, FRONT_BOTTOM_RIGHT, BACK_BOTTOM_RIGHT,
         BACK_TOP_RIGHT);
    swap(cube, MIDDLE_TOP_RIGHT, FRONT_MIDDLE_RIGHT, MIDDLE_BOTTOM_RIGHT,
         BACK_MIDDLE_RIGHT);
    break;
  }
  case r: {
    int indices[] = {3, 6, 9, 12, 15, 18, 21, 24, 27};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateDown(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, BACK_TOP_RIGHT, BACK_BOTTOM_RIGHT, FRONT_BOTTOM_RIGHT,
         FRONT_TOP_RIGHT);
    swap(cube, BACK_MIDDLE_RIGHT, MIDDLE_BOTTOM_RIGHT, FRONT_MIDDLE_RIGHT,
         MIDDLE_TOP_RIGHT);
    break;
  }
  case L: {
    int indices[] = {1, 4, 7, 10, 13, 16, 19, 22, 25};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateDown(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, BACK_TOP_LEFT, BACK_BOTTOM_LEFT, FRONT_BOTTOM_LEFT,
         FRONT_TOP_LEFT);
    swap(cube, BACK_MIDDLE_LEFT, MIDDLE_BOTTOM_LEFT, FRONT_MIDDLE_LEFT,
         MIDDLE_TOP_LEFT);
    break;
  }
  case l: {
    int indices[] = {1, 4, 7, 10, 13, 16, 19, 22, 25};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateUp(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, FRONT_TOP_LEFT, FRONT_BOTTOM_LEFT, BACK_BOTTOM_LEFT,
         BACK_TOP_LEFT);
    swap(cube, MIDDLE_TOP_LEFT, FRONT_MIDDLE_LEFT, MIDDLE_BOTTOM_LEFT,
         BACK_MIDDLE_LEFT);
    break;
  }
  case F: {
    int indices[] = {19, 20, 21, 22, 23, 24, 25, 26, 27};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateClockWise(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, FRONT_TOP_RIGHT, FRONT_TOP_LEFT, FRONT_BOTTOM_LEFT,
         FRONT_BOTTOM_RIGHT);
    swap(cube, FRONT_TOP_MIDDLE, FRONT_MIDDLE_LEFT, FRONT_BOTTOM_MIDDLE,
         FRONT_MIDDLE_RIGHT);
    break;
  }
  case f: {
    int indices[] = {19, 20, 21, 22, 23, 24, 25, 26, 27};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateAntiClockWise(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, FRONT_TOP_LEFT, FRONT_TOP_RIGHT, FRONT_BOTTOM_RIGHT,
         FRONT_BOTTOM_LEFT);
    swap(cube, FRONT_TOP_MIDDLE, FRONT_MIDDLE_RIGHT, FRONT_BOTTOM_MIDDLE,
         FRONT_MIDDLE_LEFT);
    break;
  }
  case B: {
    int indices[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateAntiClockWise(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, BACK_TOP_LEFT, BACK_TOP_RIGHT, BACK_BOTTOM_RIGHT,
         BACK_BOTTOM_LEFT);
    swap(cube, BACK_TOP_MIDDLE, BACK_MIDDLE_RIGHT, BACK_BOTTOM_MIDDLE,
         BACK_MIDDLE_LEFT);
    break;
  }
  case b: {
    int indices[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 9; i++) {
      int index = indices[i];
      Cubie_rotateClockWise(
          &cube->cube[(index - 1) % 3][(index - 1) / 3 % 3][(index - 1) / 9]);
    }
    swap(cube, BACK_TOP_RIGHT, BACK_TOP_LEFT, BACK_BOTTOM_LEFT,
         BACK_BOTTOM_RIGHT);
    swap(cube, BACK_TOP_MIDDLE, BACK_MIDDLE_LEFT, BACK_BOTTOM_MIDDLE,
         BACK_MIDDLE_RIGHT);
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
