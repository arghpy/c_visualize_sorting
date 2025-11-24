#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define UT_IMPLEMENTATION
#include "utils.h"

#include "raylib.h"

#define FONT_SIZE 20
#define FONT_POSITION ((Vector2) {20, 20})

#define WINDOW_SCALE 100
#define SCREEN_WIDTH (WINDOW_SCALE*16)
#define SCREEN_HEIGHT (WINDOW_SCALE*9)
#define WINDOW_COLOR ((Color) {18, 18, 18, 0xFF})

size_t BAR_LENGTH = 20;

// Interactive colors
Color default_color = WHITE;
Color fps_color = BLUE;
Color paused_color = WHITE;

typedef struct {
  size_t index;
  size_t pass;
  bool started;
  char text[50];
  Color color;
} Bubble_sort;

Bubble_sort b_sort = {
  .index = 0,
  .pass = 0,
  .started = false,
  .text = "[1] Bubble sort",
  .color = WHITE,
};

typedef struct {
  size_t lower_bound;
  size_t upper_bound;
  bool up;
  size_t index;
  bool started;
  char text[50];
  Color color;
} Cocktail_sort;

Cocktail_sort c_sort = {
  .lower_bound = 0,
  .upper_bound = 0,
  .up = true,
  .index = 0,
  .started = false,
  .text = "[2] Cocktail sort",
  .color = WHITE,
};

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

typedef struct {
  float height;
  Color color;
} Bar;

typedef ut_da_declare(Bar) Bars;

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

void draw_bars(Bars *bs)
{
  int screen_height = GetScreenHeight();
  for (size_t i = 0; i < bs->count; i++) {
    DrawRectangle(i * BAR_LENGTH, screen_height - bs->items[i].height, BAR_LENGTH, bs->items[i].height, bs->items[i].color);
    DrawRectangleLines(i * BAR_LENGTH, screen_height - bs->items[i].height, BAR_LENGTH, bs->items[i].height, BLACK);
  }
}

void reset_bars(Bars *bs, Menu *m)
{
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();
  ut_da_reset(bs);
  for (size_t i = 0; i < screen_width/BAR_LENGTH; i++)
    ut_da_push(bs, ((Bar) {.height = rand()%((int)(screen_height - FONT_POSITION.y - FONT_SIZE * m->count)), .color = WHITE}));
}

void reset_bubble_sort(void)
{
  b_sort.index = 0;
  b_sort.pass = 0;
  b_sort.started = false;
  b_sort.color = WHITE;
}

void bubble_sort(Bars *bs, bool *sorted)
{
  if (b_sort.pass >= bs->count - 1) {
    reset_bubble_sort();
    *sorted = true;
    return;
  }
  if (b_sort.index < bs->count - 1 - b_sort.pass) {
    if (bs->items[b_sort.index].height > bs->items[b_sort.index + 1].height) {
      SWAP(Bar, bs->items[b_sort.index], bs->items[b_sort.index + 1]);
      bs->items[b_sort.index + 1].color = GREEN;
    } else {
      bs->items[b_sort.index].color = WHITE;
      bs->items[b_sort.index + 1].color = GREEN;
    }

    (b_sort.index)++;
  } else {
    bs->items[b_sort.index].color = GREEN;
    b_sort.index = 0;
    (b_sort.pass)++;
  }
}

Bars merge(Bars *left, Bars *right)
{
  Bars merged = {0};
  size_t i = 0;
  size_t j = 0;

  while (i < left->count && j < right->count) {
    if (left->items[i].height < right->items[j].height) {
      ut_da_push(&merged, left->items[i]);
      i++;
    } else {
      ut_da_push(&merged, right->items[j]);
      j++;
    }
  }

  // Add what was left
  for (size_t k = i; k < left->count; k++)
    ut_da_push(&merged, left->items[k]);

  // Add what was right
  for (size_t k = j; k < right->count; k++)
    ut_da_push(&merged, right->items[k]);

  ut_da_free(left);
  ut_da_free(right);

  return merged;
}

// This I don't know yet how to visualize
void merge_sort(Bars *bs)
{
  if (bs->count == 1) return;

  size_t middle = bs->count / 2;

  // Left
  Bars l = {0};
  for (size_t i = 0; i < middle; i++)
    ut_da_push(&l, bs->items[i]);

  merge_sort(&l);

  // Right
  Bars r = {0};
  for (size_t i = middle; i < bs->count; i++)
    ut_da_push(&r, bs->items[i]);

  merge_sort(&r);

  Bars tmp = merge(&l, &r);
  ut_da_copy(bs, &tmp);
  ut_da_free(&tmp);
}

void print_bs(Bars *bs)
{
  printf("----------------------------------------\n");
  for (size_t i = 0; i < bs->count; i++)
    printf("%ld: %f\n", i, bs->items[i].height);
  printf("----------------------------------------\n");
}

void reset_cocktail_sort(void)
{
  c_sort.lower_bound = 0;
  c_sort.upper_bound = 0;
  c_sort.up = true;
  c_sort.index = 0;
  c_sort.started = false;
  c_sort.color = WHITE;
}

void cocktail_sort(Bars *bs, bool *sorted)
{
  if (c_sort.lower_bound == 0 && c_sort.upper_bound == 0) {
    c_sort.lower_bound = 0;
    c_sort.upper_bound = bs->count - 1;
    c_sort.index = c_sort.lower_bound;
  }

  if (c_sort.lower_bound < c_sort.upper_bound) {
    // Up
    if (c_sort.up) {
      if (c_sort.index < c_sort.upper_bound) {
        if (bs->items[c_sort.index].height > bs->items[c_sort.index + 1].height) {
          SWAP(Bar, bs->items[c_sort.index], bs->items[c_sort.index + 1]);
          bs->items[c_sort.index + 1].color = GREEN;
        } else {
          bs->items[c_sort.index].color = WHITE;
          bs->items[c_sort.index + 1].color = GREEN;
        }
        (c_sort.index)++;
        return;
      } else {
        c_sort.upper_bound--;
        c_sort.index = c_sort.upper_bound;
        c_sort.up = false;
      }
    } else {
      // Down
      if (c_sort.index > c_sort.lower_bound) {
        if (bs->items[c_sort.index].height < bs->items[c_sort.index - 1].height) {
          SWAP(Bar, bs->items[c_sort.index], bs->items[c_sort.index - 1]);
          bs->items[c_sort.index - 1].color = GREEN;
        } else {
          bs->items[c_sort.index].color = WHITE;
          bs->items[c_sort.index - 1].color = GREEN;
        }
        (c_sort.index)--;
        return;
      } else {
        c_sort.lower_bound++;
        c_sort.index = c_sort.lower_bound;
        c_sort.up = true;
      }
    }
  } else {
    bs->items[c_sort.index].color = GREEN;
    reset_cocktail_sort();
    *sorted = true;
    return;
  }
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
