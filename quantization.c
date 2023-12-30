#include <stdio.h>
#include <stdlib.h>
#include "source/graphic_tools.h"

void main(int argc, char **argv){
  image img0;
  loadImage(&img0, argv[1]);

  imageP img1 = imageToImageP(&img0, 2);
  imageP img2 = imageToImageP(&img0, 2);

  unsigned k;

  for(k = 0; k < 10; k++){
    centroidIteration(&img2, &img0);
    updatePixelIndices(&img2, &img0);
  }

  image hist0, hist1, hist2;
  PHistogram2D(&img1, &img0, &hist1, &hist0, 0);
  PHistogram2D(&img2, &img0, &hist2, &hist0, 0);

  image img3 = imagePToImage(&img1);
  image img4 = imagePToImage(&img2);

  saveImage(&hist0, argv[2]);
  saveImage(&hist1, argv[3]);
  saveImage(&hist2, argv[4]);
  saveImage(&img3, argv[5]);
  saveImage(&img4, argv[6]);

  destImage(&img0);
  destImageP(&img1);
  destImageP(&img2);
  destImage(&hist0);
  destImage(&hist1);
  destImage(&hist2);
  destImage(&img3);
  destImage(&img4);
}