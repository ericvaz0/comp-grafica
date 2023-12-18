#include <stdio.h>
#include <stdlib.h>
#include "graphic_tools.h"

long min(long a, long b){
  return a < b ? a : b;
}
long max(long a, long b){
  return a > b ? a : b;
}

unsigned int avg(int a, int b, double w){
  return (int)(a*(1-w) + b*w);
}

double f(double x){
  double xx = x*x;
  return (xx)/(2*xx - 2*x + 1);
}
double mask(double x, double a, double b){
  return x < a ? 0 : (x > b ? 1 : f((x-a)/(b-a)));
}

double matrixDet(matrix* A){
  return A->a*A->d - A->b*A->c;
}

matrix matrixInv(matrix* A){
  double I = matrixDet(A);
  I = I == 0? 1 : 1.0/I;

  
  return (matrix){I*(A->d), -I*(A->b), -I*(A->c), I*(A->a)};
}

vector matrixProdVector(matrix* A, vector* u){
  return (vector){(A->a)*(u->x) + (A->b)*(u->y), (A->c)*(u->x) + (A->d)*(u->y)};
}

matrix matrixProdMatrix(matrix* A, matrix* B){
  return (matrix){(A->a)*(B->a) + (A->b)*(B->c), (A->a)*(B->b) + (A->b)*(B->d), (A->c)*(B->a) + (A->d)*(B->c), (A->c)*(B->b) + (A->d)*(B->d)};
}

vector vectorSum(vector* u, vector* v){
  return (vector){u->x + v->x, u->y + v->y};
}

vector vectorSub(vector* u, vector* v){
  return (vector){u->x - v->x, u->y - v->y};
}

void inicImage(image* img, unsigned w, unsigned h){
  img->w = w;
  img->h = h;
  img->pixels = (pixel*)calloc(w*h, sizeof(pixel));
};

void destImage(image* img){
  free(img->pixels);
}

int loadImage(image* img, char* path){
  FILE *fp;
  
  fp = fopen(path, "rb");
  unsigned char type, colorMax, c;
  unsigned w, h;


  while ((c=getc(fp))=='#')
    while((c=getc(fp))!='\n');
  ungetc(c,fp);

  fscanf(fp, "P%hhu\n", &type);
  while ((c=getc(fp))=='#')
    while((c=getc(fp))!='\n');
  ungetc(c,fp);

  if(type != 6) return 0;
  
  fscanf(fp, "%u %u\n%hhu\n", &w, &h, &colorMax);
  if(colorMax != 255) return 0;


  inicImage(img, w, h);
  size_t k = fread(img->pixels, 1, (img->w)*(img->h)*3, fp);
  fclose(fp);
}

int saveImage(image* img, char* path){
  FILE *fp;
  fp = fopen(path, "wb");
  fprintf(fp, "P6\n");
  fprintf(fp, "%u %u\n%hhu\n", img->w, img->h, 255);
  size_t k = fwrite(img->pixels, 1, (img->w)*(img->h)*3, fp);
  fclose(fp);
}

pixel getPixel(image* img, unsigned x, unsigned y){
  if(x < 0 || y < 0 || x >= img->w || y >= img->h){
    return (pixel){0,0,0};
  }
  return (img->pixels)[y*(img->w) + x];
}

void setPixel(image* img, unsigned x, unsigned y, pixel pix){
  (img->pixels)[y*(img->w) + x] = pix;
}

matrix fit3Points(vector* A0, vector* B0, vector* C0, vector* A1, vector* B1, vector* C1){
  vector u0 = vectorSub(B0, A0);
  vector v0 = vectorSub(C0, A0);
  vector u1 = vectorSub(B1, A1);
  vector v1 = vectorSub(C1, A1);

  matrix M0 = (matrix){u0.x, v0.x, u0.y, v0.y};
  matrix M1 = (matrix){u1.x, v1.x, u1.y, v1.y};
  matrix M2 = matrixInv(&M1);

  matrix M3 = matrixProdMatrix(&M2, &M1);
  matrix M4 = matrixProdMatrix(&M3, &M0);

  printf("%f, %f\n%f, %f\n\n", M3.a, M3.b, M3.c, M3.d);
  printf("%f, %f\n%f, %f\n\n", M4.a, M4.b, M4.c, M4.d);
  printf("%f, %f\n%f, %f", M0.a, M0.b, M0.c, M0.d);


  return matrixProdMatrix(&M0, &M2);
}

image affineTransform(image* img, vector* O, matrix* T){
  unsigned w = img->w;
  unsigned h = img->h;

  matrix inverseT = matrixInv(T);

  image img2;
  inicImage(&img2, w, h);

  unsigned x, y;
  unsigned x1, y1;
  vector u;
  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      u = (vector){x,y};
      u = vectorSub(&u, O);
      u = matrixProdVector(&inverseT, &u);
      u = vectorSum(&u, O);

      setPixel(&img2, x, y, getPixel(img, u.x, u.y));
    }
  }

  return img2;
}

pixel avgPixel(pixel pix1, pixel pix2, double w){
  unsigned char r = avg(pix1.r, pix2.r, w);
  unsigned char g = avg(pix1.g, pix2.g, w);
  unsigned char b = avg(pix1.b, pix2.b, w);

  return (pixel){r, g, b};
}

void overlaySmooth(image* img1, image* img2, int x0, int y0, int center, unsigned band, double (*mask) (double, double, double)){
  unsigned x1 = max(0, x0);
  unsigned y1 = max(0, y0);

  unsigned x2 = min(img1->w, img2->w + x0);
  unsigned y2 = min(img1->h, img2->h + y0);

  unsigned x, y;
  pixel pix1, pix2, pix;

  for(x = x1; x < x2; x++){
    for(y = y1; y < y2; y++){
      pix1 = getPixel(img1, x, y);
      pix2 = getPixel(img2, x-x0, y-y0);

      pix = avgPixel(pix1, pix2, mask(x, center-band, center+band));
      setPixel(img1, x, y, pix);
    }
  }
}