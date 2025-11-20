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

bool sorted = false;

typedef struct {
  size_t pass;
  size_t index;
} Bubble_sort;

Bubble_sort b_sort = {0};

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

void bubble_sort(Bars *bs)
{
  if (b_sort.pass >= bs->count - 1) {
    b_sort.pass = 0;
    sorted = true;
    return;
  }
  if (b_sort.index < bs->count - 1 - b_sort.pass) {
    if (bs->items[b_sort.index].height > bs->items[b_sort.index + 1].height) {
      SWAP(Bar, bs->items[b_sort.index], bs->items[b_sort.index + 1]);
      bs->items[b_sort.index + 1].color = GREEN;
    } else bs->items[b_sort.index].color = WHITE;

    (b_sort.index)++;
  } else {
    bs->items[b_sort.index].color = GREEN;
    b_sort.index = 0;
    (b_sort.pass)++;
  }
}

int main(void)
{
  Bars bs = {0};
  srand(time(0));
  reset_bars(&bs);

  size_t k = 0;
  bool paused = false;

  SetTraceLogLevel(LOG_ERROR);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Visualize Sorting");
  SetTargetFPS(240);

  while(!WindowShouldClose()) {

    if (IsKeyPressed(KEY_R)) {
      reset_bars(&bs);
      k = 0;
      sorted = false;

      // Bubble sort
      {
        b_sort.index = 0;
        b_sort.pass = 0;
      }
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
      } else {
        if (!paused) {
          if (bs.count < 2) continue;
          bubble_sort(&bs);
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
