#include <stdbool.h>

int getRandomInt(int);
double getRandomDouble(double);
double interp(double, double, double);
float interpF(float, float, float);
int clip(int, int, int);
float clipFloat(float, float, float);
int distance(int, int, int, int);
float distanceFloat(float, float, float, float);
double eulerDist(double, double, double, double);
int directionTo(int, int, int, int);
void velocity(double[2], int, double);
int **create2dIntArray(int, int);
bool **create2dBoolArray(int, int);
float **create2dFloatArray(int, int);
void delete2dArray(void **, int);
void drawCircle(int x, int y, int size, void (*)(int, int));
float cubicPulse(float, float, float);
float impulse(float, float);
float parabola(float, float);
void vectorAdd(double[2], double[2], double[2]);
void vectorSubtract(double[2], double[2], double[2]);
void vectorNormalize(double[2], double[2]);
void vectorMultiply(double[2], double[2], double);
void vectorTruncate(double[2], double);
double vectorDistance(double[2], double[2]);
double vectorLength(double[2]);
