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

long min(long a, long b);
long max(long a, long b);

unsigned int avg(int a, int b, double w);

double f(double x);
double mask(double x, double a, double b);

double matrixDet(matrix* A);

matrix matrixInv(matrix* A);

vector matrixProdVector(matrix* A, vector* u);

matrix matrixProdMatrix(matrix* A, matrix* B);

vector vectorSum(vector* u, vector* v);

vector vectorSub(vector* u, vector* v);

void inicImage(image* img, unsigned w, unsigned h);

void destImage(image* img);

int loadImage(image* img, char* path);

int saveImage(image* img, char* path);

pixel getPixel(image* img, unsigned x, unsigned y);

void setPixel(image* img, unsigned x, unsigned y, pixel pix);

pixel pixelScale(pixel* pix, double w);

matrix fit3Points(vector* A0, vector* B0, vector* C0, vector* A1, vector* B1, vector* C1);

image affineTransform(image* img, vector* O, matrix* T);

image affineTransformWeighted(image* img, vector* O, matrix* T);

pixel avgPixel(pixel pix1, pixel pix2, double w);

void overlaySmooth(image* img1, image* img2, int x0, int y0, int center, unsigned band, double (*mask) (double, double, double));

#endif