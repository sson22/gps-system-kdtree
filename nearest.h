#ifndef NEAREST
#define NEAREST
#include "kdtree.h"

void find_nearest_neighbor(double xdata, double ydata, struct kdnode *node, struct kdnode **nearest, double *nearest_distance);
void print_nearest(double xdata, double ydata, struct kdnode *d, FILE *f);

#endif