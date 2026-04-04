#include <stddef.h>
#include "raylib.h"
#include "gui/pause/pause.h"
#include "entity/ball/ball.h"
#include "entity/paddle/paddle.h"

int main(void) {
  // Initialization
  //---------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(screenWidth, screenHeight, "Arkanoid");

  BallSpawn(NULL);

  Paddle playerPaddle;
  PaddleInit(&playerPaddle);

  int framesCounter = 0;

  SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
  //----------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    framesCounter++;

    BeginDrawing();
    {
      PauseMenu();
      PaddleDraw(&playerPaddle);

      if (!PAUSE) {
        BallUpdateAll();
        PaddleUpdate(&playerPaddle);

        Rectangle paddleRect = { 
            playerPaddle.position.x,
            playerPaddle.position.y, 
            playerPaddle.size.x,
            playerPaddle.size.y };

        BallsCollideWithPaddle(paddleRect);

        if (IsKeyPressed(KEY_S)) BallSpawn(NULL);

        if (IsKeyPressed(KEY_UP)) {
          Vector2 currentSpeed = BallGetDefaults().speed;
          BallSetAll(
              (BallConfig){ .speed = &(Vector2){ currentSpeed.x + 1.0f,
                                                 currentSpeed.y + 1.0f } });
        }
        if (IsKeyPressed(KEY_DOWN)) {
          Vector2 currentSpeed = BallGetDefaults().speed;
          BallSetAll(
              (BallConfig){ .speed = &(Vector2){ currentSpeed.x - 1.0f,
                                                 currentSpeed.y - 1.0f } });
        }
        if (IsKeyPressed(KEY_R)) BallRemove(BallGetCount() - 1);
      }

      ClearBackground(RAYWHITE);

      BallDrawAll();
      // DrawText("PRESS SPACE to PAUSE BALL MOVEMENT", 10,
      // GetScreenHeight() - 25, 20, LIGHTGRAY);

      // On pause, we draw a blinking message
      // if (PAUSE) DrawText("PAUSED", 350, 200, 30, GRAY);

      DrawFPS(10, 10);
      DrawText(TextFormat("BALLS: %i", BallGetCount()), 10, 30, 20, LIGHTGRAY);
    }
    EndDrawing();
  }

  // De-Initialization
  //---------------------------------------------------------
  CloseWindow();  // Close window and OpenGL context
  //----------------------------------------------------------

  return 0;
}