#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphic_tools.h"

long min(long a, long b){
  return a < b ? a : b;
}

long max(long a, long b){
  return a > b ? a : b;
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

pixel pixelScale(pixel* pix, double w){
  unsigned char r = w*pix->r;
  unsigned char g = w*pix->g;
  unsigned char b = w*pix->b;

  return (pixel){r, g, b}; 
}

double pixelDistance(pixel* pix1, pixel* pix2, int metric){
  if(metric == 0){
    return abs((int)pix1->r - (int)pix2->r) + abs((int)pix1->g - (int)pix2->g) + abs((int)pix1->b - (int)pix2->b);
  }
  else{
    return sqrt(pow((int)pix1->r - (int)pix2->r, 2) + pow((int)pix1->g - (int)pix2->g, 2) + pow((int)pix1->b - (int)pix2->b, 2));
  }

}

void inicImage(image* img, unsigned w, unsigned h){
  img->w = w;
  img->h = h;
  img->pixels = (pixel*)calloc(w*h, sizeof(pixel));
};
void inicImageP(imageP* img, unsigned w, unsigned h, unsigned n){
  img->w = w;
  img->h = h;
  img->n = n;
  img->palette = (pixel*)calloc(n, sizeof(pixel));
  img->pixelIndices = (unsigned*)calloc(w*h, sizeof(unsigned));
};

void destImage(image* img){
  free(img->pixels);
}
void destImageP(imageP* img){
  free(img->palette);
  free(img->pixelIndices);
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

imageP imageToImageP(image* img0, unsigned Q, int mode){
  imageP img1;

  unsigned w = img0->w;
  unsigned h = img0->h;
  inicImageP(&img1, w, h, Q*Q*Q);
  
  if(mode == 0){
    setNaivePalette(&img1, Q);
  }
  else if(mode == 1){
    setRandomPalette(&img1);
  }

  updatePixelIndices(&img1, img0);

  return img1;
}

void updatePixelIndices(imageP* img1, image* img0){
  unsigned w = img1->w;
  unsigned h = img1->h;
  unsigned n = img1->n;
  double min;
  double dist;

  unsigned pixelIndex;

  unsigned x, y, k;
  
  pixel pix0, pix1;

  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      for(k = 0; k < n; k++){

        pix0 = getPixel(img0, x, y);
        pix1 = img1->palette[k];
        dist = pixelDistance(&pix0, &pix1, 0);
        if(k == 0 || dist < min){
          min = dist;
          pixelIndex = k;
        }
      }
      
      setPixelIndex(img1, x, y, pixelIndex);
      
    }
  } 
}

image imagePToImage(imageP* img0){
  image img1;
  unsigned w = img0->w;
  unsigned h = img0->h;
  inicImage(&img1, w, h);

  unsigned x, y;

  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      setPixel(&img1, x, y, img0->palette[getPixelIndex(img0, x, y)]);
    }
  }

  return img1;
}

void setNaivePalette(imageP* img, unsigned Q){
  img->n = Q*Q*Q;
  unsigned quant = 256/Q;

  unsigned i, j, k;

  for(i = 0; i < Q; i++){
    for(j = 0; j < Q; j++){
      for(k = 0; k < Q; k++){
        img->palette[i*Q*Q + j*Q + k] = (pixel){(i+0.5)*quant, (j+0.5)*quant, (k+0.5)*quant};
      }
    }
  }
}

