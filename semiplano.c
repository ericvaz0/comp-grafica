#include <stdio.h>
#include <stdlib.h>
#include "graphic_tools.h"

pixel f(unsigned x, unsigned y){
  vector A = {40, 60};
  vector B = {120, 82};

  double p = (y-A.y)*(B.x-A.x)-(B.y-A.y)*(x - A.x);
  if(p > 0) return (pixel){255, 0, 0};
  else return (pixel){0,0,255};
}

void main(int argc, char **argv){
  image img = plotFunction(200, 200, f);
  saveImage(&img, argv[1]);
  destImage(&img);
}