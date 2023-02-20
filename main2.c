#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#include "utils.h"
#include "kdtree.h"
#include "nearest.h"
#include "radius.h"

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
		//print_data(d,w);
		kd = kd_insert(d, kd);
	}
	fclose(f);
	
	// search data sets within the radius of data sets from stdin,
	// print results to argv[2]
	f= my_fopen(argv[2],"w");
	int search_count = 0;
	while ( (s = getfield(stdin,NL)) ) {
		
		double xdata = atof(strtok(s, " "));
		double ydata =  atof(strtok(NULL, " "));
		double radius = atof(strtok(NULL, ""));
		
		
		search_count = radius_search(xdata, ydata, kd, radius, search_count, f);
		if(search_count == 0){
		fprintf(f, "%lf %lf %lf --> NOT FOUND\n", xdata, ydata, radius);
		}
		print_radius(xdata, ydata, radius);
		free(s);
}

	fclose(f);  
	free_tree(kd);
	fflush(stdout);
	data_free_header();
	return 0;
}
