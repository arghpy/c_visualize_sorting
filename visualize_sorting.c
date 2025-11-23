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

void reset_cocktail_sort(void)
{
  c_sort.lower_bound = 0;
  c_sort.upper_bound = 0;
  c_sort.up = true;
  c_sort.index = 0;
  c_sort.started = false;
  c_sort.color = WHITE;

  started_sorting = c_sort.started;
}

void cocktail_sort(Bars *bs)
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
    sorted = true;
    return;
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
