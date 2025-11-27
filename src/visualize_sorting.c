#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define UT_IMPLEMENTATION
#include "utils.h"

#include "raylib.h"
#include "./other/include/bars.h"
#include "./algorithms/include/bubble_sort.h"
#include "./algorithms/include/cocktail_sort.h"
// #include "./algorithms/include/merge_sort.h"

#define FONT_SIZE 25
#define FONT_POSITION ((Vector2) {20, 20})

#define WINDOW_SCALE 100
#define SCREEN_WIDTH (WINDOW_SCALE*16)
#define SCREEN_HEIGHT (WINDOW_SCALE*9)
#define WINDOW_COLOR ((Color) {18, 18, 18, 0xFF})

// Interactive colors
Color default_color = WHITE;
Color fps_color = BLUE;
Color paused_color = WHITE;

char fps_text[50] = {0};

typedef struct {
  char *text;
  Color *color;
} Text;

Text *button_options[] = {
  &((Text){ .text = b_sort.text,                   .color = &b_sort.color}),
  &((Text){ .text = c_sort.text,                   .color = &c_sort.color}),
  &((Text){ .text = "[space] pause",               .color = &paused_color }),
  &((Text){ .text = "[r] reset array",             .color = &default_color }),
  &((Text){ .text = "[left arrow] slow down",      .color = &default_color }),
  &((Text){ .text = "[right arrow] speed up",      .color = &default_color }),
  &((Text){ .text = "[up arrow] increase array",   .color = &default_color }),
  &((Text){ .text = "[down arrow] decrease array", .color = &default_color }),
};

typedef ut_da_declare(Text) Menu;
Menu menu = {0};

void populate_options(Menu *m)
{
  int screen_width = GetScreenWidth();
  // First is always FPS
  snprintf(fps_text, ARRAY_LEN(fps_text), "FPS %d, Elements: %ld", GetFPS(), screen_width/BAR_LENGTH);
  ut_da_push(m, ((Text) {.text = fps_text, .color = &fps_color}));

  for (size_t i = 0; i < ARRAY_LEN(button_options); i++) {
    ut_da_push(m, *button_options[i]);
  }
}

void draw_options(Menu *m)
{
  for (size_t i = 0; i < m->count; i++)
    DrawText(m->items[i].text, FONT_POSITION.x, FONT_POSITION.y + FONT_SIZE * i, FONT_SIZE, *m->items[i].color);
}

void reset_bars(Bars *bs, Menu *m)
{
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();
  ut_da_reset(bs);
  for (size_t i = 0; i < screen_width/BAR_LENGTH; i++)
    ut_da_push(bs, ((Bar) {.height = rand()%((int)(screen_height - FONT_POSITION.y - FONT_SIZE * m->count)), .color = WHITE}));
}

int main(void)
{
  int FPS = 60;
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Visualize Sorting");
  SetTargetFPS(FPS);

  populate_options(&menu);

  Bars bs = {0};
  srand(time(0));
  reset_bars(&bs, &menu);

  size_t k = 0;
  bool paused = false;
  bool sorted = false;
  bool started_sorting = false;

  while(!WindowShouldClose()) {

    paused_color = paused ? RED: WHITE;

    if (IsKeyPressed(KEY_UP)) {
      if (BAR_LENGTH > 3)
        BAR_LENGTH -= 2;
      reset_bars(&bs, &menu);
      k = 0;
      reset_bubble_sort();
      reset_cocktail_sort();
      started_sorting = false;
      sorted = false;
    }

    if (IsKeyPressed(KEY_DOWN)) {
      BAR_LENGTH += 2;
      reset_bars(&bs, &menu);
      k = 0;

      reset_bubble_sort();
      reset_cocktail_sort();
      started_sorting = false;
      sorted = false;
    }

    if (IsKeyPressed(KEY_R)) {
      reset_bars(&bs, &menu);
      k = 0;
      sorted = false;

      reset_bubble_sort();
      reset_cocktail_sort();
      started_sorting = false;
      sorted = false;
    }

    if (IsKeyPressed(KEY_LEFT)) {
      if (FPS > 20) {
        FPS -= 10;
        SetTargetFPS(FPS);
      }
    }

    if (IsKeyPressed(KEY_RIGHT)) {
      FPS += 10;
      SetTargetFPS(FPS);
    }

    if (IsKeyPressed(KEY_SPACE)) paused = !paused;

    if (IsKeyPressed(KEY_ONE) && !started_sorting && !sorted) {
      b_sort.started = true;
      b_sort.color = GREEN;
      started_sorting = b_sort.started;
    }
    if (IsKeyPressed(KEY_TWO) && !started_sorting && !sorted) {
      c_sort.started = true;
      c_sort.color = GREEN;
      started_sorting = c_sort.started;
    }

    // Remake bars white when sorting is finished, each FPS
    if (sorted) {
      if (k >= bs.count) {
        k = 0;
      } else {
        bs.items[k].color = WHITE;
        k++;
      }
      reset_bubble_sort();
      reset_cocktail_sort();
      started_sorting = false;
    } else {
      if (!paused) {
        if (bs.count < 2) continue;

        if (b_sort.started)
          bubble_sort(&bs, &sorted);

        if (c_sort.started)
          cocktail_sort(&bs, &sorted);
      }
    }

    BeginDrawing();
    {
      ClearBackground(WINDOW_COLOR);

      int screen_width = GetScreenWidth();
      // Text
      sprintf(menu.items[0].text, "FPS %d, Elements: %ld", GetFPS(), screen_width/BAR_LENGTH);
      draw_options(&menu);

      draw_bars(&bs);
    }
    EndDrawing();
  }
  CloseWindow();
  ut_da_free(&bs);
  return 0;
}
