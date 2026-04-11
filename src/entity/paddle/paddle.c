#include "paddle.h"

/**
 * @brief Wewnętrzna, ukryta struktura przechowująca stan paletki.
 */
static struct {
  Vector2 position;
  Vector2 size;
  float speed;
  Color color;
} paddle = { 0 };

/** * @brief Inicjalizuje paletkę gracza (ustawia rozmiar, pozycję, prędkość i
 * kolor). Paletka domyślnie pojawia się na środku, u dołu ekranu.
 */
void PaddleInit(void) {
  paddle.size = (Vector2){ 120.0f, 20.0f };
  paddle.speed = 8.0f;
  paddle.color = DARKBLUE;
  paddle.position = (Vector2){ GetScreenWidth() / 2.0f - paddle.size.x / 2.0f,
                               GetScreenHeight() - 40.0f };
}

/**
 * @brief Ustawia nową konfigurację paletki (rozmiar, prędkość, kolor).
 * @param config Struktura zawierająca nowe ustawienia paletki.
 */
void PaddleSetConfig(PaddleConfig config) {
  paddle.size = config.size;
  paddle.speed = config.speed;
  paddle.color = config.color;
}

/** * @brief Aktualizuje pozycję paletki na podstawie wejścia z klawiatury
 * (strzałki w lewo/prawo). Blokuje również paletkę przed wyjechaniem poza
 * krawędzie ekranu.
 */
void PaddleUpdate(void) {
  if (IsKeyDown(KEY_LEFT)) paddle.position.x -= paddle.speed;
  if (IsKeyDown(KEY_RIGHT)) paddle.position.x += paddle.speed;

  if (paddle.position.x <= 0) {
    paddle.position.x = 0;
  }
  if (paddle.position.x + paddle.size.x >= GetScreenWidth()) {
    paddle.position.x = GetScreenWidth() - paddle.size.x;
  }
}

/** * @brief Rysuje paletkę na ekranie w aktualnej pozycji.
 */
void PaddleDraw(void) {
  DrawRectangleV(paddle.position, paddle.size, paddle.color);
}

/**
 * @brief Zwraca prostokąt kolizji (hitbox) paletki.
 * @return Rectangle Obiekt prostokąta reprezentujący pozycję i wymiary paletki.
 */
Rectangle PaddleGetRect(void) {
  return (Rectangle){ paddle.position.x, paddle.position.y, paddle.size.x,
                      paddle.size.y };
}