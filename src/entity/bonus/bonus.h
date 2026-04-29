#pragma once
#include "raylib.h"
#include <stdbool.h>

typedef enum { BONUS_MULTIBALL, BONUS_WIDE_PADDLE } BonusType;

void BonusesInit(void);
void BonusesClear(void);
void BonusSpawn(float x, float y, BonusType type);
void BonusesUpdate(Rectangle paddleRect);
void BonusesDraw(void);
void BonusesCleanup(void);