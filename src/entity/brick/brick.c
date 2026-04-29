#include "brick.h"
#include <stdbool.h>
#include <stdlib.h>

static Brick bricks[MAX_BRICKS] = { 0 };
static Texture2D brickTexture;

/**
 * @brief Inicjalizuje siatkę cegieł na górze ekranu.
 */
void BricksInit(void) {
  if (brickTexture.id == 0) {
    brickTexture = LoadTexture("assets/brick_texture.png");
  }
  int brickWidth = 60;
  int brickHeight = 20;
  int padding = 5;
  int offsetX =
      (GetScreenWidth() - (BRICK_COLUMNS * (brickWidth + padding))) / 2 +
      padding / 2;
  int offsetY = 50;

  Color rowColors[BRICK_ROWS] = { RED, ORANGE, YELLOW, GREEN, BLUE };

  for (int r = 0; r < BRICK_ROWS; r++) {
    for (int c = 0; c < BRICK_COLUMNS; c++) {
      int index = r * BRICK_COLUMNS + c;

      bricks[index].rect.x = offsetX + c * (brickWidth + padding);
      bricks[index].rect.y = offsetY + r * (brickHeight + padding);
      bricks[index].rect.width = brickWidth;
      bricks[index].rect.height = brickHeight;

      bricks[index].active = true;
      bricks[index].color = rowColors[r];
      bricks[index].isDying = false;
      bricks[index].alpha = 255;
    }
  }
}

/**
 * @brief Aktualizuje logikę cegieł (animacja znikania).
 */
void BricksUpdate(void) {
  for (int i = 0; i < MAX_BRICKS; i++) {
    if (bricks[i].active && bricks[i].isDying) {
      bricks[i].alpha -= 15;
      bricks[i].rect.x += 1.0f;
      bricks[i].rect.y += 1.0f;
      bricks[i].rect.width -= 2.0f;
      bricks[i].rect.height -= 2.0f;

      if (bricks[i].alpha <= 0) {
        bricks[i].active = false;
      }
    }
  }
}

/**
 * @brief Rysuje wszystkie aktywne cegły.
 */
void BricksDraw(void) {
  for (int i = 0; i < MAX_BRICKS; i++) {

    if (!bricks[i].active) continue;

    Color color = bricks[i].color;

    color.a = bricks[i].alpha;

    DrawTexturePro(brickTexture,
                   (Rectangle){ 0.0f, 0.0f, (float)brickTexture.width,
                                (float)brickTexture.height },
                   bricks[i].rect, (Vector2){ 0.0f, 0.0f }, 0.0f,
                   color
    );
  }
}

bool BricksAreAllDestroyed(void) {
  for (int i = 0; i < MAX_BRICKS; i++) {
    if (bricks[i].active)
      return false;
  }
  return true;
}

void BricksCleanup(void) { UnloadTexture(brickTexture); }

/**
 * @brief Zwraca wskaźnik do tablicy wszystkich cegieł (przyda się do fizyki).
 */
Brick* BricksGetAll(void) { return bricks; }

/**
 * @brief Zwraca całkowitą liczbę cegieł.
 */
int BricksGetCount(void) { return MAX_BRICKS; }