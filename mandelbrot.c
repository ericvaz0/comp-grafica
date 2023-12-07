#include <stdio.h>
#include <stdlib.h>

int bounded(int i, int j, int l, int h, float x0, float x1, float y0, float y1, int maxIterations, float maxNormSq){
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

void main(int argc, char **argv){
  int i, j;

  int l = 2000;
  int h = 2000;
  unsigned char type, cmax, caractere;
  FILE *fp;

  fp = fopen(argv[1], "wb");
  fprintf(fp, "P6\n");
  fprintf(fp, "%u %u\n%hhu\n", l, h, 255);
  for (j=0; j<h; j++){
    for (i=0; i<l; i++){
      fprintf(fp, "%c%c%c", 0, bounded(i, j, l, h, -2.5, 1.5, -2, 2, 256, 10), 0);
    }
    }
  fclose(fp);
}