void setRandomPalette(imageP* img){
  unsigned n = img->n;

  unsigned k;
  for(k = 0; k < n; k++){
    img->palette[k] = (pixel){rand(), rand(), rand()};
  }
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
unsigned getPixelIndex(imageP* img, unsigned x, unsigned y){
  if(x < 0 || y < 0 || x >= img->w || y >= img->h){
    return 0;
  }
  return (img->pixelIndices)[y*(img->w) + x];
}

void setPixel(image* img, int x, int y, pixel pix){
  if(x >= 0 && x <= img->w && y >= 0 && y <= img->h){
    (img->pixels)[y*(img->w) + x] = pix;
  }
}
void setPixelIndex(imageP* img, int x, int y, unsigned index){
  if(x >= 0 && x <= img->w && y >= 0 && y <= img->h){
    (img->pixelIndices)[y*(img->w) + x] = index;
  }
}

//dados dois conjuntos de pontos no plano {A0, B0, C0} e {A1, B1, C1}, esta função retorna
//a única transformação afim T : R^2 -> R^2 tal que T(A1) = A0, T(B1) = B0, T(C1) = C0
//uma tranformação afim consiste de uma transformação linear seguida por uma translação
//a transformação afim é dada pelo struct affine, que carrega a matriz e vetor correspondentes
affine fit3Points(vector* A0, vector* B0, vector* C0, vector* A1, vector* B1, vector* C1){
  vector u0 = vectorSub(B0, A0);
  vector v0 = vectorSub(C0, A0);
  vector u1 = vectorSub(B1, A1);
  vector v1 = vectorSub(C1, A1);

  matrix M0 = (matrix){u0.x, v0.x, u0.y, v0.y};
  matrix M1 = (matrix){u1.x, v1.x, u1.y, v1.y};
  matrix M2 = matrixInv(&M1);

  matrix M3 = matrixProdMatrix(&M2, &M1);
  matrix M4 = matrixProdMatrix(&M3, &M0);

  vector v = vectorSub(A0, A1);

  affine T = (affine){matrixProdMatrix(&M0, &M2), v};
  return T;
}

double norm(double x, double y){
  return sqrt(x*x + y*y);
}


//uso: o argumento img é um ponteiro para um struct image contendo uma imagem
//o argumento O aponta para um struct vector, que define a origem do sistema de coordenadas
//o argumento T aponta para uma matriz contendo a transformação linear
//o argumento weighted define o modo de suavização da imagem:
//weighted = 0: sem suavização
//weighted = 1: suavização pela métrica do taxi
//weighted = 2: suavização pela métrica euclidiana
image linearTransformation(image* img, vector* O, matrix* T, int weighted){
  unsigned w = img->w;
  unsigned h = img->h;

  //calcula a matriz inversa
  matrix inverseT = matrixInv(T);

  image img2;
  //inicia a imagem que armazenará o resultado
  inicImage(&img2, w, h);

  unsigned x, y;
  vector u;

  pixel pix1;
  pixel pix2;
  pixel pix3;
  pixel pix4;

  double e1;
  double e2;
  double w1;
  double w2;
  double w3;
  double w4;
  double total;

  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      u = (vector){x,y};

      //move o sistema de coordenadas para a origem
      u = vectorSub(&u, O);
      //aplica a transformação linear
      u = matrixProdVector(&inverseT, &u);
      u = vectorSum(&u, O);
      
      //obtem o píxel por arredondamento para baixo
      //(o único pixel utilizado se weighted = 0)
      pix1 = getPixel(img, floor(u.x), floor(u.y));


      if(weighted != 0){
        //calcula o erro de arredondamento
        e1 = u.x - floor(u.x);
        e2 = u.y - floor(u.y);

        //obtem os três outros pixel vizinhos
        pix2 = getPixel(img, floor(u.x), ceil(u.y));
        pix3 = getPixel(img, ceil(u.x), floor(u.y));
        pix4 = getPixel(img, ceil(u.x), ceil(u.y));

        //estima as distâncias complementares entre o ponto atual e os quatro pixels vizinhos, conforme a métrica escolhida
        if(weighted == 1){
          w1 = (2-e1-e2);
          w2 = (1-e1+e2);
          w3 = (1+e1-e2);
          w4 = (  e1+e2);
        }
        else{
          w1 = norm(1-e1,1-e2);
          w2 = norm(1-e1,  e2);
          w3 = norm(  e1,1-e2);
          w4 = norm(  e1,  e2);
        }


        //aplica uma média ponderada, utilizando as distâncias complementares como pesos
        total = 1/(w1+w2+w3+w4);
        pix1 = pixelScale(&pix1, w1*total);
        pix2 = pixelScale(&pix2, w2*total);
        pix3 = pixelScale(&pix3, w3*total);
        pix4 = pixelScale(&pix4, w4*total);

        //armazena a média dos pixels
        pix1 = (pixel){pix1.r+pix2.r+pix3.r+pix4.r, pix1.g+pix2.g+pix3.g+pix4.g, pix1.b+pix2.b+pix3.b+pix4.b};
      }


      //altera pixel
      setPixel(&img2, x, y, pix1);
    }
  }

  return img2;
}

//retorna uma imagem im2 que consiste da imagem img transladada em direção ao vetor v
image translateImage(image* img, vector* v){
  unsigned x,y;
  unsigned w = img->w;
  unsigned h = img->h;

  image img2;
  inicImage(&img2, w, h);

  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      setPixel(&img2, x, y, getPixel(img, x-v->x, y-v->y));
    }
  }

  return img2;
}


//esta função sobrepõe uma imagem apontada por img2 sobre a imagem apontada por img1
//utiliza-se um viés através da função mask.
//a função mask deve retornar um double entre 0 e 1
//caso mask(x,y) = z, então o ponto (x,y) deve conter z*100% da cor encontrada no pixel (x,y) da primeira imagem
//e (1-z)*100% da cor encontrada no pixel (x,y) da segunda imagem
void overlay(image* img1, image* img2, double (*mask) (unsigned, unsigned)){
  unsigned w = img1->w;
  unsigned h = img1->h;

  unsigned x, y;
  pixel pix1, pix2, pix;

  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      pix1 = getPixel(img1, x, y);
      pix2 = getPixel(img2, x, y);

      pix1 = pixelScale(&pix1, 1-mask(x,y));
      pix2 = pixelScale(&pix2, mask(x,y));

      pix = (pixel){pix1.r+pix2.r, pix1.g+pix2.g, pix1.b+pix2.b};
      setPixel(img1, x, y, pix);
    }
  }
}

