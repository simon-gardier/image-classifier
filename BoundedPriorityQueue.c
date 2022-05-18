#include "BoundedPriorityQueue.h"
#include <stdlib.h>
#include <stdio.h>


struct bounded_priority_queue_t {
  size_t actualsize;
  size_t capacity;
  double *key;
  size_t *value;
};
static void doubleswap(double *array,size_t *array2, int i,int j )
{
  double temp = array[i];
  array[i] = array[j];
  array[j] = temp;
  size_t temp2 = array2[i];
  array2[i] = array2[j];
  array2[j]= temp2;
}

static void maxHeapify(double* key,size_t *value, unsigned int root, unsigned int heapSize){
    unsigned int left = 2*root + 1;
    unsigned int right = 2*root + 2;
    unsigned int largest = root;
    
    if(left < heapSize && key[left] > key[largest]){
        largest = left;
    }
    if(right < heapSize && key[right] > key[largest]){
        largest = right;
    }
    
    if(largest != root){
        doubleswap(key,value, root, largest);
        maxHeapify(key,value, largest, heapSize);
    }
}
static void HeapDeleteMax(BoundedPriorityQueue *bpq)
{
  unsigned int position = bpq->actualsize - 1;
  bpq->key[0]= bpq->key[position];
  bpq->value[0]= bpq->key[position];
  bpq->actualsize = position;
  maxHeapify(bpq->key,bpq->value,0,bpq->actualsize);
}
static void Heap(BoundedPriorityQueue *bpq, size_t position,double key,size_t value)
{
  bpq->key[position] = key;
  bpq->value[position]= value;
  int i = position;
  while (i > 0 &&bpq->key[i/2] < bpq->key[i])
  {
    doubleswap(bpq->key,bpq->value,i,i/2);
    i /= 2;
  } 
}


BoundedPriorityQueue* bpqCreate(size_t capacity) {
  if(capacity <= 0)
    return NULL;
  BoundedPriorityQueue* Priority = malloc(sizeof(BoundedPriorityQueue));
  if(!Priority)
    return NULL;
  Priority->capacity = capacity;
  Priority->actualsize = 0;
  Priority->key = malloc(capacity*sizeof(double));
  if(!Priority->key){
    free(Priority);
    return NULL;
  }
  Priority->value = malloc(capacity*sizeof(size_t));
  if(!Priority->value){
    free(Priority->key);
    free(Priority);
    return NULL;
  }

  return Priority;
}

void bpqFree(BoundedPriorityQueue* bpq) {
  free(bpq->value);
  free(bpq->key);
  free(bpq);
}

bool bpqInsert(BoundedPriorityQueue* bpq, double key, size_t value) {
  if(!bpq ||bpq->actualsize == bpq->capacity )
    return false;
  if(bpq->actualsize == 0)
  {
    bpq->key[bpq->actualsize] = key;
    bpq->value[bpq->actualsize]= value;
    bpq->actualsize++;
    return true;
  }
  Heap(bpq,bpq->actualsize,key,value);
  bpq->actualsize++;
  return true;
  
}

void bpqReplaceMaximum(BoundedPriorityQueue* bpq, double key, size_t value) {
  if(bpq->actualsize > 0 && bpqMaximumKey(bpq) > key)
  { 
    HeapDeleteMax(bpq);
    bool test = bpqInsert(bpq,key,value);
}
}

double bpqMaximumKey(const BoundedPriorityQueue* bpq) {
  if(bpq->actualsize >0)
    return bpq->key[0];
  else 
    return 0;
} 

size_t* bpqGetItems(const BoundedPriorityQueue* bpq) {
  return bpq->value;
}

size_t bpqSize(const BoundedPriorityQueue* bpq) {
  return bpq->actualsize;
}

size_t bpqCapacity(const BoundedPriorityQueue* bpq) {
  return bpq->capacity;
}
