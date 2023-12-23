#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "source/graphic_tools.h"

//uso do programa: compile com 'gcc -o run rotacionar.c graphic_tools.c'
//para rotacionar 'Patrick (Original).ppm', armazene o programa e este aquivo na mesma pasta e execute:
//'./run 'Patrick (Original).ppm'  'Patrick (Não suavizado).ppm' 'Patrick (Suavizado 1).ppm' 'Patrick (Suavizado 2).ppm''
void main(int argc, char **argv){
  unsigned x, y;


  //imagem armazenada em um struct image, implementado em graphic_tools.h
  image img1;

  //lê a imagem do endereço dado no primeiro argumento
  loadImage(&img1, argv[1]);
  matrix A;
  image img2;
  image img3;
  image img4;

  double angle = 0.25878;

  //matriz armazenada em um struct matrix, implementado em graphi_tools.h
  matrix T = {cos(angle), sin(angle), -sin(angle), cos(angle)};

  //a função abaixo está implementada em graphic_tools.c
  //a primeira imagem não recebe suavização
  //a segunda recebe suavização considerando a distância de manhattan entre a imagem da transformação linear e os pixels vizinhos
  //a terceira imagem suaviza através da distância euclidiana
  img2 = linearTransformation(&img1, &(vector){(img1.w+1)/2, (img1.h+1)/2}, &T, 0);
  img3 = linearTransformation(&img1, &(vector){(img1.w+1)/2, (img1.h+1)/2}, &T, 1);
  img4 = linearTransformation(&img1, &(vector){(img1.w+1)/2, (img1.h+1)/2}, &T, 2);

  //salva as imagens nos endereços dados pelo segundo, terceiro e quarto argumentos
  saveImage(&img2, argv[2]);
  saveImage(&img3, argv[3]);
  saveImage(&img4, argv[4]);

  //destrói as imagens, aplicando um free na memória alocada
  destImage(&img1);
  destImage(&img2);
  destImage(&img3);
  destImage(&img4);
}
