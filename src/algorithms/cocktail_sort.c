#include "./include/cocktail_sort.h"

Cocktail_sort cocktail_sort_state = {
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
  cocktail_sort_state.lower_bound = 0;
  cocktail_sort_state.upper_bound = 0;
  cocktail_sort_state.up = true;
  cocktail_sort_state.index = 0;
  cocktail_sort_state.started = false;
  cocktail_sort_state.color = WHITE;
}

void cocktail_sort(Bars *bs, bool *sorted)
{
  if (cocktail_sort_state.lower_bound == 0 && cocktail_sort_state.upper_bound == 0) {
    cocktail_sort_state.lower_bound = 0;
    cocktail_sort_state.upper_bound = bs->count - 1;
    cocktail_sort_state.index = cocktail_sort_state.lower_bound;
  }

  if (cocktail_sort_state.lower_bound < cocktail_sort_state.upper_bound) {
    // Up
    if (cocktail_sort_state.up) {
      if (cocktail_sort_state.index < cocktail_sort_state.upper_bound) {
        if (bs->items[cocktail_sort_state.index].height > bs->items[cocktail_sort_state.index + 1].height) {
          SWAP(Bar, bs->items[cocktail_sort_state.index], bs->items[cocktail_sort_state.index + 1]);
          bs->items[cocktail_sort_state.index + 1].color = GREEN;
        } else {
          bs->items[cocktail_sort_state.index].color = WHITE;
          bs->items[cocktail_sort_state.index + 1].color = GREEN;
        }
        (cocktail_sort_state.index)++;
        return;
      } else {
        cocktail_sort_state.upper_bound--;
        cocktail_sort_state.index = cocktail_sort_state.upper_bound;
        cocktail_sort_state.up = false;
      }
    } else {
      // Down
      if (cocktail_sort_state.index > cocktail_sort_state.lower_bound) {
        if (bs->items[cocktail_sort_state.index].height < bs->items[cocktail_sort_state.index - 1].height) {
          SWAP(Bar, bs->items[cocktail_sort_state.index], bs->items[cocktail_sort_state.index - 1]);
          bs->items[cocktail_sort_state.index - 1].color = GREEN;
        } else {
          bs->items[cocktail_sort_state.index].color = WHITE;
          bs->items[cocktail_sort_state.index - 1].color = GREEN;
        }
        (cocktail_sort_state.index)--;
        return;
      } else {
        cocktail_sort_state.lower_bound++;
        cocktail_sort_state.index = cocktail_sort_state.lower_bound;
        cocktail_sort_state.up = true;
      }
    }
  } else {
    bs->items[cocktail_sort_state.index].color = GREEN;
    reset_cocktail_sort();
    *sorted = true;
    return;
  }
}
