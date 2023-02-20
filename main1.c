 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#include "utils.h"
#include "kdtree.h"
#include "radius.h"
#include "nearest.h"

#define C ','     
#define Q '\"'   
#define NL '\n'
#define FIELD_LEN 128
#define MAX_DISTANCE 1000

int main(int argc, char **argv){
	// handle if there's not enough argv
	if(argc < 3){
		fprintf(stderr, "Incorrect arguments\n");
		fprintf(stderr, "Run in the form\n");
		fprintf(stderr, "dict datafile outputfile < inputfile\n");
		exit(1);
	}
	
	// insert data to KD Tree
	struct data *d;
	char *s=NULL;
	FILE *f= my_fopen(argv[1], "r");
	data_get_header(f);  // get header line of data
	struct kdnode *kd = create_list();
	while ((d = get_next_data(f))) {
		kd = kd_insert(d, kd);
	}
	fclose(f);
	
	// search nearest data set to data sets from stdin, 
	// print results to argv[2]
	f= my_fopen(argv[2],"w");
	struct kdnode *nearest = create_list();
	while ( (s = getfield(stdin,NL)) ) {
		double xdata = atof(strtok(s, " "));
		double ydata =  atof(strtok(NULL, ""));
		double d = MAX_DISTANCE;
		double *dis = &d;
		find_nearest_neighbor(xdata, ydata, kd, &nearest, dis);
		print_nearest(xdata, ydata, nearest, f);
		free(s);
	}
	
	fclose(f);  
	free_tree(kd);
	fflush(stdout);
	data_free_header();
	return 0;
}
