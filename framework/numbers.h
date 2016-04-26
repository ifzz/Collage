#include <stdbool.h>

int getRandomInt(int);
double getRandomDouble(double);
double interp(double, double, double);
int clip(int, int, int);
float clipFloat(float, float, float);
int distance(int, int, int, int);
float distanceFloat(float, float, float, float);
int directionTo(int, int, int, int);
void velocity(double[2], int, double);
int **create2dIntArray(int, int);
bool **create2dBoolArray(int, int);
float **create2dFloatArray(int, int);
void delete2dArray(void **, int);
void drawCircle(int x, int y, int size, void (*)(int, int));
float cubicPulse(float, float, float);
float impulse(float, float);
