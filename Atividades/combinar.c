#include <stdio.h>
#include <stdlib.h>
#include "source/graphic_tools.h"

//uma função de passo suave com f(0) = 0, f(1) = 1, crescente no intervalo [0,1]
double f(double x){
  double xx = x*x;
  return (xx)/(2*xx - 2*x + 1);
}

//a função anterior ajustada para que f(200, y) = 0, f(400, y) = 1, decrescente em relação x no intervalo [200, 400]
double mask(unsigned x, unsigned y){
  if(x < 200) return 1;
  else if(x > 400) return 0;
  else return 1-f((((double)x)-200)/200);
}

//uso do programa: compile com 'gcc -o run combinar.c graphic_tools.c'
//execute './run gatobranco.ppm gatopreto.ppm out.ppm'
void main(int argc, char **argv){

  //carrega as imagens
  image img1;
  loadImage(&img1, argv[1]);
  image img2;
  loadImage(&img2, argv[2]);

  //esta função está implementada em graphic_tools.c
  affine T = fit3Points(&(vector){280, 320}, &(vector){190, 200}, &(vector){385, 200}, &(vector){440, 390}, &(vector){350, 290}, &(vector){530, 285});

  //aplica uma transformação afim na segunda imagem
  image img3 = linearTransformation(&img2, &(vector){440, 390}, &T.M, 1);
  img3 = translateImage(&img3, &T.v);

  //sobrepõe a segunda imagem sobre a primeira
  overlay(&img1, &img3, mask);

  //salva a primeira imagem
  saveImage(&img1, argv[3]);

  //destrói as três imagens
  destImage(&img1);
  destImage(&img2);
  destImage(&img3);
}
