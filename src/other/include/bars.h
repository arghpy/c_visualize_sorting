#ifndef BARS_H
#define BARS_H

#include <stddef.h>
#include "raylib.h"
#include "utils.h"

extern size_t BAR_LENGTH;

typedef struct {
  float height;
  Color color;
} Bar;

typedef ut_da_declare(Bar) Bars;

void draw_bars(Bars *bs);
void print_bars(Bars *bs);

#endif
