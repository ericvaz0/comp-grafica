#ifndef GTOOLS   /* Include guard */
#define GTOOLS


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

struct imageP {
  unsigned w;
  unsigned h;
  pixel* palette;
  unsigned n;
  unsigned* pixelIndices;
};
typedef struct imageP imageP;

struct matrix {
  double a;
  double b;
  double c;
  double d;
};
typedef struct matrix matrix;

struct vector
{
  double x;
  double y;
};
typedef struct vector vector;

struct affine {
  matrix M;
  vector v;
};
typedef struct affine affine;

long min(long a, long b);
long max(long a, long b);

double matrixDet(matrix* A);

matrix matrixInv(matrix* A);

vector matrixProdVector(matrix* A, vector* u);

matrix matrixProdMatrix(matrix* A, matrix* B);

vector vectorSum(vector* u, vector* v);

vector vectorSub(vector* u, vector* v);

void inicImage(image* img, unsigned w, unsigned h);
void inicImageP(imageP* img, unsigned w, unsigned h, unsigned n);

void destImage(image* img);
void destImageP(imageP* img);

int loadImage(image* img, char* path);

imageP imageToImageP(image* img0, unsigned n, int mode);

image imagePToImage(imageP* img0);

void updatePixelIndices(imageP* img1, image* img0);

void setNaivePalette(imageP* img, unsigned Q);

void setRandomPalette(imageP* img);

int saveImage(image* img, char* path);

void setBit(unsigned char* byte, unsigned char pos, unsigned char bit);

unsigned char getBit(unsigned char* byte, unsigned char pos);

void printByte(unsigned char* byte);

void breakBytes(unsigned char* array, unsigned char* array0, int l, int n);

void joinBytes(unsigned char* array, unsigned char* array0, int l, int n);

int saveImageP(imageP* img, char* path);

int loadImageP(imageP* img, char* path);

pixel getPixel(image* img, unsigned x, unsigned y);
unsigned getPixelIndex(imageP* img, unsigned x, unsigned y);

void setPixel(image* img, int x, int y, pixel pix);

void setPixelIndex(imageP* img, int x, int y, unsigned index);

pixel pixelScale(pixel* pix, double w);

double pixelDistance(pixel* pix1, pixel* pix2, int metric);

affine fit3Points(vector* A0, vector* B0, vector* C0, vector* A1, vector* B1, vector* C1);

image linearTransformation(image* img, vector* O, matrix* T, int weighted);

image translateImage(image* img, vector* v);

void overlay(image* img1, image* img2, double (*mask) (unsigned, unsigned));

image plotFunction(unsigned w, unsigned h, pixel (*f) (unsigned, unsigned));

void histogram(image* img, unsigned redHist[256], unsigned blueHist[256], unsigned greenHist[256]);

void centroidIteration(imageP* img, image* img0);

void fillImage(image* img, pixel pix);

void Histogram2D(image* img, image* hist);

void PHistogram2D(imageP* img1, image* hist, image* hist0);
#endif