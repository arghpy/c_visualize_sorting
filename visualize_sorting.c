#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define UT_IMPLEMENTATION
#include <utils.h>

#include <raylib.h>

#define WINDOW_SCALE 100
#define SCREEN_WIDTH (WINDOW_SCALE*16)
#define SCREEN_HEIGHT (WINDOW_SCALE*9)
#define WINDOW_COLOR ((Color) {18, 18, 18, 0xFF})

#define BAR_LENGTH 20

typedef struct {
  float height;
  Color color;
} Bar;

typedef ut_da_declare(Bar) Bars;

void draw_bars(Bars *bs)
{
  for (size_t i = 0; i < bs->count; i++) {
    DrawRectangle(i * BAR_LENGTH, SCREEN_HEIGHT - bs->items[i].height, BAR_LENGTH, bs->items[i].height , bs->items[i].color);
    DrawRectangleLines(i * BAR_LENGTH, SCREEN_HEIGHT - bs->items[i].height, BAR_LENGTH, bs->items[i].height , BLACK);
  }
}

void reset_bars(Bars *bs)
{
  ut_da_reset(bs);
  for (size_t i = 0; i < SCREEN_WIDTH/BAR_LENGTH; i++)
    ut_da_push(bs, ((Bar) {.height = rand()%SCREEN_HEIGHT, .color = WHITE}));
}

int main(void)
{
  Bars bs = {0};

  size_t i = 0;
  size_t j = 0;
  size_t k = 0;

  srand(time(0));
  reset_bars(&bs);

  bool paused = false;
  bool sorted = false;

  SetTraceLogLevel(LOG_ERROR);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Visualize Sorting");
  SetTargetFPS(240);

  // asm("int3");
  while(!WindowShouldClose()) {

    if (IsKeyPressed(KEY_R)){
      reset_bars(&bs);
      i = 0;
      j = 0;
      k = 0;
      sorted = false;
    }
    if (IsKeyPressed(KEY_SPACE)) paused = !paused;
    BeginDrawing();
    {
      ClearBackground(WINDOW_COLOR);

      // Remake bars white when sorting is finished, each FPS
      if (sorted) {
        if (k >= bs.count) {
          k = 0;
        } else {
          bs.items[k].color = WHITE;
          k++;
        }
      }

      if (!sorted) {
        if (!paused) {
          if (bs.count > 1) {
            if (j >= bs.count - 1) {
              j = 0;
              sorted = true;
            }
            if (i < bs.count - 1 - j) {
              if (bs.items[i].height > bs.items[i + 1].height) {
                SWAP(Bar, bs.items[i], bs.items[i + 1]);
                bs.items[i + 1].color = GREEN;
              } else bs.items[i].color = WHITE;
              i++;
            } else {
              bs.items[i].color = GREEN;
              i = 0;
              j++;
            }
          }
        }
      }
      draw_bars(&bs);
    }
    EndDrawing();
  }
  CloseWindow();
  ut_da_free(&bs);
  return 0;
}
