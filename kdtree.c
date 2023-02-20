#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "kdtree.h"

// demonstration: using getdelim to read .csv
// note that it is simpler just to getline and parse string char by char



//------ HEADER STUFFS: reading column names of csv file  -----------------
// global variables & const for use in this module only
#define NUM_FIELDS 11  // number of fields, assigned for simplicity
static char *dname[NUM_FIELDS];   // the header

void data_get_header(FILE *f) {
	int i;
	for (i=0; i<NUM_FIELDS-1; i++) {
		dname[i]= getfield(f,C);
	}
	dname[i]= getfield(f,NL);
}
void data_free_header() {
	int i;
	for (i=0; i<NUM_FIELDS; i++) {
		free(dname[i]);
	}
}

//--------------------- END OF HEADER STUFFS -----------------
// chop the last char of s if it is delim	
void chop(char *s, char delim) {
	int l= strlen(s)- 1;
	// first: shop \n or \r (Windows problem!) 
	for (; l>=0 && isspace(s[l]) ; l--) s[l]= '\0';
	if (s[l]==delim) s[l]= '\0';
}

// copying s top t, reducing "" to "
//     return 0 if s contains final "
//            1 otherwise (ie. s not yet finished)
int scopy(char *t, char *s, char deli) {
	char *p= t, *q= s;
	char prev=' ';
	while (*q) {
		if (*q==Q && prev!=Q && (*(q+1)!=Q) ) {
			*p= '\0'; 
			return 0;
		}
		if (*q==Q && prev==Q) {
			prev= ' ';
			q++;
			continue;
		}
		prev= *q;
		*p++= *q++;
	}
	*p= '\0';
    return 1;
}

// get & return next csv field, which might or might not start with "
//    the field deds withj "deli", which can be  a comma or a newline
char *getfield(FILE *f, char deli) {
	char *s=NULL;
	size_t l= 0;

	// get first component
    if (getdelim(&s, &l, deli, f) <=0 ) {
		if (s) free(s);
		return NULL;
	}  
	if (*s != Q) {  // simple return for field not starting with "
		chop(s, deli);
		return s;
	}

	char line[FIELD_LEN+3]; // buffer, +3 for possible \r \n \0
	int cont= scopy(line, s+1, deli);  // add first component to buffer

	while (cont) {
		char *p= line+strlen(line);
		getdelim(&s, &l, deli, f);
		cont= scopy(p, s, deli);
	}
    free(s);
	return my_strdup(line);
}	

data_t *get_next_data(FILE *f) {
	char *s= getfield(f,C);
	if (!s) return NULL;

	data_t *d= my_malloc(sizeof(*d));
    d->year= s;
	d->bid= getfield(f,C);
	d->pid= getfield(f,C); 
	d->bpid= getfield(f,C);
	d->clue= getfield(f,C);
	d->name= getfield(f,C);
	d->code= getfield(f,C);
	d->desc= getfield(f,C);
	//To use x, y later for comparision, change to double 
	d->x= atof(getfield(f,C));
	d->y= atof(getfield(f,C));
	// To localize reading errors, read until \n here
	d->loc= getfield(f,NL);
	return d;
}
// print datas in data struct
void print_data(data_t *d, FILE *f) {
	int i;
	for (i=0; i<NUM_FIELDS; i++) {
		 fprintf(f, "%s: ", dname[i]);
		switch (i) {
			case 0:
				fprintf(f,"%s", d->year);
				break;
			case 1:
				fprintf(f,"%s", d->bid);
				break;
			case 2:
				fprintf(f,"%s", d->pid);
				break;
			case 3:
				fprintf(f,"%s", d->bpid);
				break;
			case 4:
				fprintf(f,"%s", d->clue);
				break;
			case 5:
				fprintf(f,"%s", d->name);
				break;
			case 6:
				fprintf(f,"%s", d->code);
				break;
			case 7:
				fprintf(f,"%s", d->desc);
				break;
			case 8:
				fprintf(f,"%lf", d->x);
				break;
			case 9:
				fprintf(f,"%lf", d->y);
				break;
			case 10:
				fprintf(f,"%s", d->loc);
				break;
		}
		if (i!=5) fprintf(f, " || ");
	}
	fprintf(f,"\n");
}
// create node and assign null
struct kdnode *create_list() {
	return NULL;
}
// insert data into kd tree
struct kdnode *kd_insert(struct data *data, struct kdnode *parent){
	if(parent == NULL){
	parent = (struct kdnode *)my_malloc(sizeof(struct kdnode));
	parent->left = NULL;
	parent->right = NULL;
	parent->data = data;
	parent->depth = 0;
	parent->duplicate = NULL;
	
	return parent;
	}
	struct kdnode **insert_location = &parent;//if the parent was the null pointer, then we want to feel it up.
	struct kdnode *current_parent = parent;
   
