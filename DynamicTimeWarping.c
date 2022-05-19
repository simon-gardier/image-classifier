#include "Sketch.h"
#include "DynamicTimeWarping.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static double min3(double a, double b, double c){
  double min = a;
  if(b < min)
    min = b;
  if(c < min)
    min = c;
  return min;
}

double dtw(Sketch sketch1, Sketch sketch2, double maxDistance){
  if(sketch1.points == sketch2.points)
    return 0.0;

  unsigned int costMatrixWidth = sketch1.size + 1;
  unsigned int costMatrixHeight = sketch2.size + 1;

  //Initialization of the distance matrix
  double costMatrix[costMatrixHeight][costMatrixWidth];

  //Initialization of the distance matrix rows
  //first line
  costMatrix[0][0] = 0;
  for(unsigned int row1Col = 1; row1Col < costMatrixWidth; row1Col++){
    costMatrix[0][row1Col] = +INFINITY;
  }
  //other lines
  for(unsigned int row = 1; row < costMatrixHeight; row++){
    costMatrix[row][0] = +INFINITY;
  }

  //Filling of the distance matrix
  for(unsigned int i = 0; i < sketch1.size; i++){

    //Process the distances from all sketch2 points to point i in sketch1
    for(unsigned int j = 0; j < sketch2.size; j++){
      
      double pointsDistance = ((double)(abs(sketch1.points[i].x - sketch2.points[j].x)) + (abs(sketch1.points[i].y - sketch2.points[j].y))) / 2;
      costMatrix[j+1][i+1] = pointsDistance + min3(costMatrix[j][i], costMatrix[j][i+1], costMatrix[j+1][i]);
    }
    
    unsigned int smallerValueInCol = 0;
    for(unsigned int j = 1; j < costMatrixHeight; j++){
      if(costMatrix[j][i+1] <= maxDistance){
        smallerValueInCol = 1;
        break;
      }
    }
    if(!smallerValueInCol)
        return +INFINITY;
  }

  return costMatrix[costMatrixHeight-1][costMatrixWidth-1];
}
