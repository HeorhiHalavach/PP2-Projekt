#pragma once
#include "raylib.h"

typedef struct PaddleConfig {
  Vector2 size;
  float speed;
  Color color;
} PaddleConfig;

void PaddleInit(void);
void PaddleSetConfig(PaddleConfig config);
void PaddleUpdate(void);
void PaddleDraw(void);

Rectangle PaddleGetRect(void);