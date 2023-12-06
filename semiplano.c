#include <stdio.h>
#include <stdlib.h>
void main(int argc, char **argv){
  int i, j, l, h;
  unsigned char type, cmax, caractere;
  FILE *fp;
  
  l = 100;
  h = 100;

  int a = 31;
  int b = 42;

  int c = 73;
  int d = 65;

  fp = fopen(argv[1], "wb");
  fprintf(fp, "P6\n");
  fprintf(fp, "%u %u\n%hhu\n", l, h, 255);
  for (j=0; j<h; j++){
    for (i=0; i<l; i++){
      if((j-b-1)*(c-a) > (d-b)*(i-a)){
        fprintf(fp, "%c%c%c", 255, 0, 0);
      }
      else if((j-b-1)*(c-a) < (d-b)*(i-a)){
        fprintf(fp, "%c%c%c", 0, 0, 255);
      }
      else fprintf(fp, "%c%c%c", 0, 0, 0);
      
      
    }
  }
  fclose(fp);
}