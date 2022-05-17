#include "BoundedPriorityQueue.h"
#include <stdlib.h>
typedef struct Data_t{
  double key;
  size_t value;
}Data;


struct bounded_priority_queue_t {
  unsigned int actualsize;
  Data *array;
  size_t capacity;
};
static double getKey(Data d){
  return d.key;
}
static void swap(Data* array, int i, int j){
  Data temp = array[i];
  array[i] = array[j];
  array[j] = temp;
}
static void heapify(Data *array, int n, int i)
{
    // Find parent
    int parent = (i - 1) / 2;
 
    if (getKey(array[parent]) > 0) {
      if (getKey(array[i]) > getKey(array[parent]) ) {
        swap(array, i,parent);
        heapify(array, n, parent);
        }
    }
}

static void setData(BoundedPriorityQueue *bpq,double key,size_t value, size_t position){
  bpq->array[position].key = key;
  bpq->array[position].value = value;
}


BoundedPriorityQueue* bpqCreate(size_t capacity) {
  if(capacity <= 0)
    return NULL;
  BoundedPriorityQueue* Priority = malloc(sizeof(BoundedPriorityQueue));
  if(!Priority)
    return NULL;
  Priority->capacity = capacity;
  Priority->actualsize = 0;
  Priority->array = malloc(capacity*sizeof(Data));
  if(!Priority->array){
    free(Priority);
    return NULL;
  }
  return Priority;
}

void bpqFree(BoundedPriorityQueue* bpq) {
  free(bpq->array);
  free(bpq);
}

bool bpqInsert(BoundedPriorityQueue* bpq, double key, size_t value) {
  if(!bpq )
    return false;
  if(bpq->actualsize == bpq->capacity)
    return false;
  if(bpq->actualsize == 0)
  {
    setData(bpq,key,value,bpq->actualsize);
    bpq->actualsize++;
    return true;
  }
  setData(bpq,key,value,bpq->actualsize);
  int tempo = bpq->actualsize;
  bpq->actualsize++;
  heapify(bpq->array,bpq->actualsize, tempo);
  return true;
  
}

void bpqReplaceMaximum(BoundedPriorityQueue* bpq, double key, size_t value) {
  bpqInsert(bpq,key,value);
}

double bpqMaximumKey(const BoundedPriorityQueue* bpq) {
  return getKey(bpq->array[0]);
} 

size_t* bpqGetItems(const BoundedPriorityQueue* bpq) {
  return bpq->array;
}

size_t bpqSize(const BoundedPriorityQueue* bpq) {
  return bpq->actualsize;
}

size_t bpqCapacity(const BoundedPriorityQueue* bpq) {
  return bpq->capacity;
}


