#include "./include/cocktail_sort.h"

Cocktail_sort c_sort = {
  .lower_bound = 0,
  .upper_bound = 0,
  .up = true,
  .index = 0,
  .started = false,
  .text = "[2] Cocktail sort",
  .color = WHITE,
};

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
