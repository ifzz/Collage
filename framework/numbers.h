#include <stdbool.h>

int getRandomInt(int);
double getRandomDouble(double);
double interp(double, double, double);
float interpF(float, float, float);
int clip(int, int, int);
float clipFloat(float, float, float);
double clipDouble(double, double, double);
int distance(int, int, int, int);
float distanceFloat(float, float, float, float);
double eulerDist(double, double, double, double);
int directionTo(int, int, int, int);
double directionToDouble(double, double, double, double);
void velocity(double[2], int, double);
void velocityFloat(float[2], int, float);
void velocityRad(double[2], double, double);
int **create2dIntArray(int, int);
bool **create2dBoolArray(int, int);
float **create2dFloatArray(int, int);
void delete2dArray(void **, int);
void drawCircle(int x, int y, int size, void*, void (*)(void*, int, int, int));
float cubicPulse(float, float, float);
float impulse(float, float);
float parabola(float, float);
float powerCurve(float, float, float);
void vectorAdd(double[2], double[2], double[2]);
void vectorSubtract(double[2], double[2], double[2]);
void vectorRotate(double[2], double[2], int);
void vectorNormalize(double[2], double[2]);
void vectorMultiply(double[2], double[2], double);
void vectorTruncate(double[2], double);
double vectorDistance(double[2], double[2]);
double vectorLength(double[2]);
double vectorDot(double[2], double[2]);
int radToDeg(double);
double degToRad(int);
int roll(int, int);
