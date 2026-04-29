#include "ball.h"
#include <stdlib.h>
#include "../brick/brick.h"
#include "../bonus/bonus.h"
#include "../../audio/audio.h"
#include <math.h>

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------

static Ball balls[MAX_BALLS] = { 0 };
static int ballCount = 0;

static Vector2 defaultSpeed = { 0.5f, 6.0f };
static int defaultRadius = 10;
static Color defaultColor = { 190, 33, 55, 255 };

static Texture2D ballTexture;

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

/**
 * @brief Generuje losową prędkość piłki z losowym kierunkiem.
 * @return Wektor prędkości z losowym znakiem na obu osiach.
 */
static Vector2 RandomSpeed(void) {
  float sx = 4.0f + (rand() % 4);
  float sy = 3.0f + (rand() % 3);
  return (Vector2){
    .x = (rand() % 2 ? sx : -sx),
    .y = (rand() % 2 ? sy : -sy),
  };
}

/**
 * @brief Zwraca prędkość — override jeśli podany, inaczej domyślną.
 * @param override Wskaźnik na prędkość lub NULL.
 * @return Wynikowy wektor prędkości.
 */
static Vector2 ResolveSpeed(Vector2* override) {
  return override ? *override : defaultSpeed;
}

/**
 * @brief Wstawia nową piłkę do tablicy.
 * @param position Pozycja startowa.
 * @param speed    Prędkość startowa.
 * @param radius   Promień piłki.
 * @param color    Kolor piłki.
 */
