#include <stdio.h>
#include <stdlib.h>
#include "graphic_tools.h"

//Este programa converte um arquivo .ppm no modo P6 para o modo P9

//Compilação: Com a pasta comp-grafica aberta no terminal, execute o seguinte comando:
//  gcc -o run Projeto-1/P6toP9.c  source/graphic_tools.c -Isource

//Utilização: Para converter a imagem 'images/borboleta.ppm' e armazená-la em 'Projeto-1/Borboleta P9.ppm', execute
//  ./run images/borboleta.ppm 'Projeto-1/Borboleta P9.ppm'
void main(int argc, char **argv){
  //Carrega argv[1] para a memória no formato de um struct image
  image img0;
  loadImage(&img0, argv[1]);

  imageP img1 = imageToImageP(&img0, 2, 0);
  //Cria uma imagem de paleta reduzida a partir de img0 com uma paleta reduzida com 8 cores

  //Aplica 10 iterações do algorítimo de Lloyd em img1
  unsigned k;
  for(k = 0; k < 10; k++){
    centroidIteration(&img1, &img0);
  }

  //Salva a img1 no formato P9 em argv[2]
  saveImageP(&img1, argv[2]);

  //Libera a memória alocada
  destImage(&img0);
  destImageP(&img1);
}