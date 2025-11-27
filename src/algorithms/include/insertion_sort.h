#ifndef INSERTION_H
#define INSERTION_H

#include "../../other/include/bars.h"
#include "raylib.h"

typedef struct {
  size_t k;
  size_t index;
  size_t current_index;
  bool started;
  char text[50];
  Color color;
} Insertion_sort;

extern Insertion_sort insertion_sort_state;
void insertion_sort(Bars *bs, bool *sorted);
void reset_insertion_sort(void);

#endif
