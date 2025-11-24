#include "./include/bubble_sort.h"

Bubble_sort b_sort = {
  .index = 0,
  .pass = 0,
  .started = false,
  .text = "[1] Bubble sort",
  .color = WHITE,
};

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
