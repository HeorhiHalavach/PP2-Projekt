#include "ball.h"
#include <stdlib.h>
#include "../brick/brick.h"

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------

static Ball balls[MAX_BALLS] = { 0 };
static int ballCount = 0;

static Vector2 defaultSpeed = { 5.0f, 5.0f };
static int defaultRadius = 20;
static Color defaultColor = { 190, 33, 55, 255 };

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

/**
 * @brief Aktualizuje pozycje wszystkich aktywnych piłek i obsługuje odbicia od
 * ścian. Koryguje pozycję piłki by nie wchodziła w ścianę.
 */
void BallUpdateAll(void) {
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
    } else if (b->position.x <= b->radius) {
      b->position.x = b->radius;
      b->speed.x *= -1.0f;
    }

    if (b->position.y >= h - b->radius) {
      b->position.y = h - b->radius;
      b->speed.y *= -0.95f;
    } else if (b->position.y <= b->radius) {
      b->position.y = b->radius;
      b->speed.y *= -0.95f;
    }
  }
}

/**
 * @brief Rysuje wszystkie aktywne piłki.
 */
void BallDrawAll(void) {
  for (int i = 0; i < ballCount; i++) {
    if (!balls[i].active) continue;
    DrawCircleV(balls[i].position, (float)balls[i].radius, balls[i].color);
  }
}

/**
 * @brief Zwraca aktualną liczbę piłek.
 * @return Liczba aktywnych piłek.
 */
int BallGetCount(void) { return ballCount; }

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
 * @brief Sprawdza kolizje wszystkich piłek z paletką i odbija je pod
 * odpowiednim kątem.
 */
void BallsCollideWithPaddle(Rectangle paddle) {
  for (int i = 0; i < ballCount; i++) {
    if (!balls[i].active) continue;

    if (CheckCollisionCircleRec(balls[i].position, (float)balls[i].radius,
                                paddle)) {
      if (balls[i].speed.y > 0.0f) {
        balls[i].speed.y = -5.0f;

        float paddleCenter = paddle.x + paddle.width / 2.0f;
        float hitPoint =
            (balls[i].position.x - paddleCenter) / (paddle.width / 2.0f);

        balls[i].speed.x = hitPoint * 6.0f;

        float minSpeedX = 1.5f;

        if (balls[i].speed.x >= 0.0f && balls[i].speed.x < minSpeedX) {
          balls[i].speed.x = minSpeedX;
        } else if (balls[i].speed.x < 0.0f && balls[i].speed.x > -minSpeedX) {
          balls[i].speed.x = -minSpeedX;
        }
      }
    }
  }
}

/**
 * @brief Sprawdza kolizje piłek z cegłami i niszczy je przy uderzeniu.
 */
void BallsCollideWithBricks(void) {
  Brick* bricks = BricksGetAll();
  int count = BricksGetCount();

  for (int i = 0; i < ballCount; i++) {
    if (!balls[i].active) continue;

    for (int j = 0; j < count; j++) {
      if (bricks[j].active && !bricks[j].isDying) {
        if (CheckCollisionCircleRec(balls[i].position, (float)balls[i].radius,
                                    bricks[j].rect)) {
          bricks[j].isDying = true;

          balls[i].speed.y *= -1.0f;
          break;
        }
      }
    }
  }
}