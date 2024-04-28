#include "cube.h"
#include "include/raylib.h"
#include <math.h>
#include <string.h>

#define CUBE_SIZE 0.95

float camera_mag = 5;
float camera_mag_vel = 0.0f;
float camera_theta = PI / 4;
float camera_phi = PI / 4;

Camera camera = {{0}, {0, 0, 0}, {0, 1, 0}, 90, CAMERA_PERSPECTIVE};

void handleKeyPress(Cube *cube) {
  // int x = GetKeyPressed();
  // if (x != 0)
  //   printf("%d\n", x);

  if (IsKeyPressed(KEY_U)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(cube, u);
    else
      Cube_rotate(cube, U);
  } else if (IsKeyPressed(KEY_D)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(cube, d);
    else
      Cube_rotate(cube, D);
  } else if (IsKeyPressed(KEY_L)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(cube, l);
    else
      Cube_rotate(cube, L);
  } else if (IsKeyPressed(KEY_R)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(cube, r);
    else
      Cube_rotate(cube, R);
  } else if (IsKeyPressed(KEY_F)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(cube, f);
    else
      Cube_rotate(cube, F);
  } else if (IsKeyPressed(KEY_B)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(cube, b);
    else
      Cube_rotate(cube, B);
  } else if (IsKeyPressed(KEY_M)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(cube, m);
    else
      Cube_rotate(cube, M);
  } else if (IsKeyPressed(KEY_E)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(cube, e);
    else
      Cube_rotate(cube, E);
  } else if (IsKeyPressed(KEY_S)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(cube, s);
    else
      Cube_rotate(cube, S);
  }
}

void handleMouseMovementAndUpdateCamera() {
  float dt = GetFrameTime();

  camera_mag += camera_mag_vel * dt;
  if (camera_mag < SIZE)
    camera_mag = SIZE;
  if (camera_mag > 3 * SIZE)
    camera_mag = 3 * SIZE;
  camera_mag_vel -= GetMouseWheelMove() * 10;
  camera_mag_vel *= 0.9;

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Vector2 delta = GetMouseDelta();
    camera_theta -= delta.x * 0.005;
    camera_phi -= delta.y * 0.005;
  }
  if (camera_phi >= PI)
    camera_phi = PI - 0.01;
  if (camera_phi <= 0)
    camera_phi = 0.01;

  camera.position.z = sinf(camera_phi) * cosf(camera_theta) * camera_mag;
  camera.position.x = sinf(camera_phi) * sinf(camera_theta) * camera_mag;
  camera.position.y = cosf(camera_phi) * camera_mag;
}

int main(int argc, char **argv) {
  InitWindow(1200, 800, "Rubik's Cube");
  SetTargetFPS(40);

  Cube cube = Cube_make();

  for (int i = 0; i < strlen(argv[1]); i++) {
    char curr = (argv[1][i] == '2') ? argv[1][i - 1] : argv[1][i];
    Cube_rotate(&cube, getCorrespondingRotation(curr));
  }

  // int x = 0;

  while (!WindowShouldClose()) {
    handleMouseMovementAndUpdateCamera();

    handleKeyPress(&cube);

    // if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    //   x = (x == 26) ? 0 : x + 1;

    BeginDrawing();

    BeginMode3D(camera);
    ClearBackground(LIGHTGRAY);

    DrawCube((Vector3){0}, SIZE - 0.1f, SIZE - 0.1f, SIZE - 0.1f, BLACK);
    for (int z = 0; z < SIZE; z++)
      for (int y = 0; y < SIZE; y++)
        for (int x = 0; x < SIZE; x++)
          Cubie_drawCubie(&cube.cube[x][y][z],
                          (Vector3){x - (int)(SIZE / 2), y - (int)(SIZE / 2),
                                    z - (int)(SIZE / 2)});

    // Cubie_drawCubie(&cube.cube[6 % 3][6 / 3 % 3][6 / 9],
    //                 (Vector3){6 % 3 - 1, 6 / 3 % 3 - 1, 6 / 9 - 1});
    // Cubie_drawCubie(&cube.cube[8 % 3][8 / 3 % 3][8 / 9],
    //                 (Vector3){8 % 3 - 1, 8 / 3 % 3 - 1, 8 / 9 - 1});
    // Cubie_drawCubie(&cube.cube[24 % 3][24 / 3 % 3][24 / 9],
    //                 (Vector3){24 % 3 - 1, 24 / 3 % 3 - 1, 24 / 9 - 1});
    // Cubie_drawCubie(&cube.cube[26 % 3][26 / 3 % 3][26 / 9],
    //                 (Vector3){26 % 3 - 1, 26 / 3 % 3 - 1, 26 / 9 - 1});

    EndMode3D();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
