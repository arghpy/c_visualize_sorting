#ifndef BUBBLE_H
#define BUBBLE_H

#include <stdbool.h>
#include <stddef.h>
#include "raylib.h"
#include "../../other/include/bars.h"

typedef struct {
  size_t index;
  size_t pass;
  bool started;
  char text[50];
  Color color;
} Bubble_sort;

extern Bubble_sort bubble_sort_state;

void reset_bubble_sort(void);
void bubble_sort(Bars *bs, bool *sorted);

#endif
