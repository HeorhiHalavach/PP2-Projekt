#include <stddef.h>
#include "raylib.h"
#include "gui/pause/pause.h"
#include "entity/ball/ball.h"
#include "entity/paddle/paddle.h"
#include "entity/brick/brick.h"
#include "entity/bonus/bonus.h"
#include "audio/audio.h"

typedef enum GameScreen { START_MENU, GAMEPLAY, GAME_OVER, WIN_SCREEN } GameScreen;

int main(void) {
  // Initialization
  //---------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(screenWidth, screenHeight, "Arkanoid");

  AudioInit();
  Texture2D bgTexture = LoadTexture("assets/background.png");
  BallSpawn(NULL);
  PaddleInit();
  BricksInit();
  BonusesInit();

  int lives = 3;
  int score = 0;
  int framesCounter = 0;

  GameScreen currentScreen = START_MENU;

  SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
  //----------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    framesCounter++;

    switch (currentScreen) {
      case START_MENU: {
        if (IsKeyPressed(KEY_SPACE)) {
          currentScreen = GAMEPLAY;
        }
      } break;

      case WIN_SCREEN: {
        if (IsKeyPressed(KEY_SPACE)) {
          lives = 3;
          score = 0;
          BricksInit();
          BonusesClear();
          BallRemove(BallGetCount(false));
          BallSpawn(NULL);
          currentScreen = START_MENU;
        }
      } break;

      case GAMEPLAY: {
        if (!PAUSE) {
          if (lives > 0) {
            Rectangle pRect = PaddleGetRect();
            BallUpdateAll(&pRect);
            PaddleUpdate();

            score += BallsCollideWithBricks();
            BricksUpdate();
            BonusesUpdate(pRect);

            if (BricksAreAllDestroyed()) {
              AudioPlay(SOUND_WIN);
              currentScreen = WIN_SCREEN;
            }

            if (BallGetCount(true) == 0) {
              lives--;
              PAUSE = true;
              if (lives > 0) {
                AudioPlay(SOUND_LOSE_LIFE);
                BallSpawn(NULL);
              } else {
                AudioPlay(SOUND_GAME_OVER);
                currentScreen = GAME_OVER;
              }
            }

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
            if (IsKeyPressed(KEY_R)) BallRemove(BallGetCount(false) - 1);
          }
        }
      } break;

      case GAME_OVER: {
        if (IsKeyPressed(KEY_SPACE)) {
          lives = 3;
          score = 0;
          BricksInit();
          BonusesClear();

          for (int i = BallGetCount(false) - 1; i >= 0; i--) {
            BallRemove(i);
          }

          BallSpawn(NULL);
          PaddleInit();
          currentScreen = START_MENU;
        }
      } break;
    }

        BeginDrawing();
        {
          ClearBackground(BLACK);
          DrawTexture(bgTexture, 0, 0, WHITE);

          switch (currentScreen) {
            case START_MENU: {
              DrawText("ARKANOID",
                       screenWidth / 2 - MeasureText("ARKANOID", 60) / 2, 100,
                       60, YELLOW);
              DrawText(
                  "PRESS SPACE TO START",
                  screenWidth / 2 - MeasureText("PRESS SPACE TO START", 20) / 2,
                  250, 20, LIGHTGRAY);
              DrawText(
                  "Use LEFT / RIGHT arrows to move",
                  screenWidth / 2 -
                      MeasureText("Use LEFT / RIGHT arrows to move", 20) / 2,
                  300, 20, GRAY);
            } break;

            case WIN_SCREEN: {
              DrawText("YOU WIN!",
                       screenWidth / 2 - MeasureText("YOU WIN!", 60) / 2, 150,
                       60, GREEN);
              DrawText(
                  TextFormat("TOTAL SCORE: %i", score),
                  screenWidth / 2 -
                      MeasureText(TextFormat("TOTAL SCORE: %i", score), 30) / 2,
                  250, 30, YELLOW);
              DrawText("PRESS SPACE TO PLAY AGAIN",
                       screenWidth / 2 -
                           MeasureText("PRESS SPACE TO PLAY AGAIN", 20) / 2,
                       320, 20, LIGHTGRAY);
            } break;

            case GAMEPLAY: {
              BricksDraw();
              BonusesDraw();
              PaddleDraw();
              BallDrawAll();
              PauseMenu();

              DrawFPS(10, 10);
              DrawText(TextFormat("BALLS: %i", BallGetCount(true)), 10, 30, 20,
                       LIGHTGRAY);
              DrawText(TextFormat("LIVES: %i", lives), 10, 50, 20, RED);
              DrawText(TextFormat("SCORE: %i", score), screenWidth - 150, 30,
                       20, GREEN);
            } break;

            case GAME_OVER: {
              DrawText("GAME OVER",
                       screenWidth / 2 - MeasureText("GAME OVER", 60) / 2, 150,
                       60, RED);
              DrawText(
                  TextFormat("FINAL SCORE: %i", score),
                  screenWidth / 2 -
                      MeasureText(TextFormat("FINAL SCORE: %i", score), 30) / 2,
                  250, 30, GREEN);
              DrawText("PRESS SPACE TO RESTART",
                       screenWidth / 2 -
                           MeasureText("PRESS SPACE TO RESTART", 20) / 2,
                       320, 20, LIGHTGRAY);
            } break;
          }
        }
        EndDrawing();
    }
    AudioCleanup();
    UnloadTexture(bgTexture);
    BricksCleanup();
    PaddleCleanup();
    BallCleanup();
    BonusesCleanup();
    // De-Initialization
    //---------------------------------------------------------
    CloseWindow();  // Close window and OpenGL context
    //----------------------------------------------------------
    return 0;
}