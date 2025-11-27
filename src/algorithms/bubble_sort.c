#include "./include/bubble_sort.h"

Bubble_sort bubble_sort_state = {
  .index = 0,
  .pass = 0,
  .started = false,
  .text = "[1] Bubble sort",
  .color = WHITE,
};

void reset_bubble_sort(void)
{
  bubble_sort_state.index = 0;
  bubble_sort_state.pass = 0;
  bubble_sort_state.started = false;
  bubble_sort_state.color = WHITE;
}

void bubble_sort(Bars *bs, bool *sorted)
{
  if (bubble_sort_state.pass >= bs->count - 1) {
    reset_bubble_sort();
    *sorted = true;
    return;
  }
  if (bubble_sort_state.index < bs->count - 1 - bubble_sort_state.pass) {
    if (bs->items[bubble_sort_state.index].height > bs->items[bubble_sort_state.index + 1].height) {
      SWAP(Bar, bs->items[bubble_sort_state.index], bs->items[bubble_sort_state.index + 1]);
      bs->items[bubble_sort_state.index + 1].color = GREEN;
    } else {
      bs->items[bubble_sort_state.index].color = WHITE;
      bs->items[bubble_sort_state.index + 1].color = GREEN;
    }

    (bubble_sort_state.index)++;
  } else {
    bs->items[bubble_sort_state.index].color = GREEN;
    bubble_sort_state.index = 0;
    (bubble_sort_state.pass)++;
  }
}
