#include <stdio.h>
#include <stdlib.h>
#include "graphic_tools.h"

//Este programa gera um histograma triplo de uma imagem .ppm no formato P6

//Compilação: Com a pasta comp-grafica aberta no terminal, execute o seguinte comando:
//  gcc -o run 'Projeto-1/Figura 2.c' source/graphic_tools.c -Isource

//Utilização: Para gerar o histograma da imagem 'images/borboleta.ppm' e armazená-lo em 'Projeto-1/Figuras/2.ppm', execute
//  ./run images/borboleta.ppm 'Projeto-1/Figuras/2.ppm'
void main(int argc, char **argv){

  //Carrega argv[1] para a memória no formato de um struct image
  image img0;
  loadImage(&img0, argv[1]);

  //Gera o histograma e o armazena em um struct image
  image hist;
  Histogram2D(&img0, &hist);

  //Exporta o histograma para argv[2]
  saveImage(&hist, argv[2]);

  //Libera a memória alocada
  destImage(&img0);
  destImage(&hist);
}