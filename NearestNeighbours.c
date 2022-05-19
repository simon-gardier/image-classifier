#include "NearestNeighbours.h"
#include "BoundedPriorityQueue.h"
#include "DynamicTimeWarping.h"
#include "math.h"
#include <stdlib.h>

SketchDistance* nearestNeighbours(const Dataset* dataset, Sketch query, size_t k) {
  BoundedPriorityQueue* q = bpqCreate(k);
  bpqInsert(q, INFINITY, 0);

  if(!q)
    return NULL;

  double distancesArray[dataset->size];

  for(unsigned int i = 0; i < dataset->size; i++){
    double distanceBetween = dtw(query, dataset->sketches[i], bpqMaximumKey(q));
    distancesArray[i] = distanceBetween;
    if(!bpqInsert(q, distanceBetween, i) && distanceBetween < bpqMaximumKey(q))
      bpqReplaceMaximum(q, distanceBetween, i);
  }

  size_t* queueData = bpqGetItems(q);

  SketchDistance* nearestSketches = malloc(sizeof(SketchDistance) * k);
  for(unsigned int i = 0; i < k; i++){
    nearestSketches[i].distance = distancesArray[queueData[i]];
    nearestSketches[i].sketch = &(dataset->sketches[queueData[i]]);
  }

  bpqFree(q);

  return nearestSketches;
}
