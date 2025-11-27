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

Text *algorithm_options[] = {
  &((Text){ .text = bubble_sort_state.text,   .color = &bubble_sort_state.color}),
  &((Text){ .text = cocktail_sort_state.text, .color = &cocktail_sort_state.color}),
};

Text *button_options[] = {
  &((Text){ .text = "[F5] show this menu",         .color = &default_color }),
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
  for (size_t i = 0; i < ARRAY_LEN(algorithm_options); i++)
    ut_da_push(m, *algorithm_options[i]);

  for (size_t i = 0; i < ARRAY_LEN(button_options); i++)
    ut_da_push(m, *button_options[i]);
}

void draw_options(Menu *m, Font *font, bool show_menu)
{
  size_t counter = show_menu ? m->count : ARRAY_LEN(algorithm_options);
  for (size_t i = 0; i < counter; i++) {
    Vector2 position = {
      .x = FONT_POSITION.x,
      .y = FONT_POSITION.y + font->baseSize * i,
    };
    DrawTextEx(*font, m->items[i].text, position, FONT_SIZE, 0, *m->items[i].color);
  }
}

void reset_bars(Bars *bs, Menu *m, Font *font)
{
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();
  ut_da_reset(bs);
  for (size_t i = 0; i < screen_width/BAR_LENGTH; i++)
    ut_da_push(bs, ((Bar) {.height = rand()%((int)(screen_height - FONT_POSITION.y - font->baseSize * m->count)), .color = WHITE}));
}

int main(void)
{
  int FPS = 60;
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Visualize Sorting");
  SetTargetFPS(FPS);

  char *font_path = "./thirdparty/assets/fonts/JetBrainsMono-Regular.ttf";
  Font font = LoadFontEx(font_path, FONT_SIZE, NULL, 0);

  populate_options(&menu);

  Bars bs = {0};
  srand(time(0));
  reset_bars(&bs, &menu, &font);

  size_t k = 0;
  bool paused = false;
  bool sorted = false;
  bool started_sorting = false;
  bool show_menu = false;

  while(!WindowShouldClose()) {

    if (IsKeyPressed(KEY_F5)) show_menu = !show_menu;

    if (IsKeyPressed(KEY_SPACE)) paused = !paused;
    paused_color = paused ? RED: WHITE;

    if (IsKeyPressed(KEY_R)) {
      reset_bars(&bs, &menu, &font);
      k = 0;
      sorted = false;

      reset_bubble_sort();
      reset_cocktail_sort();
      started_sorting = false;
      sorted = false;
    }

    if (IsKeyPressed(KEY_UP)) {
      if (BAR_LENGTH > 3)
        BAR_LENGTH -= 2;
      reset_bars(&bs, &menu, &font);
      k = 0;
      reset_bubble_sort();
      reset_cocktail_sort();
      started_sorting = false;
      sorted = false;
    }

    if (IsKeyPressed(KEY_DOWN)) {
      BAR_LENGTH += 2;
      reset_bars(&bs, &menu, &font);
      k = 0;

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

    if (IsKeyPressed(KEY_ONE) && !started_sorting && !sorted) {
      bubble_sort_state.started = true;
      bubble_sort_state.color = GREEN;
      started_sorting = bubble_sort_state.started;
    }
    if (IsKeyPressed(KEY_TWO) && !started_sorting && !sorted) {
      cocktail_sort_state.started = true;
      cocktail_sort_state.color = GREEN;
      started_sorting = cocktail_sort_state.started;
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

        if (bubble_sort_state.started)
          bubble_sort(&bs, &sorted);

        if (cocktail_sort_state.started)
          cocktail_sort(&bs, &sorted);
      }
    }

    BeginDrawing();
    {
      ClearBackground(WINDOW_COLOR);
      draw_options(&menu, &font, show_menu);
      draw_bars(&bs);
    }
    EndDrawing();
  }
  UnloadFont(font);
  CloseWindow();
  ut_da_free(&bs);
  return 0;
}