static void SpawnBall(Vector2 position, Vector2 speed, int radius,
                      Color color) {
  if (ballCount >= MAX_BALLS) return;

  balls[ballCount++] = (Ball){
    .position = position,
    .speed = speed,
    .radius = radius,
    .color = color,
    .active = true,
  };
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

/**
 * @brief Tworzy piłkę z domyślnymi ustawieniami.
 * @param position Pozycja startowa lub NULL — wtedy środek ekranu.
 */
void BallSpawn(Vector2* position) {
  if (ballTexture.id == 0) {
    ballTexture = LoadTexture("assets/ball_texture.png");
  }
  Vector2 pos =
      position ? *position
               : (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

  SpawnBall(pos, ResolveSpeed(NULL), defaultRadius, defaultColor);
}

/**
 * @brief Tworzy piłkę z konfiguracją. Pola NULL przyjmują wartości domyślne.
 * @param cfg Struktura konfiguracyjna z opcjonalnymi polami.
 */
void BallSpawnConfig(BallConfig cfg) {
  Vector2 pos = cfg.position ? *cfg.position
                             : (Vector2){ GetScreenWidth() / 2.0f,
                                          GetScreenHeight() / 2.0f };

  SpawnBall(pos, ResolveSpeed(cfg.speed),
            cfg.radius ? *cfg.radius : defaultRadius,
            cfg.color ? *cfg.color : defaultColor);
}

/**
 * @brief Usuwa ostatnie `count` piłek. Jeśli count > ballCount, usuwa
 * wszystkie.
 * @param count Liczba piłek do usunięcia.
 */
void BallRemove(int count) {
  if (count <= 0) return;
  if (count > ballCount) count = ballCount;

  int newCount = ballCount - count;
  for (int i = newCount; i < ballCount; i++) balls[i] = (Ball){ 0 };

  ballCount = newCount;
}

/**
 * @brief Ustawia wartości wszystkich istniejących piłek oraz wartości domyślne.
 *        Zachowuje kierunek (znak) prędkości każdej piłki.
 * @param cfg Pola do zmiany — NULL pomija dane pole.
 */
void BallSetAll(BallConfig cfg) {
  if (cfg.speed && cfg.speed->x > 0.0f && cfg.speed->y > 0.0f) {
    defaultSpeed = *cfg.speed;
  }
  if (cfg.radius && cfg.radius > 0) {
    defaultRadius = *cfg.radius;
  }
  if (cfg.color) {
    defaultColor = *cfg.color;
  }

  for (int i = 0; i < ballCount; i++) {
    if (!balls[i].active) continue;

    if (cfg.speed) {
      balls[i].speed.x =
          cfg.speed->x * (balls[i].speed.x < 0.0f ? -1.0f : 1.0f);
      balls[i].speed.y =
          cfg.speed->y * (balls[i].speed.y < 0.0f ? -1.0f : 1.0f);
    }
    if (cfg.radius) balls[i].radius = *cfg.radius;
    if (cfg.color) balls[i].color = *cfg.color;
  }
}

/** * @brief Aktualizuje pozycje wszystkich aktywnych piłek, obsługuje odbicia
 * od ścian oraz opcjonalnie od paletki gracza. Koryguje pozycję piłki by nie
 * wchodziła w przeszkody.
 * @param paddleRect Wskaźnik na prostokąt paletki. Jeśli przekazano NULL,
 * kolizja z paletką jest ignorowana.
 */
void BallUpdateAll(Rectangle* paddleRect) {
  int w = GetScreenWidth();
  int h = GetScreenHeight();

  for (int i = 0; i < ballCount; i++) {
    if (!balls[i].active) continue;

    Ball* b = &balls[i];
    b->position.x += b->speed.x;
    b->position.y += b->speed.y;

    if (b->position.x >= w - b->radius) {
      b->position.x = w - b->radius;
      b->speed.x *= -1.0f;
      AudioPlay(SOUND_HIT_WALL);
    } else if (b->position.x <= b->radius) {
      b->position.x = b->radius;
      b->speed.x *= -1.0f;
      AudioPlay(SOUND_HIT_WALL);
    }

    if (b->position.y <= b->radius) {
      b->position.y = b->radius;
      b->speed.y *= -1.0f;
      AudioPlay(SOUND_HIT_WALL);
    }

    if (b->position.y >= h + b->radius) {
      b->active = false;
    }

    if (paddleRect != NULL) {
      if (b->speed.y > 0 && CheckCollisionCircleRec(b->position, (float)b->radius, *paddleRect)) {
        if (b->position.y < paddleRect->y + (paddleRect->height / 2.0f)) {
          float BASE_SPEED = 6.0f;
          float paddleCenter = paddleRect->x + (paddleRect->width / 2.0f);
          float hitFactor =
              (b->position.x - paddleCenter) / (paddleRect->width / 2.0f);

          b->speed.x = hitFactor * 6.0f;
          b->speed.y = -5.0f;

          float newDirectionMag =
              sqrtf(b->speed.x * b->speed.x + b->speed.y * b->speed.y);
          b->speed.x = (b->speed.x / newDirectionMag) * BASE_SPEED;
          b->speed.y = (b->speed.y / newDirectionMag) * BASE_SPEED;
          AudioPlay(SOUND_HIT_PADDLE);
        }
      }
    }
  }
}

/**
 * @brief Rysuje wszystkie aktywne piłki.
 */
void BallDrawAll(void) {
  for (int i = 0; i < ballCount; i++) {
    if (!balls[i].active) continue;

    if (ballTexture.id != 0) {
      Rectangle source = { 0.0f, 0.0f, (float)ballTexture.width,
                           (float)ballTexture.height };
      Rectangle dest = { balls[i].position.x, balls[i].position.y,
                         (float)balls[i].radius * 2,
                         (float)balls[i].radius * 2 };
      Vector2 origin = { (float)balls[i].radius, (float)balls[i].radius };

      DrawTexturePro(ballTexture, source, dest, origin, 0.0f, WHITE);
    } else {
      DrawCircleV(balls[i].position, (float)balls[i].radius, balls[i].color);
    }
  }
}

/**
 * @brief Zwraca aktualną liczbę piłek.
 * @return Liczba aktywnych piłek.
 */
int BallGetCount(bool activeOnly) {
  if (!activeOnly) {
    return ballCount;
  }

  int count = 0;
  for (int i = 0; i < ballCount; i++) {
    if (balls[i].active) {
      count++;
    }
  }
  return count;
}

/**
 * @brief Zwraca aktualne wartości domyślne piłek.
 * @return Struktura BallDefaults z domyślną prędkością, promieniem i kolorem.
 */
BallDefaults BallGetDefaults(void) {
  return (BallDefaults){
    .speed = defaultSpeed,
    .radius = defaultRadius,
    .color = defaultColor,
  };
}

/**
 * @brief Sprawdza kolizje piłek z cegłami i niszczy je przy uderzeniu.
 */
int BallsCollideWithBricks(void) {
  int pointsEarned = 0;
  Brick* bricks = BricksGetAll();
  int count = BricksGetCount();

  for (int i = 0; i < ballCount; i++) {
    if (!balls[i].active) continue;

    for (int j = 0; j < count; j++) {
      if (bricks[j].active && !bricks[j].isDying) {
        if (CheckCollisionCircleRec(balls[i].position, (float)balls[i].radius,
                                    bricks[j].rect)) {
          bricks[j].isDying = true;

          if (GetRandomValue(1, 100) <= 10) {
            BonusType type = (GetRandomValue(0, 1) == 0) ? BONUS_MULTIBALL
                                                         : BONUS_WIDE_PADDLE;
            BonusSpawn(bricks[j].rect.x + bricks[j].rect.width / 2.0f - 15.0f,
                       bricks[j].rect.y, type);
          }
          AudioPlay(SOUND_BREAK_BRICK);

          float overlapLeft =
              (balls[i].position.x + balls[i].radius) - bricks[j].rect.x;
          float overlapRight = (bricks[j].rect.x + bricks[j].rect.width) -
                               (balls[i].position.x - balls[i].radius);
          float overlapTop =
              (balls[i].position.y + balls[i].radius) - bricks[j].rect.y;
          float overlapBottom = (bricks[j].rect.y + bricks[j].rect.height) -
                                (balls[i].position.y - balls[i].radius);

          bool fromLeft = overlapLeft < overlapRight;
          float minOverlapX = fromLeft ? overlapLeft : overlapRight;

          bool fromTop = overlapTop < overlapBottom;
          float minOverlapY = fromTop ? overlapTop : overlapBottom;

          if (minOverlapX < minOverlapY) {
            balls[i].speed.x *= -1.0f; 
          } else {
            balls[i].speed.y *=
                -1.0f; 
          }

          pointsEarned += 100;
          break;
        }
      }
    }
  }
  return pointsEarned;
}

void BallCleanup(void) { UnloadTexture(ballTexture); }