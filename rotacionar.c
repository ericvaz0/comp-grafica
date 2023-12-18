#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "graphic_tools.h"

void main(int argc, char **argv){
  image img1;
  loadImage(&img1, argv[1]);
  matrix A;
  image img2;

  char path[9] = "out0.ppm";
  
  double angle = M_PI/8;
  
  unsigned k = 0;
  for(k = 0; k < 17; k++){
    A = (matrix){cos(angle*k), -sin(angle*k), sin(angle*k), cos(angle*k)};
    img2 = affineTransform(&img1, &(vector){img1.w/2, img1.h/2}, &A);

    path[3] = 97 + k;
    saveImage(&img2, path);
  }
  

  destImage(&img1);
  destImage(&img2);
}
