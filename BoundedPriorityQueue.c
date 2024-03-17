#include "BoundedPriorityQueue.h"
#include <stdlib.h>
#include <math.h>

/**
 * @author Lei Yang
 * @author Simon Gardier
*/

typedef struct Element_t
{
  double key;
  size_t value;
} Element;

struct bounded_priority_queue_t
{
  size_t actualsize;
  size_t capacity;
  Element *elements;
};
/**
 * @fn swap
 * @brief Swap the value between 2 arrays
 * @param bqp The binary heap with Elements contains a pairs of key/value
 * @param i the first position in the arrays
 * @param j the second position in the arrays
 *
 */
static void swap(BoundedPriorityQueue *bpq, unsigned int i, unsigned int j)
{
  // For key the change
  double key = bpq->elements[i].key;
  bpq->elements[i].key = bpq->elements[j].key;
  bpq->elements[j].key = key;
  // For value the change
  size_t value = bpq->elements[i].value;
  bpq->elements[i].value = bpq->elements[j].value;
  bpq->elements[j].value = value;
}
/**
 * @fn maxHeapify
 * @brief Rebuild correclty the heap
 * @param bpq The binary heap
 * @param root the start of rebuild
 * @param heapSize the size of the heap
 */
static void maxHeapify(BoundedPriorityQueue *bpq, unsigned int root, unsigned int heapSize)
{
  unsigned int left = 2 * root + 1;
  unsigned int right = 2 * root + 2;
  unsigned int largest = root;

  if (left < heapSize && bpq->elements[left].key > bpq->elements[largest].key)
  {
    largest = left;
  }
  if (right < heapSize && bpq->elements[right].key > bpq->elements[largest].key)
  {
    largest = right;
  }
  if (largest != root)
  {
    swap(bpq, root, largest);
    maxHeapify(bpq, largest, heapSize);
  }
}

/**
 * @fn Heap
 * @brief Insert the elements in the heap
 * @param bpq The binary heap
 * @param position Where to insert in the heap
 * @param key the key
 * @param value The data we need to have
 */
static void Heap(BoundedPriorityQueue *bpq, size_t position, double key, size_t value)
{
  bpq->elements[position].key = key;
  bpq->elements[position].value = value;
  unsigned int i = position;
  while (i > 0 && bpq->elements[i / 2].key < bpq->elements[i].key)
  {
    swap(bpq, i, i / 2);
    i = i / 2;
  }
}
BoundedPriorityQueue *bpqCreate(size_t capacity)
{
  BoundedPriorityQueue *Priority = malloc(sizeof(BoundedPriorityQueue));
  if (!Priority)
    return NULL;
  Priority->capacity = capacity;
  Priority->actualsize = 0;
  Priority->elements = malloc(capacity * sizeof(Element));
  if (!Priority->elements)
  {
    free(Priority);
    return NULL;
  }
  return Priority;
}

void bpqFree(BoundedPriorityQueue *bpq)
{
  if (bpq)
    free(bpq->elements);
}

bool bpqInsert(BoundedPriorityQueue *bpq, double key, size_t value)
{
  if (bpq->actualsize == bpq->capacity)
    return false;
  Heap(bpq, bpq->actualsize, key, value);
  bpq->actualsize++;
  return true;
}

void bpqReplaceMaximum(BoundedPriorityQueue *bpq, double key, size_t value)
{
  if (bpq->actualsize > 0 && key > 0)
  {
    bpq->elements[0].key = key;
    bpq->elements[0].value = value;
    maxHeapify(bpq, 0, bpq->actualsize);
  }
}

double bpqMaximumKey(const BoundedPriorityQueue *bpq)
{
  if (bpq->actualsize > 0)
    return bpq->elements[0].key;
  else
    return INFINITY;
}

size_t *bpqGetItems(const BoundedPriorityQueue *bpq)
{
  if (!bpq->actualsize)
    return NULL;
  size_t *Items = malloc(bpq->capacity * sizeof(size_t));
  if (!Items)
    return NULL;
  for (unsigned int i = 0; i < bpq->capacity; i++)
  {
    Items[i] = bpq->elements[i].value;
  }

  return Items;
}

size_t bpqSize(const BoundedPriorityQueue *bpq)
{
  return bpq->actualsize;
}

size_t bpqCapacity(const BoundedPriorityQueue *bpq)
{
  return bpq->capacity;
}
