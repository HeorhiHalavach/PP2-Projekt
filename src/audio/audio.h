#pragma once
#include "raylib.h"

typedef enum {
  SOUND_HIT_PADDLE,
  SOUND_HIT_WALL,
  SOUND_BREAK_BRICK,
  SOUND_LOSE_LIFE,
  SOUND_GAME_OVER,
  SOUND_WIN
} GameSound;

void AudioInit(void);
void AudioPlay(GameSound sound);
void AudioCleanup(void);
