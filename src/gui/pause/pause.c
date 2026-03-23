#include "raylib.h"
#include <stdio.h>

bool PAUSE = false;

void PauseMenu() {
  if (IsKeyPressed(KEY_SPACE)) PAUSE = !PAUSE;

  if (PAUSE) DrawText("PAUSED", 350, 200, 30, GRAY);
}