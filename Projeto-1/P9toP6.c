#include <stdio.h>
#include <stdlib.h>
#include "graphic_tools.h"

//Este programa converte um arquivo .ppm no modo P9 para o modo P6

//Compilação: Com a pasta comp-grafica aberta no terminal, execute o seguinte comando:
//  gcc -o run Projeto-1/P9toP6.c  source/graphic_tools.c -Isource

//Utilização: Para converter a imagem 'Projeto-1/Borboleta P9.ppm' e armazená-la em 'Projeto-1/Borboleta P6.ppm', execute
//  ./run 'Projeto-1/Borboleta P9.ppm' 'Projeto-1/Borboleta P6.ppm'
void main(int argc, char **argv){
  //Carrega argv[1] para a memória no formato de um struct imageP
  imageP img0;
  loadImageP(&img0, argv[1]);

  //Converte img0 para uma imagem bitmap em um strict image
  image img1 = imagePToImage(&img0);

  //Salva img1 no formato P6 em argv[2]
  saveImage(&img1, argv[2]);

  //Libera a memória alocada
  destImageP(&img0);
  destImage(&img1);
}