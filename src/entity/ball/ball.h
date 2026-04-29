#pragma once
#include "raylib.h"
#include <stddef.h>

#define MAX_BALLS 32

typedef struct {
  Vector2 position;
  Vector2 speed;
  int radius;
  Color color;
  bool active;
} Ball;

typedef struct {
  Vector2* position;
  Vector2* speed;
  int* radius;
  Color* color;
} BallConfig;

typedef struct {
  Vector2 speed;
  int radius;
  Color color;
} BallDefaults;

void BallSpawn(Vector2* position);
void BallSpawnConfig(BallConfig config);
void BallRemove(int count);

void BallSetAll(BallConfig config);
void BallUpdateAll(Rectangle* paddleRect);
void BallDrawAll(void);

int BallGetCount(bool activeOnly);
BallDefaults BallGetDefaults(void);
int BallsCollideWithBricks(void);
void BallCleanup(void);
