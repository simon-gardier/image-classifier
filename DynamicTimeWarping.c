#include "Sketch.h"
#include "DynamicTimeWarping.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void free2dArray(void** array, unsigned int length){
  for(unsigned int i = 0; i < length; i++){
    free(array[i]);
  }
  free(array);
}

static double min3(double a, double b, double c){
  double min = a;
  if(b < min)
    min = b;
  if(c < min)
    min = c;
  return min;
}

double dtw(Sketch sketch1, Sketch sketch2, double maxDistance){

  unsigned int costMatrixWidth = sketch1.size + 1;
  unsigned int costMatrixHeight = sketch2.size + 1;

  //Initialization of the distance matrix
  double** costMatrix = malloc(sizeof(double*) * costMatrixHeight);
  if(!costMatrix)
    return 0.0;

  //Initialization of the distance matrix rows
  for(unsigned int i = 0; i < costMatrixHeight; i++){
    costMatrix[i] = malloc(sizeof(double) * costMatrixWidth);

    if(!costMatrix[i]){
      free2dArray((void**)costMatrix, i);
      return 0;
    }

    if(i == 0){
      costMatrix[1][0] = 0;
      for(unsigned int j = 1; j < costMatrixWidth; j++){
        costMatrix[i][j] = INFINITY;
      }
    }
    else{
      costMatrix[i][0] = INFINITY;
    }
  }

  //Filling of the distance matrix
  for(unsigned int i = 1; i < costMatrixWidth; i++){
    //Process the distances from all sketch2 points to point i in sketch1 
    for(unsigned int j = 1; j < costMatrixHeight; j++){
      double pointsDistance = ((double)(abs(sketch1.points[i - 1].x - sketch2.points[j - 1].x)) + (abs(sketch1.points[i - 1].y - sketch2.points[j - 1].y))) / 2;
      costMatrix[j][i] = pointsDistance + min3(costMatrix[j-1][i-1], costMatrix[j-1][i], costMatrix[j][i-1]);
      if(costMatrix[j][i] >= maxDistance){
        return INFINITY;
      }
    }
  }
  return costMatrix[costMatrixHeight-1][costMatrixWidth-1];
}
