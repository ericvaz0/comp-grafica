#include <stdio.h>
#include <stdlib.h>
#include "graphic_tools.h"

//Este programa recebe uma imagem .ppm no formato P6 e gera duas imagens de paletas reduzidas
//A primeira imagem possui uma paleta aleatória de 8 cores
//A segunda imagem possui uma paleta otimizada pelo algorítimo de Lloyd

//Compilação: Com a pasta comp-grafica aberta no terminal, execute o seguinte comando:
//  gcc -o run 'Projeto-1/Figuras 4, 5, 6, 7.c' source/graphic_tools.c -Isource

//Utilização: Para carregar a imagem 'images/borboleta.ppm',
//gerar um histograma da imagem com uma paleta aleatoria destacada em 'Projeto-1/Figuras/4.ppm'
//gerar uma versão da imagem com esta paleta aleatória em 'Projeto-1/Figuras/5.ppm'
//gerar um histograma da imagem com uma paleta otimizada destacada em 'Projeto-1/Figuras/7.ppm'
//gerar uma versão da imagem com esta paleta otimizada em 'Projeto-1/Figuras/6.ppm'
//execute o segunte comando:
//  ./run images/borboleta.ppm 'Projeto-1/Figuras/4.ppm' 'Projeto-1/Figuras/5.ppm' 'Projeto-1/Figuras/6.ppm' 'Projeto-1/Figuras/7.ppm'

void main(int argc, char **argv){
  //Carrega a imagem em argv[1]
  image img0;
  loadImage(&img0, argv[1]);

  //Cria uma imagem de paleta a partir de img0 com uma paleta reduzida com 8 cores aleatórias
  imageP img1 = imageToImageP(&img0, 2, 1);

  //Converte a imagem de paleta reduzida de volta para um struct compatível com o formato ppm
  image img2 = imagePToImage(&img1);

  image hist0;
  image hist1;

  //Gera um histograma da imagem em argv[0]
  Histogram2D(&img0, &hist0);
  //Desenha círculos coloridos sobre o histograma, destacando a paleta
  PHistogram2D(&img1, &hist1, &hist0);

  //Salva a img2 e o histograma da sua paleta
  saveImage(&hist1, argv[2]);
  saveImage(&img2, argv[3]);

  unsigned k;
  
  //Aplica 10 iterações do algorítimo de Lloyd em img1
  for(k = 0; k < 10; k++){
    centroidIteration(&img1, &img0);
  }

  image hist2;

  //Gera um histograma com círculos destacando a nova paleta
  PHistogram2D(&img1, &hist2, &hist0);

  //Converte a imagem de paleta reduzida para um struct compatível com o formato .ppm
  image img3 = imagePToImage(&img1);

  //Salva a a img3 e o histograma da sua paleta
  saveImage(&img3, argv[4]);
  saveImage(&hist2, argv[5]);

  //Libera a memória alocada
  destImage(&img0);
  destImageP(&img1);
  destImage(&img2);
  destImage(&img3);
  destImage(&hist0);
  destImage(&hist1);
  destImage(&hist2);
}