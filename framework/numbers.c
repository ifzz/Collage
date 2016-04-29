#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define M_PI 3.14159265358979323846


int getRandomInt(int n) {
	if ((n - 1) == RAND_MAX) {
		return rand();
	} else {
		long end = RAND_MAX / n; // truncate skew
		//assert (end > 0L);
		end *= n;

		int r;
		while ((r = rand()) >= end);

		return r % n;
	}
}

double getRandomDouble(double n) {
	return (double)rand()/(double)(RAND_MAX / n);
}

double interp(double n1, double n2, double t) {
	return n1 + (n2 - n1) * t;
}

float interpF(float n1, float n2, float t) {
	return n1 + (n2 - n1) * t;
}

int distance(int x1, int y1, int x2, int y2) {
	return abs(x2 - x1) + abs(y2 - y1);
}

float distanceFloat(float x1, float y1, float x2, float y2) {
	long x_dist = fabs(x2 - x1);
	long y_dist = fabs(y2 - y1);

	if (x_dist > y_dist) {
		return (float) (y_dist + (x_dist - y_dist));
	} else {
		return (float) (x_dist + (y_dist - x_dist));
	}
}

float cubicPulse(float c, float w, float x) {
	x = fabs(x - c);

	if (x > w)
		return 0.f;

	x /= w;

	return 1.f - x * x * (3.f - 2.f * x);
}

float impulse(float k, float x) {
	const float h = k * x;

	return h * expf(1.0f - h);
}

float parabola(float x, float k) {
	return powf(4.0f * x * (1.0f - x), k);
}

int clip(int i, int min, int max) {
	const int t = i < min ? min : i;
	return t > max ? max : t;
}

float clipFloat(float f, float min, float max) {
  const float t = f < min ? min : f;
  return t > max ? max : t;
}

int directionTo(int x1, int y1, int x2, int y2) {
	double theta = atan2((y2 - y1), -(x1 - x2));

	if (theta < 0) {
		theta += 2 * M_PI;
	}

	return (int)((theta * (180 / M_PI)) + .5);
}

void velocity(double velocityArray[2], int direction, double speed) {
	double rad = (double)direction * (M_PI / 180);

	velocityArray[0] = (cos(rad) * speed);
	velocityArray[1] = (sin(rad) * speed);
}

int **create2dIntArray(int width, int height) {
	int **array;
	int i;

	array = malloc(width * sizeof(int *));

	for (i = 0; i < width; i ++) {
		array[i] = malloc(height * sizeof(int));
	}

	return array;
}

bool **create2dBoolArray(int width, int height) {
	bool **array;
	int i;

	array = malloc(width * sizeof(bool *));

	for (i = 0; i < width; i ++) {
		array[i] = malloc(height * sizeof(bool));
	}

	return array;
}

float **create2dFloatArray(int width, int height) {
	float **array;
	int i;

	array = malloc(width * sizeof(float *));

	for (i = 0; i < width; i ++) {
		array[i] = malloc(height * sizeof(float));
	}

	return array;
}

void delete2dArray(void **array, int width) {
	int i;

	for (i = 0; i < width; ++i) {
		free(array[i]);
	}

	free(array);
}

//Source: http://stackoverflow.com/a/10322471
void drawCircle(int x, int y, int size, void (*callback)(int, int)) {
	for(int xx = -size; xx <= size; xx++) {
		for(int yy = -size; yy <= size; yy++) {
			double dx = (double)xx / (double)size;
			double dy = (double)yy / (double)size;
			
			if(dx*dx+dy*dy <= 1) {
				callback(x + xx, y + yy);
			}
		}
	}
}
