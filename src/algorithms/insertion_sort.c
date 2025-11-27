#include "./include/insertion_sort.h"

Insertion_sort insertion_sort_state = {
  .k = 0,
  .index = 0,
  .current_index = 1,
  .started = false,
  .text = "[3] Insertion sort",
  .color = WHITE,
};

void reset_insertion_sort(void)
{
  insertion_sort_state.k = 0;
  insertion_sort_state.index = 0;
  insertion_sort_state.current_index = 1;
  insertion_sort_state.started = false;
  insertion_sort_state.color = WHITE;
}

void insertion_sort(Bars *bs, bool *sorted)
{
  if (bs->count < 2) return;

  if (insertion_sort_state.index > bs->count - 1) {
    if (insertion_sort_state.k < bs->count) {
      bs->items[insertion_sort_state.k].color = GREEN;
      insertion_sort_state.k++;
    }
    else {
      reset_insertion_sort();
      *sorted = true;
    }
  } else {
    size_t *j = &insertion_sort_state.current_index;
    if ((*j > 0) && bs->items[*j].height < bs->items[*j - 1].height) {
      SWAP(Bar, bs->items[*j], bs->items[*j - 1]);
      bs->items[*j - 1].color = GREEN;
      (*j)--;
    } else {
      bs->items[*j].color = WHITE;
      insertion_sort_state.index++;
      insertion_sort_state.current_index = insertion_sort_state.index;
    }
  }
}
