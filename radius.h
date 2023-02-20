#ifndef RADIUS
#define RADIUS

#include "kdtree.h"

int radius_search(double xdata, double ydata, struct kdnode *node, double radius, int search_count, FILE *f);
void print_radius(double xdata, double ydata, double radius);

#endif