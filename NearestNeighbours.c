#include "NearestNeighbours.h"
#include "BoundedPriorityQueue.h"
#include "DynamicTimeWarping.h"
#include "math.h"
#include <stdlib.h>

SketchDistance* nearestNeighbours(const Dataset* dataset, Sketch query, size_t k) {
  BoundedPriorityQueue* q = bpqCreate(k);

  if(!q)
    return NULL;

  double distanceBetweenArray[dataset->size];

  for(unsigned int i = 0; i < dataset->size; i++){
    double distanceBetween = dtw(query, dataset->sketches[i], bpqMaximumKey(q));
    distanceBetweenArray[i] = distanceBetween;

    if(!bpqInsert(q, distanceBetween, i))
      bpqReplaceMaximum(q, distanceBetween, i);
  }
  
  size_t* queueData = bpqGetItems(q);

  SketchDistance* nearestSketches = malloc(sizeof(SketchDistance) * k);
  for(unsigned int i = 0; i < bpqSize(q); i++){
    nearestSketches[i].distance = distanceBetweenArray[queueData[i]];
    nearestSketches[i].sketch = &(dataset->sketches[queueData[i]]);
  }

  bpqFree(q);

  return nearestSketches;
}
