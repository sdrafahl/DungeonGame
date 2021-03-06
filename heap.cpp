#include <stdlib.h>
#include <string.h>

#include "heap.h"

#define BINHEAP_START_SIZE 128

struct binheap_node {
  void *datum;
  uint32_t index;
};

static void percolate_up(binheap_t *h, uint32_t index)
{
  uint32_t parent;
  binheap_node_t *tmp;

  for (parent = (index - 1) / 2;
       index && h->compare(h->array[index]->datum, h->array[parent]->datum) < 0;
       index = parent, parent = (index - 1) / 2) {
    tmp = h->array[index];
    h->array[index] = h->array[parent];
    h->array[parent] = tmp;
    h->array[index]->index = index;
    h->array[parent]->index = parent;
  } 
}

static void percolate_down(binheap_t *h, uint32_t index)
{
  uint32_t child;
  void *tmp;

  for (child = (2 * index) + 1;
       child < h->size;
       index = child, child = (2 * index) + 1) {
    if (child + 1 < h->size &&
        h->compare(h->array[child]->datum, h->array[child + 1]->datum) > 0) {
      child++;
    }
    if (h->compare(h->array[index]->datum, h->array[child]->datum) > 0) {
      tmp = h->array[index];
      h->array[index] = h->array[child];
      h->array[child] = (binheap_node_t*)tmp;
      h->array[index]->index = index;
      h->array[child]->index = child;
    }
  }
}

static void heapify(binheap_t *h)
{
  uint32_t i;

  for (i = (h->size + 1) / 2; i; i--) {
    percolate_down(h, i);
  }
  percolate_down(h, 0);
}

void binheap_init(binheap_t *h,
                  int32_t (*compare)(const void *key, const void *with),
                  void (*datum_delete)(void *))
{
  h->size = 0;
  h->array_size = BINHEAP_START_SIZE;
  h->compare = compare;
  h->datum_delete = datum_delete;

  h->array = (binheap_node_t**) calloc(h->array_size, sizeof (*h->array));
}

void binheap_init_from_array(binheap_t *h,
                             void *array,
                             uint32_t size,
                             uint32_t nmemb,
                             int32_t (*compare)(const void *key,
                                                const void *with),
                             void (*datum_delete)(void *))
{
  uint32_t i;
  char *a;

  h->size = h->array_size = nmemb;
  h->compare = compare;
  h->datum_delete = datum_delete;

  h->array = (binheap_node_t**) calloc(h->array_size, sizeof (*h->array));

  for (i = 0, a =(char*) array; i < h->size; i++) {
    h->array[i] =(binheap_node_t*) malloc(sizeof (*h->array[i]));
    h->array[i]->index = i;
    h->array[i]->datum = a + (i * size);
  }

  heapify(h);
}

void binheap_delete(binheap_t *h)
{
  uint32_t i;

  for (i = 0; i < h->size; i++) {
    if (h->datum_delete) {
      h->datum_delete(h->array[i]->datum);
    }
    free(h->array[i]);
  }
  free(h->array);
  memset(h, 0, sizeof (*h));
}

binheap_node_t *binheap_insert(binheap_t *h, void *v)
{
  binheap_node_t **tmp;
  binheap_node_t *retval;

  if (h->size == h->array_size) {
    h->array_size *= 2;
    tmp = (binheap_node_t**)realloc(h->array, h->array_size * sizeof (*h->array));
    if (!tmp) {
      /* Error */
    } else {
      h->array = tmp;
    }
  }

  h->array[h->size] = retval = (binheap_node_t*) malloc(sizeof (*h->array[h->size]));
  h->array[h->size]->datum = v;
  h->array[h->size]->index = h->size;

  percolate_up(h, h->size);
  h->size++;

  return retval;
}

void *binheap_peek_min(binheap_t *h)
{
  return h->size ? h->array[0]->datum : NULL;
}

void *binheap_remove_min(binheap_t *h)
{
  void *tmp;

  if (!h->size) {
    return NULL;
  }

  tmp = h->array[0]->datum;
  free(h->array[0]);
  h->size--;
  h->array[0] = h->array[h->size];
  percolate_down(h, 0);

  return tmp;
}

void binheap_decrease_key(binheap_t *h, binheap_node_t *n)
{
  percolate_up(h, n->index);
}

uint32_t binheap_is_empty(binheap_t *h)
{
  //return h->size;
  return !h->size;
}

#ifdef TESTING

#include <stdio.h>

int32_t compare_int(const void *key, const void *with)
{
  return *(const int32_t *) key - *(const int32_t *) with;
}
int32_t compare_cell(const void *key,const void *with){
  return *(const distanceCell *) key->distance - *(const distanceCell *) with->distance;
}
int32_t compare_monster(const void *key,const void *with){
  return (*(const Monster *) key).roundVal - (*(const Monster *) with).roundVal;
}

#endif