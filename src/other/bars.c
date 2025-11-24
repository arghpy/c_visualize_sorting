#include "./include/bars.h"
#include <stdio.h>

size_t BAR_LENGTH = 20;

void draw_bars(Bars *bs)
{
  int screen_height = GetScreenHeight();
  for (size_t i = 0; i < bs->count; i++) {
    DrawRectangle(i * BAR_LENGTH, screen_height - bs->items[i].height, BAR_LENGTH, bs->items[i].height, bs->items[i].color);
    DrawRectangleLines(i * BAR_LENGTH, screen_height - bs->items[i].height, BAR_LENGTH, bs->items[i].height, BLACK);
  }
}

void print_bars(Bars *bs)
{
  printf("----------------------------------------\n");
  for (size_t i = 0; i < bs->count; i++)
    printf("%ld: %f\n", i, bs->items[i].height);
  printf("----------------------------------------\n");
}
