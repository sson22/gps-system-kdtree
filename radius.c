#include "radius.h"

static int radius_count = 0;
// search data sets within the given radius of the data sets from stdin
// print the result to argv[2]
int radius_search(double xdata, double ydata, struct kdnode *node, double radius, int search_count, FILE *f){
	// decide which direction you will visit
	// good_side: direction you will visit
	// bad_side: other direction you may or may not visit depeding on condition 
	struct kdnode *good_side = NULL;
	struct kdnode *bad_side= NULL;
	radius_count++;
	
	if(!node){
		return search_count;
	}
	
	int comp_direction = ((node)->depth)%2;
	double distance = cal_distance(node->data->x, node->data->y, xdata, ydata);
	
	// check if the point is within the radius
	if(distance < radius){  
		struct kdnode *temp;  
		fprintf(f, "%lf %lf %lf --> ", xdata, ydata, radius);
		print_data(node->data, f); 
		if(node->duplicate){
            temp = node;
            while(temp->duplicate){
               fprintf(f, "%lf %lf %lf --> ", xdata, ydata, radius);
		        print_data(node->data, f);  
                temp= temp->duplicate;
            }
        }
		search_count++;
	}
	// check if there's no data found 
	
	
	// traverse the kd tree
	if(comp_direction == 0){
		if(xdata < node->data->x){
			good_side = (node->left); 
			bad_side = (node->right); 
		} else {
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
    // decide whether to go good side or bad side or both 
	// go to good side first
	search_count = radius_search(xdata, ydata, good_side, radius, search_count, f);
	// decide whether to go bad side(other side) as well. 
	if(comp_direction == 0){
		if(fabs(xdata - node->data->x) < radius){
			search_count = radius_search(xdata, ydata, bad_side, radius, search_count, f);
		}
	} else if(comp_direction == 1){
		if(fabs(ydata - node->data->y) < radius){
			search_count = radius_search(xdata, ydata, bad_side, radius, search_count, f);
		}
	}
	return search_count;
}
// print the radius search result
void print_radius(double xdata, double ydata, double radius){
	fprintf(stdout, "%lf %lf %lf -->  %d\n", xdata, ydata, radius, radius_count);
}