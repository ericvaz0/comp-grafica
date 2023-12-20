#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "graphic_tools.h"

void main(int argc, char **argv){
  unsigned x, y;

  image img1;
  loadImage(&img1, argv[1]);
  matrix A;
  image img2;
  image img3;
  image img4;

  inicImage(&img2, img1.w*2, img1.h*2);
  
  for(x = 0; x < img1.w; x++){
    for(y = 0; y < img1.h; y++){
      setPixel(&img2, (img1.w+1)/2 + x, (img1.h+1)/2 + y, getPixel(&img1, x, y));
    }
  }

  matrix T = {2.7, 0, 0, 1.75};
  img3 = affineTransform(&img2, &(vector){(img2.w+1)/2, (img2.h+1)/2}, &T);
  img4 = affineTransformWeighted(&img2, &(vector){(img2.w+1)/2, (img2.h+1)/2}, &T);

  saveImage(&img3, argv[2]);
  saveImage(&img4, argv[3]);
  destImage(&img1);
  destImage(&img2);
  destImage(&img3);
  destImage(&img4);
}
