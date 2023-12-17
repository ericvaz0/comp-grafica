#include <stdio.h>
#include <stdlib.h>

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
struct pixel {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

typedef struct pixel pixel;

struct image {
  unsigned w;
  unsigned h;
  pixel* pixels;
};

typedef struct image image;

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

image affineTransform(image* img, double O1, double O2, double A1, double A2, double B1, double B2){
  unsigned w = img->w;
  unsigned h = img->h;

  image img2;
  inicImage(&img2, w, h);

  unsigned x, y;
  unsigned x1, y1;
  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      x1 = (unsigned) ((x - O1)*A1 + (y - O2)*A2 + O1);
      y1 = (unsigned) ((x - O1)*B1 + (y - O2)*B2 + O2);

      setPixel(&img2, x, y, getPixel(img, x1, y1));
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

void overlay(image* img1, image* img2, unsigned x0, unsigned y0){
  unsigned w = min(img2->w, img1->w - x0);
  unsigned h = min(img2->h, img1->h - y0);

  unsigned x, y;

  for(x = 0; x < w; x++){
    for(y = 0; y < h; y++){
      setPixel(img1, x+x0, y+y0, getPixel(img2, x, y));
    }
  }
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

void main(int argc, char **argv){
  image img1;
  loadImage(&img1, argv[1]);
  image img2;
  loadImage(&img2, argv[2]);
  image img3 = affineTransform(&img2, 440, 390, 0.9, 0, -0.01, 0.9);

  overlaySmooth(&img1, &img3, -150, -70, 290, 100, mask);

  saveImage(&img1, argv[3]);

  destImage(&img1);
  destImage(&img2);
  destImage(&img3);
}