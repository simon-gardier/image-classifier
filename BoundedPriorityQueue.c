#include "BoundedPriorityQueue.h"
#include <stdlib.h>
#include <math.h>

typedef struct Element_t{
  double key;
  size_t value;
}Element;

struct bounded_priority_queue_t {
  size_t actualsize;
  size_t capacity;
  Element *element;
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
static void doubleswap(BoundedPriorityQueue *bpq,unsigned int i,unsigned int j )
{
  //For key the change
  double key =bpq->element[i].key;
  bpq->element[i].key = bpq->element[j].key;
  bpq->element[j].key = key;
  //For value the change
  size_t value =bpq->element[i].value;
  bpq->element[i].value = bpq->element[j].value;
  bpq->element[j].value = value;

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
static void maxHeapify(BoundedPriorityQueue *bpq, unsigned int root, unsigned int heapSize){
    unsigned int left = 2*root + 1;
    unsigned int right = 2*root + 2;
    unsigned int largest = root;
    
    if(left < heapSize && bpq->element[left].key > bpq->element[largest].key){
        largest = left;
    }
    if(right < heapSize && bpq->element[right].key > bpq->element[largest].key){
        largest = right;
    }
    if(largest != root){
        doubleswap(bpq, root, largest);
        maxHeapify(bpq, largest, heapSize);
    }
}
/**
 * @fn HeapDeleteMax
 * @brief Delete the maximum in the heap
 * @param bpq The binay heap need to retire 
 */
static void HeapDeleteMax(BoundedPriorityQueue *bpq)
{
  unsigned int position = bpq->actualsize; 
  position = position - 1;
  bpq->element[0].key= bpq->element[position].key;
  bpq->element[0].value= bpq->element[position].value;
  bpq->actualsize = position;
  maxHeapify(bpq,0,bpq->actualsize);
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
  bpq->element[position].key = key;
  bpq->element[position].value= value;
  unsigned int i = position;
  while (i > 0 && bpq->element[i/2].key < bpq->element[i].key)
  {
    doubleswap(bpq,i,i/2);
    i = i / 2;
  } 
}
BoundedPriorityQueue* bpqCreate(size_t capacity) {
  BoundedPriorityQueue *Priority = malloc(sizeof(BoundedPriorityQueue));
  if(!Priority)
    return NULL;
  Priority->capacity = capacity;
  Priority->actualsize = 0;
  Priority->element = malloc(capacity * sizeof(Element));
  if(!Priority->element)
  {
    free(Priority);
    return NULL;
  }
  return Priority;
}

void bpqFree(BoundedPriorityQueue* bpq) {
  if(bpq) 
    free(bpq->element);
}

bool bpqInsert(BoundedPriorityQueue* bpq, double key, size_t value) {
  if(bpq->actualsize == bpq->capacity )
    return false;
  Heap(bpq,bpq->actualsize,key,value);
  bpq->actualsize++;
  return true;
  
}

void bpqReplaceMaximum(BoundedPriorityQueue* bpq, double key, size_t value) {
  if(bpq->actualsize > 0 && bpqMaximumKey(bpq) < key)
  { 
    HeapDeleteMax(bpq);
    bpqInsert(bpq,key,value);
  }
}

double bpqMaximumKey(const BoundedPriorityQueue* bpq) {
  if(bpq->actualsize >0)
    return bpq->element[0].key;
  else 
    return INFINITY;
} 

size_t* bpqGetItems(const BoundedPriorityQueue* bpq) {
  if(!bpq->actualsize)
    return NULL;
  size_t *Items = malloc(bpq->capacity*sizeof(size_t));
  if(!Items)
    return NULL;
  for(unsigned int i= 0; i < bpq->capacity;i++)
  {
    Items[i]= bpq->element[i].value;
  }
  return Items;
}


size_t bpqSize(const BoundedPriorityQueue* bpq) {
  return bpq->actualsize;
}

size_t bpqCapacity(const BoundedPriorityQueue* bpq) {
  return bpq->capacity;
}

