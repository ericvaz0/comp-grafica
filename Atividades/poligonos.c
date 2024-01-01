#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//esta função determina a orientação de um triângulo de vertices (ax, ay), (bx, by), (cx, cy)
int orientation(int ax, int ay, int bx, int by, int cx, int cy){
    //determinante da matriz
    //bx-ax   cx-ax
    //by-ay   cy-ay
    int det = (bx-ax)*(cy-ay) - (by-ay)*(cx-ax);
    if(det >= 0) return 1;
    else if(det < 0) return -1;
}

//calcula o número de revoluções de um polígono em relação a um ponto fixo (i,j)
//o array pontos possui 2*N inteiros no formato i1, j1, i2, j2, ..., iN, jN.
//cada ik, jk é um par de coordenadas de um ponto do polígono
//a função retorna o número de revoluções módulo mod
int winding(int i, int j, int* pontos, int N, int mod){
    int winding = 0;
    int ori;
    int k;

    for(k = 0; k < N; k++){
        ori = orientation(i, j, pontos[2*(k)], pontos[2*(k) + 1], pontos[2*((k+1)%N)], pontos[2*((k+1)%N) + 1]);
        if(pontos[2*(k)]  < i && pontos[2*((k+1)%N)]  >= i && ori == -1){
            winding++;
        }
        else if(pontos[2*((k+1)%N)]  < i && pontos[2*(k)]  >= i && ori == 1){
            winding--;
        }
    }
    return (mod == 0? winding : winding%mod) == 0 ? 0 : 255;
}


//Esta função recebe um número variável argumentos:
//Os primeiros dois argumentos são a altura e largura da imagem a ser gerada.
//Em seguida, escreva um número par de inteiros no formato i1, j1, i2, j2, .... Estas são as coordenadas dos vértices de um polígono.
//Opcionalmente, adicione em seguida um inteiro que servirá como módulo para o número de revoluções (2 por padrão)
//No fim, escreva o endereço da imagem a ser salva.
void main(int argc, char **argv){
  int i, j, k;

  int h = atoi(argv[1]);
  int l = atoi(argv[2]);
  int N = (int)(argc - 4)/2;
  
  int pontos[2*N];
  for(k = 0; k < N; k++){
    pontos[2*k] = atoi(argv[3 + 2*k]);
    pontos[2*k+1] = atoi(argv[3 + 2*k + 1]);
  }

  int mod = argc%2 == 0? 2 : atoi(argv[argc-2]);

  FILE *fp;

  fp = fopen(argv[argc-1], "wb");
  fprintf(fp, "P6\n");
  fprintf(fp, "%u %u\n%hhu\n", l, h, 255);

  unsigned char c;
  for (j=0; j<h; j++){
    for (i=0; i<l; i++){
        c = winding(i,j, pontos, N, mod);
        fprintf(fp, "%c%c%c", c, c, c);
    }
  }
  fclose(fp);
}
