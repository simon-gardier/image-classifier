#include "BoundedPriorityQueue.h"
#include <stdlib.h>
#include <math.h>

struct bounded_priority_queue_t {
  size_t actualsize;
  size_t capacity;
  double* keys;
  size_t* values;
};
/**
 * @fn doubleswap
 * @brief Swap the value between 2 arrays
 * @param array first array to swap the value
 * @param array2 second array to swap the value
 * @param i the first position in the arrays
 * @param j the second position in the arrays 
 * 
 */
static void doubleswap(double *array,size_t *array2, int i,int j )
{
  double temp = array[i];
  array[i] = array[j];
  array[j] = temp;
  size_t temp2 = array2[i];
  array2[i] = array2[j];
  array2[j]= temp2;
}
/**
 * @fn maxHeapify
 * @brief Rebuild correclty the heap
 * @param key the heap to rebuild
 * @param value associated data to key
 * @param root the start of rebuild
 * @param heapSize the size of the heap
 * 
 */
static void maxHeapify(double* keys,size_t *values, unsigned int root, unsigned int heapSize){
    unsigned int left = 2*root + 1;
    unsigned int right = 2*root + 2;
    unsigned int largest = root;
    
    if(left < heapSize && keys[left] > keys[largest]){
        largest = left;
    }
    if(right < heapSize && keys[right] > keys[largest]){
        largest = right;
    }
    
    if(largest != root){
        doubleswap(keys,values, root, largest);
        maxHeapify(keys,values, largest, heapSize);
    }
}
/**
 * @fn HeapDeleteMax
 * @brief Delete the maximum in the heap
 * @param bpq The binay heap need to retire 
 */
static void HeapDeleteMax(BoundedPriorityQueue *bpq)
{
  unsigned int position = bpq->actualsize - 1;
  bpq->keys[0]= bpq->keys[position];
  bpq->values[0]= bpq->values[position];
  bpq->actualsize = position;
  maxHeapify(bpq->keys,bpq->values,0,bpq->actualsize);
}
/**
 * @fn Heap
 * @brief Insert the element in the heap
 * @param bpq The binary heap
 * @param position Where to insert in the heap
 * @param key the key 
 * @param value The data we need to have
 */
static void Heap(BoundedPriorityQueue *bpq, size_t position,double key,size_t value)
{
  bpq->keys[position] = key;
  bpq->values[position]= value;
  int i = position;
  while (i > 0 &&bpq->keys[i/2] < bpq->keys[i])
  {
    doubleswap(bpq->keys,bpq->values,i,i/2);
    i /= 2;
  } 
}
BoundedPriorityQueue* bpqCreate(size_t capacity) {
  BoundedPriorityQueue* Priority = malloc(sizeof(BoundedPriorityQueue));
  if(!Priority)
    return NULL;
  Priority->capacity = capacity;
  Priority->actualsize = 0;
  Priority->keys = malloc(capacity*sizeof(double));
  if(!Priority->keys){
    free(Priority);
    return NULL;
  }
  Priority->values = malloc(capacity*sizeof(size_t));
  if(!Priority->values){
    free(Priority->keys);
    free(Priority);
    return NULL;
  }
  return Priority;
}

void bpqFree(BoundedPriorityQueue* bpq) {
  if(bpq){
    free(bpq->values);
    free(bpq->keys);
    free(bpq);
  }
}

bool bpqInsert(BoundedPriorityQueue* bpq, double key, size_t value) {
  if(!bpq ||bpq->actualsize == bpq->capacity )
    return false;
  Heap(bpq,bpq->actualsize,key,value);
  bpq->actualsize++;
  return true;
  
}

void bpqReplaceMaximum(BoundedPriorityQueue* bpq, double key, size_t value) {
  if(bpq->actualsize > 0 && bpqMaximumKey(bpq) > key)
  { 
    HeapDeleteMax(bpq);
    bpqInsert(bpq,key,value);
  }
}

double bpqMaximumKey(const BoundedPriorityQueue* bpq) {
  if(bpq->actualsize >0)
    return bpq->keys[0];
  else 
    return INFINITY;
} 

size_t* bpqGetItems(const BoundedPriorityQueue* bpq) {
  return bpq->values;
}

size_t bpqSize(const BoundedPriorityQueue* bpq) {
  return bpq->actualsize;
}

size_t bpqCapacity(const BoundedPriorityQueue* bpq) {
  return bpq->capacity;
}


