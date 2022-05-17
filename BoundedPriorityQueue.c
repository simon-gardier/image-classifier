#include "BoundedPriorityQueue.h"
#include <stdlib.h>



struct bounded_priority_queue_t {
  size_t actualsize;
  size_t capacity;
  double *key;
  size_t *value;
};
static void doubleswap(double *array,size_t *array2, int i,int j ,int m, int n)
{
  double temp = array[i];
  array[i] = array[j];
  array[j] = temp;
  size_t temp2 = array2[m];
  array2[m] = array2[n];
  array2[n]= temp2;
}
static unsigned int minposition(double *array,unsigned int begin,unsigned int end)
{
  double min = array[begin];
  unsigned int min_position = begin;
  for(unsigned int i = begin ; i < end ;i++)
  {
    if(min > array[i]){
      min = array[i];
      min_position = i;
    }
  }
  return min_position;
}
static void Heap(BoundedPriorityQueue *bpq, size_t position,double key,size_t value)
{
  bpq->key[position] = key;
  bpq->value[position]= value;
  int i = position;
  bpq->actualsize++;
  while (i > 0 &&bpq->key[i/2] < bpq->key[i])
  {
    doubleswap(bpq->key,bpq->value,i,i/2,i,i/2);
    i = i/2;
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
  return true;
  
}

void bpqReplaceMaximum(BoundedPriorityQueue* bpq, double key, size_t value) {
  if(bpq->actualsize > 0)
  { 
    unsigned int half = bpq->actualsize/2;
    unsigned int position = minposition(bpq->key,half,bpq->actualsize);
    if(bpq->key[position] > key)
      return;
    Heap(bpq,position,key,value);
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



