#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"

typedef struct Paddle {
  Vector2 position;
  Vector2 size;
  float speed;
  Color color;
} Paddle;

void PaddleInit(Paddle *paddle);
void PaddleUpdate(Paddle *paddle);
void PaddleDraw(Paddle *paddle);

#endif