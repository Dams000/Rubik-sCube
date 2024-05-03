#include "cube.h"
#include "scramble.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CUBIE_SIZE 0.9

float camera_mag = 2 * SIZE;
float camera_mag_vel = 0.0f;
float camera_theta = PI / 5;
float camera_phi = PI / 3;

Camera camera = {{0}, {0, 0, 0}, {0, 1, 0}, 90, CAMERA_PERSPECTIVE};

Cube cube;
char *scramble[SCRAMBLE_SIZE];
char currentScramble[5 * SCRAMBLE_SIZE + 1];

bool showHelp = false;

char *enter = "Press 'Enter' to scramble the cube.";
char *rotateFace =
    "Press the corresponding key to move each face (Hold alt down for "
    "prime moves):";
char *facesKey = "R (right), L (left), U (up), D (down), F (front), B (back).";
char *mouseRight =
    "Press right mouse button to reset the cube to its original, solved state.";
char *mouseMiddle = "Press middle mouse button to reset camera settings.";
char *mouseLeft = "Hold left mouse button down to move the camera around.";

void handleKeyPress() {
  if (IsKeyPressed(KEY_U)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, u, 1);
    else
      Cube_rotate(&cube, U, 1);
  } else if (IsKeyPressed(KEY_D)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, d, 1);
    else
      Cube_rotate(&cube, D, 1);
  } else if (IsKeyPressed(KEY_L)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, l, 1);
    else
      Cube_rotate(&cube, L, 1);
  } else if (IsKeyPressed(KEY_R)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, r, 1);
    else
      Cube_rotate(&cube, R, 1);
  } else if (IsKeyPressed(KEY_F)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, f, 1);
    else
      Cube_rotate(&cube, F, 1);
  } else if (IsKeyPressed(KEY_B)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, b, 1);
    else
      Cube_rotate(&cube, B, 1);
  } else if (IsKeyPressed(KEY_M)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, m, 1);
    else
      Cube_rotate(&cube, M, 1);
  } else if (IsKeyPressed(KEY_E)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, e, 1);
    else
      Cube_rotate(&cube, E, 1);
  } else if (IsKeyPressed(KEY_S)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, s, 1);
    else
      Cube_rotate(&cube, S, 1);
  } else if (IsKeyPressed(KEY_X)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, x, 1);
    else
      Cube_rotate(&cube, X, 1);
  } else if (IsKeyPressed(KEY_Y)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, y, 1);
    else
      Cube_rotate(&cube, Y, 1);
  } else if (IsKeyPressed(KEY_Z)) {
    if (IsKeyDown(KEY_LEFT_ALT))
      Cube_rotate(&cube, z, 1);
    else
      Cube_rotate(&cube, Z, 1);
  } else if (IsKeyPressed(KEY_ENTER)) {
    currentScramble[0] = '\0';
    Cube_free(cube);
    cube = Cube_make(CUBIE_SIZE);
    generateScramble(scramble, SIZE);

    for (int i = 0; i < SCRAMBLE_SIZE; i++) {
      Cube_applyMoves(&cube, scramble[i]);
      if (scramble[i][0] == '1')
        strcat(currentScramble, scramble[i] + 2);
      else
        strcat(currentScramble, scramble[i]);
      free(scramble[i]);
      strcat(currentScramble, " ");
    }
  }
}

void handleMouseMovementAndUpdateCamera() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    Cube_free(cube);
    cube = Cube_make(CUBIE_SIZE);
    currentScramble[0] = '\0';
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
    camera_mag = 2 * SIZE;
    camera_mag_vel = 0.0f;
    camera_theta = PI / 5;
    camera_phi = PI / 3;
  }

  float dt = GetFrameTime();

  camera_mag += camera_mag_vel * dt;
  if (camera_mag < 1.25f * SIZE)
    camera_mag = 1.25f * SIZE;
  if (camera_mag > 2.5f * SIZE)
    camera_mag = 2.5f * SIZE;
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

void drawHelpScreen() {

  int fontSize =
      fmax(fmin(floor((float)(GetScreenWidth() - 100) / 400) * 10, 40), 18);

  ClearBackground(LIGHTGRAY);
  DrawText("Press 'h' to exit.", 10, 10, 20, DARKGRAY);
  DrawText(enter, GetScreenWidth() / 2 - MeasureText(enter, fontSize) / 2,
           GetScreenHeight() / 2 - 150, fontSize, BLACK);
  DrawText(rotateFace,
           GetScreenWidth() / 2 - MeasureText(rotateFace, fontSize) / 2,
           GetScreenHeight() / 2 - 100, fontSize, BLACK);
  DrawText(facesKey, GetScreenWidth() / 2 - MeasureText(facesKey, fontSize) / 2,
           GetScreenHeight() / 2 - 50, fontSize, BLACK);
  DrawText(mouseRight,
           GetScreenWidth() / 2 - MeasureText(mouseRight, fontSize) / 2,
           GetScreenHeight() / 2, fontSize, BLACK);
  DrawText(mouseMiddle,
           GetScreenWidth() / 2 - MeasureText(mouseMiddle, fontSize) / 2,
           GetScreenHeight() / 2 + 50, fontSize, BLACK);
  DrawText(mouseLeft,
           GetScreenWidth() / 2 - MeasureText(mouseLeft, fontSize) / 2,
           GetScreenHeight() / 2 + 100, fontSize, BLACK);
}

void drawCube() {
  BeginMode3D(camera);
  ClearBackground(LIGHTGRAY);

  DrawLine3D(Vector3Zero(), (Vector3){(float)SIZE / 2 + 2, 0, 0}, GRAY);
  DrawLine3D(Vector3Zero(), (Vector3){0, (float)SIZE / 2 + 2, 0}, GRAY);
  DrawLine3D(Vector3Zero(), (Vector3){0, 0, (float)SIZE / 2 + 2}, GRAY);
  DrawCube((Vector3){0}, SIZE - (1 - CUBIE_SIZE) - 0.05,
           SIZE - (1 - CUBIE_SIZE) - 0.05, SIZE - (1 - CUBIE_SIZE) - 0.05,
           BLACK);
  Cube_drawCube(&cube);

  EndMode3D();
  DrawText("Press 'h' for help.", 10, 10, 20, DARKGRAY);
  DrawText("Current scramble:",
           GetScreenWidth() / 2 - MeasureText("Current scramble:", 30) / 2, 10,
           30, BLACK);
  DrawText(currentScramble,
           GetScreenWidth() / 2 - MeasureText(currentScramble, 20) / 2, 50, 20,
           BLACK);
}

int main(int argc, char **argv) {
  SetTraceLogLevel(LOG_WARNING);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1200, 800, "Rubik's Cube");
  SetWindowMinSize(800, 600);
  SetTargetFPS(40);

  cube = Cube_make(0.9f);
  printf("oki \n");

  if (argc == 2) {
    char *moves = argv[1];
    Cube_applyMoves(&cube, moves);
  }

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_H))
      showHelp = !showHelp;

    if (!showHelp) {
      handleMouseMovementAndUpdateCamera();
      handleKeyPress();
    }

    BeginDrawing();

    if (showHelp)
      drawHelpScreen();
    else
      drawCube();

    EndDrawing();
  }

  Cube_free(cube);

  CloseWindow();
  return 0;
}
