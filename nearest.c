#include "nearest.h"
#include <math.h>

int near_count = 0;
//search nearest neighbor from the data sets from stdin
void find_nearest_neighbor(double xdata, double ydata, struct kdnode *node, struct kdnode **nearest, double *nearest_distance){
	 
	struct kdnode *good_side = NULL;
	struct kdnode *bad_side= NULL;
	if(!node){
		return;
	}
	
	// increase count when the function was called
	near_count++;
	int comp_direction = ((node)->depth)%2;
	double distance = cal_distance(node->data->x, node->data->y, xdata, ydata);
	//printf("depth: %d  %f  %f\n", node->depth, distance, *nearest_distance);
	// check if the input node has the nearest distance
	if(distance < *nearest_distance){
		*nearest = node;
		*nearest_distance = distance;
		//printf("change nearest %d\n", (*nearest)->depth);
	}
	// traverse the node along kd tree
	if(comp_direction == 0){
		if(xdata < node->data->x){
			good_side = (node->left); 
			bad_side = (node->right); 
		}else{
			good_side = (node->right);
			bad_side = (node->left); 
		}
	}else if(comp_direction == 1) {
		if(ydata < node->data->y){
			good_side = (node->left); 
			bad_side = (node->right);
		}else{
			good_side = (node->right);
			bad_side = (node->left);
		}   
	}
	// check if traversed node is the nearest neighbor
	find_nearest_neighbor(xdata, ydata, good_side, nearest, nearest_distance);
	// when depth of the tree is 0 and even number
	if(comp_direction == 0){
		// check other side if the distance between data and the x-axis is smaller than smallest distance so far
		if(pow(fabs(xdata - node->data->x),2) < pow(*nearest_distance,2)){
			find_nearest_neighbor(xdata, ydata, bad_side, nearest, nearest_distance);
		}
	// when depth of the tree is odd number
	}else if(comp_direction == 1){
		// check other side if the distance between data and the y-axis is smaller than smallest distance so far
		if(pow(fabs(ydata - node->data->y),2) < pow(*nearest_distance,2)){
			find_nearest_neighbor(xdata, ydata, bad_side, nearest, nearest_distance);
		}
	}

}
// print the nearst neighbor search result to argv[2]
void print_nearest(double xdata, double ydata, struct kdnode *d, FILE *f){
	struct kdnode *temp;  
	fprintf(f, "%lf %lf --> ", xdata, ydata);
	print_data(d->data, f);  
	if(d->duplicate){
            temp = d->duplicate;
            while(temp){
                fprintf(f, "%lf %lf --> ", xdata, ydata);
		        print_data(temp->data, f);  
                temp= temp->duplicate;
            }
    }
	fprintf(stdout, "%lf %lf -->  %d\n", xdata, ydata, near_count);
}