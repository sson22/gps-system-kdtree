#ifndef KDTREE
#define KDTREE

#include "utils.h"
#include <stdio.h>
#include <math.h>

#define C ','     
#define Q '\"'   
#define NL '\n' 
 
#define FIELD_LEN 128
#define MAX_DISTANCE 1000

typedef struct data{
	char *year, *bid, *pid, *bpid;  // 0-3
	char *clue, *name;              // 4, 5
	char *code;                     // 6
	char *desc;                     // 7
	double x, y;                    // 8,9
	char *loc;                      // 10;
}data_t;

typedef struct kdnode{
	data_t *data;
    struct kdnode *left;
    struct kdnode *right;
    struct kdnode *parent;
    struct kdnode *duplicate;
    int depth;
}kd_node;

struct kdnode *create_list();
void free_tree(struct kdnode *parent);
void free_data(struct data *data);

struct kdnode *kd_insert(struct data *data, struct kdnode *parent);
double cal_distance(double parentx, double parenty, double xdata, double ydata);
void print_data(data_t *d, FILE *f);
data_t *get_next_data(FILE *f);
void data_get_header(FILE *f);
void data_free_header();
char *getfield(FILE *f, char deli);

#endif