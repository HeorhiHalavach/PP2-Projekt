#include "audio.h"

static Sound sndHitPaddle;
static Sound sndHitWall;
static Sound sndBreakBrick;
static Sound sndLoseLife;
static Sound sndGameOver;
static Sound sndWin;

void AudioInit(void) {
  InitAudioDevice();

  sndHitPaddle = LoadSound("assets/hit_paddle.wav");
  sndHitWall = LoadSound("assets/hit_wall.wav");
  sndBreakBrick = LoadSound("assets/break_brick.wav");
  sndLoseLife = LoadSound("assets/lose_life.wav");
  sndGameOver = LoadSound("assets/game_over.wav");
  sndWin = LoadSound("assets/win.wav");
}

void AudioPlay(GameSound sound) {
  switch (sound) {
    case SOUND_HIT_PADDLE:
      PlaySound(sndHitPaddle);
      break;
    case SOUND_HIT_WALL:
      PlaySound(sndHitWall);
      break;
    case SOUND_BREAK_BRICK:
      PlaySound(sndBreakBrick);
      break;
    case SOUND_LOSE_LIFE:
      PlaySound(sndLoseLife);
      break;
    case SOUND_GAME_OVER:
      PlaySound(sndGameOver);
      break;
    case SOUND_WIN:
      PlaySound(sndWin);
      break;
  }
}

void AudioCleanup(void) {
  UnloadSound(sndHitPaddle);
  UnloadSound(sndHitWall);
  UnloadSound(sndBreakBrick);
  UnloadSound(sndLoseLife);
  UnloadSound(sndGameOver);
  UnloadSound(sndWin);

  CloseAudioDevice();
}