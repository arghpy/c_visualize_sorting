#ifndef COCKTAIL_H
#define COCKTAIL_H

#include <stdbool.h>
#include <stddef.h>
#include "raylib.h"
#include "../../other/include/bars.h"

typedef struct {
  size_t lower_bound;
  size_t upper_bound;
  bool up;
  size_t index;
  bool started;
  char text[50];
  Color color;
} Cocktail_sort;

extern Cocktail_sort c_sort;

void reset_cocktail_sort(void);
void cocktail_sort(Bars *bs, bool *sorted);
#endif
