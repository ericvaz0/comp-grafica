#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int mandelbrot_bounded(int i, int j, int l, int h, float x0, float x1, float y0, float y1, int maxIterations, float maxNormSq){
    float x = ((x1 - x0)*i)/l + x0;
    float y = ((y1 - y0)*j)/h + y0;
    float R = 0;
    float I = 0;

    float S;

    for(int k = 0; k < maxIterations; k++){
        S = R;
        R = R*R - I*I;
        I = 2*S*I;

        R += x;
        I += y;
        
        if(R*R + I*I > maxNormSq) return k;
    }

    return 0;
}

float alfa(int Ux, int Uy, int Vx, int Vy){
    float produto = Ux*Vx + Uy*Vy;
    float norma = sqrtf((Ux*Ux + Uy*Uy)*(Vx*Vx + Vy*Vy));
    float a = produto/norma;
    if(a < -1) return M_PI;
    else if(a > 1) return 0;
    else return acosf(a);
}


#define eps 0.001
int polygon(int i, int j, int* pontos, int N){
    int lados[2*N];
    int vetores[2*N];
    float angulo = 0;

    for(int k = 0; k < N; k++){

        lados[2*k] = pontos[2*((k+1)%N)] - pontos[2*k];
        lados[2*k + 1] = pontos[2*((k+1)%N) + 1] - pontos[2*k + 1];

        vetores[2*k] = i - pontos[2*k];
        vetores[2*k+1] = j - pontos[2*k + 1];
    }

    for(int k = 0; k < N; k++){

        angulo += alfa(lados[2*k], lados[2*k + 1], vetores[2*k], vetores[2*k+1]);
        angulo += alfa(-lados[2*((N+k-1)%N)], -lados[2*((N+k-1)%N) + 1], vetores[2*k], vetores[2*k+1]);
    }
    if(angulo > (N-2)*M_PI + eps) return 255;
    else return 0;
}

#define h 100
#define l 100

int cor(int i, int j){
    return mandelbrot_bounded(i, j, l, h, -2.5, 1.5, -2, 2, 256, 10);
}

void main(int argc, char **argv){

  int i, j;
  int N = 5;
  int pontos[10] = {10, 10, 80, 20, 80, 60, 40, 80, 10, 50};

  FILE *fp;

  fp = fopen(argv[1], "wb");
  fprintf(fp, "P6\n");
  fprintf(fp, "%u %u\n%hhu\n", l, h, 255);

  unsigned char c;
  for (j=0; j<h; j++){
    for (i=0; i<l; i++){
        c = polygon(i,j, pontos, N);
        fprintf(fp, "%c%c%c", c, c, c);
    }
  }
  fclose(fp);
}