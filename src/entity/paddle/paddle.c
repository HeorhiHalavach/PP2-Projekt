#include "paddle.h"

/**
 * @brief Inicjalizuje paletkę gracza (ustawia rozmiar, pozycję, prędkość i
 * kolor). Paletka domyślnie pojawia się na środku, u dołu ekranu.
 * @param paddle Wskaźnik na strukturę paletki do zainicjalizowania.
 */
void PaddleInit(Paddle *paddle) {
  paddle->size = (Vector2){ 120.0f, 20.0f };  // Szerokość 120, wysokość 20
  paddle->position = (Vector2){ GetScreenWidth() / 2.0f - paddle->size.x / 2.0f,
                                GetScreenHeight() - 40.0f };
  paddle->speed = 8.0f;      
  paddle->color = DARKBLUE;  
}

/**
 * @brief Aktualizuje pozycję paletki na podstawie wejścia z klawiatury
 * (strzałki w lewo/prawo). Blokuje również paletkę przed wyjechaniem poza
 * krawędzie ekranu.
 * @param paddle Wskaźnik na strukturę paletki do zaktualizowania.
 */
void PaddleUpdate(Paddle *paddle) {
  if (IsKeyDown(KEY_LEFT)) paddle->position.x -= paddle->speed;
  if (IsKeyDown(KEY_RIGHT)) paddle->position.x += paddle->speed;

  if (paddle->position.x <= 0) {
    paddle->position.x = 0;
  }
  if (paddle->position.x + paddle->size.x >= GetScreenWidth()) {
    paddle->position.x = GetScreenWidth() - paddle->size.x;
  }
}

/**
 * @brief Rysuje paletkę na ekranie w aktualnej pozycji.
 * @param paddle Wskaźnik na strukturę paletki do narysowania.
 */
void PaddleDraw(Paddle *paddle) {
  DrawRectangleV(paddle->position, paddle->size, paddle->color);
}