	// decide whether compare x value or y value to find the insert location 
	int comp_direction = 0; 
	// if insert location is not null
	while(*insert_location){
		comp_direction = ((*insert_location)->depth)%2;
		// for nodes where the depth level is 0 and even number (0,2,4...) 
		// compares x values to find insert location 
		if(comp_direction == 0){
			// go left
			if(data->x < (*insert_location)->data->x ){
				current_parent = *insert_location;
				insert_location = &((*insert_location)->left); 
			// go right
			} else if(data->x == (*insert_location)->data->x && data->y == (*insert_location)->data->y) {
				current_parent = *insert_location;
				break;
			}else{
				current_parent = *insert_location;
				insert_location = &((*insert_location)->right);
			}
		// for nodes where the depth level is odd number(1,3,5...) 
		//compares y values to find insert location 
		} else if (comp_direction == 1) {
			// go left
			if(data->y < (*insert_location)->data->y) {
				current_parent = *insert_location;
				insert_location = &((*insert_location)->left); 
			// go right
			} else if(data->x == (*insert_location)->data->x && data->y == (*insert_location)->data->y) {
				current_parent = *insert_location;
				break;
			}else {
				current_parent = *insert_location;
				insert_location = &((*insert_location)->right);
			}
		}
	} 
	//printf("%f %f\n", current_parent->data->x,current_parent->data->y );
	//printf("%f %f\n", data->x, data->y);
	if(current_parent){
		// handle duplicate key, start linked list of duplicate keys 
		if((current_parent->data->x == data->x )&& (current_parent->data->y == data->y)){
			//printf("duplicate \n");
			struct kdnode *current_node = NULL;
			struct kdnode *new_node = (struct kdnode *)my_malloc(sizeof(struct kdnode));
			new_node-> left = NULL;
			new_node-> right = NULL;
			new_node-> parent = NULL;
			new_node->duplicate = NULL;
			new_node-> depth = 0;
			new_node->data = data;
			// handle first duplicate key, start linked list 
			if(current_parent->duplicate == NULL){
				current_parent->duplicate = new_node;
			} else {
				// handle if there's duplicate key linked list exists already 
				current_node = current_parent;
				while(current_node->duplicate){
					current_node = current_node->duplicate;
				}
				current_node->duplicate = new_node;
			}
			return parent;           
		}
	}        
	//printf("count %d\n", count);
	// allocate space for new child node and iniialise it
	*insert_location = (struct kdnode *)my_malloc(sizeof(struct kdnode));
	(*insert_location)->left = NULL;
	(*insert_location)->right = NULL;
	(*insert_location)->data = data;
	(*insert_location)->depth = 0;
	(*insert_location)->duplicate = NULL;
	
	// handles depth
	(*insert_location)->parent = current_parent;
	if(current_parent){
		(*insert_location)-> depth = current_parent -> depth + 1; 
	} else {
		(*insert_location)-> depth = 0;
	}
    return parent;
}

// calculate distance between two points
double cal_distance(double parentx, double parenty, double xdata, double ydata){
	return sqrt(pow(parentx - xdata, 2) + pow(parenty - ydata ,2));
}

// free data space
void free_data(struct data *data){
	free(data->bid);
	free(data->bpid);
	free(data->clue);
	free(data->code);
	free(data->desc);
	free(data->loc);
	free(data->name);
	free(data->pid);
	free(data->year);
}
//free kd tree 
void free_tree(struct kdnode *parent){
	if(!parent){
		return;
	}
	free_tree(parent->left);
	free_tree(parent->right);
	if(parent->duplicate){
        struct kdnode *temp = parent->duplicate;
         while(temp!=NULL){
            struct kdnode *t= temp;
		    temp= temp->duplicate;
            free_data(t->data);
		    free(t->data);
		    free(t);
	    }
    }
	free_data(parent->data);
	free(parent->data);
	free(parent);    
}
