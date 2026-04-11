#pragma once
#include "raylib.h"
#include <stdbool.h>

#define BRICK_ROWS 5
#define BRICK_COLUMNS 10
#define MAX_BRICKS (BRICK_ROWS * BRICK_COLUMNS)

typedef struct Brick {
  Rectangle rect; 
  Color color;  
  bool active;
  bool isDying;
  int alpha;           
} Brick;

void BricksInit(void);
void BricksUpdate(void);
void BricksDraw(void);

Brick* BricksGetAll(void);
int BricksGetCount(void);
