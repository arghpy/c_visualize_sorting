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
#include "./algorithms/include/insertion_sort.h"
// #include "./algorithms/include/merge_sort.h"

#define FONT_SIZE 25
#define FONT_POSITION ((Vector2) {20, 20})

#define WINDOW_SCALE  100
#define SCREEN_WIDTH  (WINDOW_SCALE*16)
#define SCREEN_HEIGHT (WINDOW_SCALE*9)
#define WINDOW_COLOR  ((Color) {18, 18, 18, 0xFF})

// Interactive colors
Color default_color = WHITE;
Color fps_color = BLUE;
Color paused_color = WHITE;
Color show_menu_color = WHITE;

char fps_text[50] = {0};

typedef struct {
  char *text;
  Color *color;
} Text;

Text quick_menu[] = {
  (Text){ .text = "[F5] show more options",  .color = &show_menu_color },
  (Text){ .text = bubble_sort_state.text,    .color = &bubble_sort_state.color    },
  (Text){ .text = cocktail_sort_state.text,  .color = &cocktail_sort_state.color  },
  (Text){ .text = insertion_sort_state.text, .color = &insertion_sort_state.color },
};

Text other_options[] = {
  (Text){ .text = "[space] pause",               .color = &paused_color  },
  (Text){ .text = "[r] reset array",             .color = &default_color },
  (Text){ .text = "[left arrow] slow down",      .color = &default_color },
  (Text){ .text = "[right arrow] speed up",      .color = &default_color },
  (Text){ .text = "[up arrow] increase array",   .color = &default_color },
  (Text){ .text = "[down arrow] decrease array", .color = &default_color },
};

void draw_other_options(Font *font)
{
  // Blur
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(WHITE, 0.3f));

  // Draw rectangle
  size_t text_max_length_index = 0;
  float text_max_length = MeasureTextEx(*font, other_options[text_max_length_index].text, FONT_SIZE, 0).x;

  for (size_t i = 1; i < ARRAY_LEN(other_options); i++) {
    float text_length = MeasureTextEx(*font, other_options[i].text, FONT_SIZE, 0).x;
    if (text_length > text_max_length) {
      text_max_length = text_length;
      text_max_length_index = i;
    }
  }
  float padding = font->baseSize * 0.5f;
  Rectangle menu = {
    .x = GetScreenWidth()/2.0f - (text_max_length + padding*2)/2.0f,
    .y = GetScreenHeight()/2.0f - (ARRAY_LEN(other_options) * font->baseSize + padding*2)/2.0f,
    .width = text_max_length + padding*2,
    .height = ARRAY_LEN(other_options) * font->baseSize + padding*2,
  };
  DrawRectangleRec(menu, WINDOW_COLOR);
  DrawRectangleLinesEx(menu, 1.0f, GREEN);

  // Draw options
  for (size_t i = 0; i < ARRAY_LEN(other_options); i++) {
    Vector2 position = {
      .x = menu.x + padding,
      .y = menu.y + font->baseSize * i + padding,
    };
    DrawTextEx(*font, other_options[i].text, position, FONT_SIZE, 0, *other_options[i].color);
  }
}

void draw_quick_menu(Font *font)
{
  for (size_t i = 0; i < ARRAY_LEN(quick_menu); i++) {
    Vector2 position = {
      .x = FONT_POSITION.x,
      .y = FONT_POSITION.y + font->baseSize * i,
    };
    DrawTextEx(*font, quick_menu[i].text, position, FONT_SIZE, 0, *quick_menu[i].color);
  }
}

void reset_bars(Bars *bs, Font *font)
{
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();
  ut_da_reset(bs);
  for (size_t i = 0; i < screen_width/BAR_LENGTH; i++)
    ut_da_push(bs, ((Bar) {.height = rand()%((int)(screen_height - FONT_POSITION.y - font->baseSize * (int)ARRAY_LEN(quick_menu))), .color = WHITE}));
}

void reset_algorithms(void)
{
  reset_bubble_sort();
  reset_cocktail_sort();
  reset_insertion_sort();
}

int main(void)
{
  int FPS = 60;
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Visualize Sorting");
  SetTargetFPS(FPS);

  char *font_path = "./thirdparty/assets/fonts/JetBrainsMono-Regular.ttf";
  Font font = LoadFontEx(font_path, FONT_SIZE, NULL, 0);

  Bars bs = {0};
  srand(time(0));
  reset_bars(&bs, &font);

  size_t k = 0;
  bool paused = false;
  bool sorted = false;
  bool started_sorting = false;
  bool show_menu = false;

  while(!WindowShouldClose()) {

    if (IsKeyPressed(KEY_F5)) show_menu = !show_menu;

    if (IsKeyPressed(KEY_SPACE)) paused = !paused;
    show_menu_color = show_menu ? GREEN : WHITE;
    paused_color = paused ? RED : WHITE;

    if (IsKeyPressed(KEY_R)) {
      reset_bars(&bs, &font);
      k = 0;
      sorted = false;

      reset_algorithms();
      started_sorting = false;
      sorted = false;
    }

    if (IsKeyPressed(KEY_UP)) {
      if (BAR_LENGTH > 3)
        BAR_LENGTH -= 2;
      reset_bars(&bs, &font);
      k = 0;

      reset_algorithms();
      started_sorting = false;
      sorted = false;
    }

    if (IsKeyPressed(KEY_DOWN)) {
      BAR_LENGTH += 2;
      reset_bars(&bs, &font);
      k = 0;

      reset_algorithms();
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
    if (IsKeyPressed(KEY_THREE) && !started_sorting && !sorted) {
      insertion_sort_state.started = true;
      insertion_sort_state.color = GREEN;
      started_sorting = insertion_sort_state.started;
    }

    // Remake bars white when sorting is finished, each FPS
    if (sorted) {
      if (k >= bs.count) {
        k = 0;
      } else {
        bs.items[k].color = WHITE;
        k++;
      }
      reset_algorithms();
      started_sorting = false;
    } else {
      if (!paused) {
        if (bs.count < 2) continue;

        if (bubble_sort_state.started)
          bubble_sort(&bs, &sorted);

        if (cocktail_sort_state.started)
          cocktail_sort(&bs, &sorted);

        if (insertion_sort_state.started)
          insertion_sort(&bs, &sorted);
      }
    }

    BeginDrawing();
    {
      ClearBackground(WINDOW_COLOR);
      draw_quick_menu(&font);
      draw_bars(&bs);
      if (show_menu) draw_other_options(&font);
    }
    EndDrawing();
  }
  UnloadFont(font);
  CloseWindow();
  ut_da_free(&bs);
  return 0;
}
