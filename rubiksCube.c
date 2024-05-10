#include "cube.h"
#include "include/raylib.h"
#include "scramble.h"
#include "timer.h"
#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CUBIE_SIZE 0.9

float camera_mag = 2 * SIZE;
float camera_mag_vel = 0.0f;
float camera_theta = PI / 5;
float camera_phi = PI / 3;

Camera camera = {{0}, {0, 0, 0}, {0, 1, 0}, 90, CAMERA_PERSPECTIVE};

Cube cube;
char **scramble;
char *currentScramble;

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
char *spaceBar = "Press the space bar to start (or stop) the timer";

Timer timer;
Color timerColor = BLACK;
char timmerString[10] = "00:00.000";

void handleRotation(Rotation clockwise, Rotation antiClockwise) {
  if (IsKeyPressed(KEY_LEFT_ALT))
    Cube_rotate(&cube, antiClockwise, 1);
  else
    Cube_rotate(&cube, clockwise, 1);
}

void handleKeyPress() {
  if (IsKeyPressed(KEY_U)) {
    handleRotation(U, u);
  } else if (IsKeyPressed(KEY_D)) {
    handleRotation(D, d);
  } else if (IsKeyPressed(KEY_L)) {
    handleRotation(L, l);
  } else if (IsKeyPressed(KEY_R)) {
    handleRotation(R, r);
  } else if (IsKeyPressed(KEY_F)) {
    handleRotation(F, f);
  } else if (IsKeyPressed(KEY_B)) {
    handleRotation(B, b);
  } else if (IsKeyPressed(KEY_M)) {
    handleRotation(M, m);
  } else if (IsKeyPressed(KEY_E)) {
    handleRotation(E, e);
  } else if (IsKeyPressed(KEY_S)) {
    handleRotation(S, s);
  } else if (IsKeyPressed(KEY_X)) {
    handleRotation(X, x);
  } else if (IsKeyPressed(KEY_Y)) {
    handleRotation(Y, y);
  } else if (IsKeyPressed(KEY_Z)) {
    handleRotation(Z, z);
  } else if (IsKeyPressed(KEY_ENTER)) {
    currentScramble[0] = '\0';
    Cube_free(cube);
    cube = Cube_make(CUBIE_SIZE);
    generateScramble(scramble, SIZE);

    for (int i = 0; i < SCRAMBLE_SIZE; i++) {
      Cube_applyMove(&cube, scramble[i]);
      if (scramble[i][0] == '1' && scramble[i][1] == 'w')
        strcat(currentScramble, scramble[i] + 2);
      else
        strcat(currentScramble, scramble[i]);
      free(scramble[i]);
      if (i != SCRAMBLE_SIZE - 1)
        strcat(currentScramble, " ");
    }
    char cubeStr[55];
    Cube_toString(&cube, cubeStr);
    cubeStr[54] = '\0';
    printf("%s\n", cubeStr);
  } else if (IsKeyDown(KEY_SPACE)) {
    if (!timer.isRunning && !timer.justStopped)
      timerColor = (Color){0, 204, 51, 255};
    else
      Timer_stop(&timer);
  } else if (IsKeyReleased(KEY_SPACE)) {
    if (timer.justStopped) {
      timer.justStopped = false;
      return;
    }
    timerColor = BLACK;
    if (!timer.isRunning)
      Timer_start(&timer);
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
  DrawText(spaceBar, GetScreenWidth() / 2 - MeasureText(spaceBar, fontSize) / 2,
           GetScreenHeight() / 2 + 150, fontSize, BLACK);
}

void DrawTextBoxed(const char *text, float fontSize, int y) {
  if (strlen(text) == 0)
    return;

  int lastSpace = 0;
  char *dup = strdup(text);
  char *lastSpacePtr = strrchr(dup, ' ');
  lastSpace = (lastSpacePtr != NULL) ? lastSpacePtr - dup : -1;
  while (MeasureText(dup, fontSize) > GetScreenWidth() - 20) {
    if (lastSpace == -1)
      break;
    dup[lastSpace] = '\0';
    lastSpacePtr = strrchr(dup, ' ');
    lastSpace = (lastSpacePtr != NULL) ? lastSpacePtr - dup : -1;
  }
  DrawText(dup, GetScreenWidth() / 2 - MeasureText(dup, fontSize) / 2, y,
           fontSize, BLACK);
  if (strlen(text) > strlen(dup))
    DrawTextBoxed(text + strlen(dup) + 1, fontSize, y + 30);
  free(dup);
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
  Timer_update(&timer);
  snprintf(timmerString, 10, "%02d:%02d.%03d", timer.minutes, timer.seconds,
           timer.milliseconds);
  DrawText(timmerString, GetScreenWidth() / 2 - MeasureText("00:00.00", 40) / 2,
           GetScreenHeight() - 50, 40, timerColor);
  DrawTextBoxed(currentScramble, 20, 50);
}

int main(int argc, char **argv) {
  SetTraceLogLevel(LOG_WARNING);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1200, 800, "Rubik's Cube");
  SetWindowMinSize(800, 600);
  SetTargetFPS(40);

  cube = Cube_make(CUBIE_SIZE);
  timer = Timer_make();

  scramble = malloc(SCRAMBLE_SIZE * sizeof(char *));
  currentScramble = malloc((6 * SCRAMBLE_SIZE + 1) * sizeof(char));
  currentScramble[0] = '\0';

  if (argc >= 2)
    for (int i = 1; i < argc; i++)
      Cube_applyMove(&cube, argv[i]);

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

  free(currentScramble);
  free(scramble);
  Cube_free(cube);

  CloseWindow();
  return 0;
}