image plotFunction(unsigned w, unsigned h, pixel (*f) (unsigned, unsigned)){
  image img;
  inicImage(&img, w, h);

  unsigned x,y;
  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      setPixel(&img, x, y, f(x,y));
    }
  }

  return img;
}

void histogram(image* img, unsigned redHist[256], unsigned greenHist[256], unsigned blueHist[256]){
  
  unsigned w = img->w;
  unsigned h = img->h;

  unsigned x, y, k;
  
  for(k = 0; k < 256; k++){
    redHist[k] = 0;
    blueHist[k] = 0;
    greenHist[k] = 0;
  }

  pixel pix;

  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      pix = getPixel(img, x, y);

      redHist[(int)pix.r]++;
      blueHist[(int)pix.g]++;
      greenHist[(int)pix.b]++;
    }
  }
}

void sort(unsigned* array, unsigned len, unsigned* sortedIndices){
  unsigned k;
  unsigned j;
  unsigned swap;

  for(k = 0; k < len; k++){
    sortedIndices[k] = k;
  }

  for(k = 0; k < len; k++){
    for(j = k; j < len; j++){
      if(array[sortedIndices[k]] > array[sortedIndices[j]]){
        swap = sortedIndices[k];
        sortedIndices[k] = sortedIndices[j];
        sortedIndices[j] = swap;
      }
    }
  }
}

void centroidIteration(imageP* img, image* img0){
  unsigned n = img->n;
  unsigned w = img->w;
  unsigned h = img->h;

  unsigned long* points = calloc(3*n, sizeof(unsigned long));
  unsigned *counters = calloc(n, sizeof(unsigned));
  
  unsigned x, y, k, i;
  pixel pix;

  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      i = getPixelIndex(img, x, y);
      pix = getPixel(img0, x, y);
      points[3*i] += pix.r;
      points[3*i + 1] += pix.g;
      points[3*i + 2] += pix.b;

      counters[i]++;
    }
  }

  for(k = 0; k < n; k++){
    points[3*k] = (unsigned long) (points[3*k] / ((double) counters[k]));
    points[3*k + 1] = (unsigned long) (points[3*k + 1] / ((double) counters[k]));
    points[3*k + 2] = (unsigned long) (points[3*k + 2] / ((double) counters[k]));

    img->palette[k] = (pixel){points[3*k], points[3*k + 1], points[3*k + 2]};
  }

  free(points);
  free(counters);

  updatePixelIndices(img, img0);
}

void fillImage(image* img, pixel pix){
  unsigned w = img->w;
  unsigned h = img->h;
  unsigned x, y;

  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      setPixel(img, x, y, pix);
    }
  }
}

void drawDisk(image* img, int x0, int y0, int r, pixel pix){
  int x, y;
  int rr = r*r;

  for(x = x0-r; x < x0+r; x++){
    for(y = y0-r; y < y0+r; y++){
      if(pow(x - x0, 2) + pow(y - y0, 2) <= rr){
        setPixel(img, x, y, pix);
      }
    }
  }
}

void copyImage(image* img1, image* img0){
  unsigned w = img0->w;
  unsigned h = img0->h;
  unsigned x, y;

  inicImage(img1, w, h);

  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      setPixel(img1, x, y, getPixel(img0, x, y));
    }
  }
}

void Histogram2D(image* img, image* hist){
  inicImage(hist, 768, 256);
  fillImage(hist, (pixel){255, 255, 255});

  unsigned w = img->w;
  unsigned h = img->h;

  unsigned char r, g, b;

  pixel pix;

  unsigned x, y;

  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      pix = getPixel(img, x, y);
      r = pix.r;
      g = pix.g;
      b = pix.b;

      setPixel(hist, r, g, (pixel){r,g,0});
      setPixel(hist, r + 256, b, (pixel){r,0,b});
      setPixel(hist, g + 512, b, (pixel){0,g,b});
    }
  }
}

void PHistogram2D(imageP* img1, image* hist, image* hist0){
  copyImage(hist, hist0);
  
  unsigned n = img1->n;

  pixel pix;
  unsigned k;

  unsigned char r, g, b;
  for(k = 0; k < n; k++){
    pix = img1->palette[k];
    r = pix.r;
    g = pix.g;
    b = pix.b;

    drawDisk(hist, r, g, 12, (pixel){255,255,255});
    drawDisk(hist, r, g, 10, (pixel){r,g,0});
    drawDisk(hist, r + 256, b, 12, (pixel){255,255,255});
    drawDisk(hist, r + 256, b, 10, (pixel){r,0,b});
    drawDisk(hist, g + 512, b, 12, (pixel){255,255,255});
    drawDisk(hist, g + 512, b, 10, (pixel){0,g,b});
  }
}