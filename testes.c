#include <stdio.h>
#include <stdlib.h>
#include "source/graphic_tools.h"

void main(int argc, char **argv){
  image img0;
  loadImage(&img0, argv[1]);

  imageP img1= imageToImageP(&img0, 2);
  centroidIteration(&img1, &img0);

  image hist0;
  image hist1;
  Histogram2D(&img0, &hist0);
  PHistogram2D(&img1, &img0, &hist1, &hist0, 0);

  saveImage(&hist1, argv[2]);


  destImage(&img0);
  destImage(&hist0);
  destImage(&hist1);
  destImageP(&img1);
}