#include <stdio.h>
#include <stdlib.h>
#include "source/graphic_tools.h"

void main(int argc, char **argv){
  image img0;
  loadImage(&img0, argv[1]);

  imageP img1 = imageToImageP(&img0, 4, 0);
  imageP img2 = imageToImageP(&img0, 4, 1);

  image img3 = imagePToImage(&img1);
  image img4 = imagePToImage(&img2);

  saveImage(&img3, argv[2]);
  saveImage(&img4, argv[3]);

  destImage(&img0);
  destImageP(&img1);
  destImageP(&img2);
  destImage(&img3);
  destImage(&img4);
}