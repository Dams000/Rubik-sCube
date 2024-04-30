#include "cube.h"
#include "include/raylib.h"
#include "include/raymath.h"
#include <math.h>
#include <stdarg.h>

#define CUBIE_SIZE 0.9

float camera_mag = 20;
float camera_mag_vel = 0.0f;
float camera_theta = PI / 4;
float camera_phi = PI / 4;

Camera camera = {{0}, {0, 0, 0}, {0, 1, 0}, 90, CAMERA_PERSPECTIVE};

Cube cube;

void handleKeyPress() {
  if (IsKeyPressed(KEY_U)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, u);
    else
      Cube_rotate(&cube, U);
  } else if (IsKeyPressed(KEY_D)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, d);
    else
      Cube_rotate(&cube, D);
  } else if (IsKeyPressed(KEY_L)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, l);
    else
      Cube_rotate(&cube, L);
  } else if (IsKeyPressed(KEY_R)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, r);
    else
      Cube_rotate(&cube, R);
  } else if (IsKeyPressed(KEY_F)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, f);
    else
      Cube_rotate(&cube, F);
  } else if (IsKeyPressed(KEY_B)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, b);
    else
      Cube_rotate(&cube, B);
  } else if (IsKeyPressed(KEY_M)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, m);
    else
      Cube_rotate(&cube, M);
  } else if (IsKeyPressed(KEY_E)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, e);
    else
      Cube_rotate(&cube, E);
  } else if (IsKeyPressed(KEY_S)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, s);
    else
      Cube_rotate(&cube, S);
  } else if (IsKeyPressed(KEY_X)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, x);
    else
      Cube_rotate(&cube, X);
  } else if (IsKeyPressed(KEY_Y)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, y);
    else
      Cube_rotate(&cube, Y);
  } else if (IsKeyPressed(KEY_Z)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, z);
    else
      Cube_rotate(&cube, Z);
  }
}

void handleMouseMovementAndUpdateCamera() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    cube = Cube_make(CUBIE_SIZE);

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
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1200, 800, "Rubik's Cube");
  SetWindowMinSize(600, 400);
  SetTargetFPS(40);

  cube = Cube_make(CUBIE_SIZE);

  if (argc == 2) {
    char *moves = argv[1];
    Cube_applyMoves(&cube, moves);
  }

  while (!WindowShouldClose()) {
    handleMouseMovementAndUpdateCamera();

    handleKeyPress();

    BeginDrawing();

    BeginMode3D(camera);
    ClearBackground(LIGHTGRAY);

    DrawLine3D(Vector3Zero(), (Vector3){(float)SIZE / 2 + 2, 0, 0}, GRAY);
    DrawLine3D(Vector3Zero(), (Vector3){0, (float)SIZE / 2 + 2, 0}, GRAY);
    DrawLine3D(Vector3Zero(), (Vector3){0, 0, (float)SIZE / 2 + 2}, GRAY);
    // DrawCube((Vector3){0}, SIZE - (1 - CUBIE_SIZE) - 0.05,
    //          SIZE - (1 - CUBIE_SIZE) - 0.05, SIZE - (1 - CUBIE_SIZE) - 0.05,
    //          BLACK);
    for (int z = 0; z < SIZE; z++)
      for (int y = 0; y < SIZE; y++)
        for (int x = 0; x < SIZE; x++)
          Cubie_drawCubie(&cube.cube[x][y][z],
                          (Vector3){x - (float)SIZE / 2 + 0.5f,
                                    y - (float)SIZE / 2 + 0.5f,
                                    z - (float)SIZE / 2 + 0.5f});

    EndMode3D();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
