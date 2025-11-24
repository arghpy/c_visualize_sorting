#include "./include/merge_sort.h"

Bars merge(Bars *left, Bars *right)
{
  Bars merged = {0};
  size_t i = 0;
  size_t j = 0;

  while (i < left->count && j < right->count) {
    if (left->items[i].height < right->items[j].height) {
      ut_da_push(&merged, left->items[i]);
      i++;
    } else {
      ut_da_push(&merged, right->items[j]);
      j++;
    }
  }

  // Add what was left
  for (size_t k = i; k < left->count; k++)
    ut_da_push(&merged, left->items[k]);

  // Add what was right
  for (size_t k = j; k < right->count; k++)
    ut_da_push(&merged, right->items[k]);

  ut_da_free(left);
  ut_da_free(right);

  return merged;
}

// This I don't know yet how to visualize
void merge_sort(Bars *bs)
{
  if (bs->count == 1) return;

  size_t middle = bs->count / 2;

  // Left
  Bars l = {0};
  for (size_t i = 0; i < middle; i++)
    ut_da_push(&l, bs->items[i]);

  merge_sort(&l);

  // Right
  Bars r = {0};
  for (size_t i = middle; i < bs->count; i++)
    ut_da_push(&r, bs->items[i]);

  merge_sort(&r);

  Bars tmp = merge(&l, &r);
  ut_da_copy(bs, &tmp);
  ut_da_free(&tmp);
}
