/* 	Assignment 2 for COMP20005, Semester 1, 2018.
	Marishka Nicol Magness, Student ID: 805654
	mmagness@student.unimelb.edu.au  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/* Preset defined values */ 

#define MAXCHAR 1			/* Max characters allowed per tree name */
#define MAXVAL 10000		/* Max trees as input */
#define MEGA 1000000		/* Used to convert values to and from a Mega-prefix */
#define CHECKTEN 10			/* Used to determine if a value is divisable by ten */
#define INC 10				/* Used for incremental skips of 10 steps */
#define XAXISLINECOUNT 9	/* Number of lines between each x-axis node */
#define XAXISNODECOUNT 7	/* Number of nodes on the x-axis */
#define XAXISSPACECOUNT 8	/* Number of space between each node label */
#define XAXISDOMAIN 70		/* X-axis domain */
#define YAXISDOMAIN 60		/* Y-axis domain */
#define YCENTER 1			/* Vertical midpoint of each cell */
#define XCENTER 0.5			/* Horizontal midpoint of each cell */
#define STRESSOVER 1		/* And ratio of stress over this number mean death of tree */



#define TRUE 1				/* For all true values */
#define FALSE 0				/* For all false values */
#define INV -1 				/* For various invalid or unset values */
#define STAGE3 3				/* Stage 3 labels */
#define STAGE4 4				/* Stage 4 labels */

/* Structure Declaration */

/* Sets the maximum number of characters allowed for word_t*/
typedef char word_t[MAXCHAR+1];

/* A structure that stores the information of a single tree */
typedef struct {
	word_t name;		/* Name of tree */
	double xloc;			/* Location of tree on the x-axis */
	double yloc;			/* Location of tree on the y-axis */
	double rootrad;		/* Radius of root extensions from trunk (meters) */
	double rainneed;		/* Rainfall required per year for survival */
	int water;			/* Water needed per year by tree (liters) */
	int squares;			/* Square metres the tree spans */
} treeinfo_t;

/* A structure that stores an array of tree information structures */
typedef struct {
	int tree_count;				/* Total values in array */
	 treeinfo_t tree[MAXVAL];		/* Array of all tree information */
} tree_arr;


/*Function Prototypes */
int tree_info(treeinfo_t *tree);
int conflict_chk(double rootrad1, double rootrad2, double trunk_dist);
double sumofwater(tree_arr alltrees);
double tree_dist(double x1, double y1, double x2, double y2);
int gettreeloc(double yval, double xval, tree_arr *alltrees);
int stressfct(tree_arr *alltrees, double rainfall);
void printtree(treeinfo_t treenum);
void printintinfo(int line_count, double total_water);
void printconflicts(tree_arr alltrees);
void printxaxis(int stage);
void printgraph(int part, tree_arr *alltrees);
void testprint(tree_arr alltrees);
void checkstress(tree_arr *alltrees, double rainfall);




/*Main program that executes the involving functions */
int main(int argc, char *argv[]){
	
	tree_arr alltrees;
	treeinfo_t tree[MAXVAL];
	int i ;
	double totalwater, rainfall;
	char c;
	
	rainfall = atof(argv[1]);
	
	/*Skip the first line containing the header information */
	while (scanf("%c",&c) == 1){			
		if (c == '\n'){
			break;
		}
	}
	
	/*Fills the struct with the given information */
	for (i = 0; i <= MAXVAL; i++){
		if (tree_info(&tree[i]) != EOF){
			alltrees.tree_count += TRUE;
			alltrees.tree[i] = tree[i];
		} else {
			break;
		}	
	}
	
	/*Prints the output of Stage 1 */ 
	totalwater = sumofwater(alltrees);
	printintinfo(alltrees.tree_count, totalwater);
	printf("\n");
	
	/* Prints the output of Stage 2 */
	printconflicts(alltrees);
	printf("\n");
	
	/* Prints the graph of Stage 3 */
	printgraph(STAGE3, &alltrees);
	printf("\n");

	/* Acquire input from commandline regarding annual rainfall */
	printf("S4: rainfall amount = %.1lf\n", rainfall);
	
	/*Calculates stress factors and prints the order of tree deaths (if any) */
	checkstress(&alltrees, rainfall);
	
	/*Prints the graph of Stage 4 */
	printgraph(STAGE4, &alltrees);

	
	return 0;
	
}

/* Function calculates the sum water needed */
double sumofwater(tree_arr alltrees){
	
	int i;
	double sum = 0.0;
	
	for (i = 0; i <= alltrees.tree_count; i++){
		
		sum += alltrees.tree[i].water;
	}
	
	return (sum/MEGA);		/* Sum is returned in Megaliters */
}

/*Calculates the distance of two points */
double tree_dist(double x1, double y1, double x2, double y2){
	
	double dist;
	
	dist = sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
	
	return dist;
}

/* Checks for conflict between two trees.
Returns TRUE if conflict is present
Returns FALSE if no conflict is found
*/
int conflict_chk(double rootrad1, double rootrad2, double trunk_dist){
	
	if (rootrad1 > trunk_dist || rootrad2 > trunk_dist){
		return TRUE;
	} else if (rootrad1 > (trunk_dist - rootrad2)){
		return TRUE;
	} else {
		return FALSE;
	}
	
}

/* Prints the conflicts of every tree on file */
void printconflicts(tree_arr alltrees){
	
	int i, j;
	double trunk_dist;
	
	for (i = 0; i < alltrees.tree_count; i++){
		printf("S2: tree %s is in conflict with ", alltrees.tree[i].name);
		for (j = 0; j < alltrees.tree_count; j++){
			if (j == i){
				/* do nothing */
			} else {
				trunk_dist = tree_dist(
					alltrees.tree[i].xloc, 
					alltrees.tree[i].yloc, 
					alltrees.tree[j].xloc, 
					alltrees.tree[j].yloc);
				if (conflict_chk(
					alltrees.tree[i].rootrad, 
					alltrees.tree[j].rootrad, 
					trunk_dist) == 1){
				printf("%s ", alltrees.tree[j].name);
					}
			}
		}
		printf(" \n");
	}
}

/* Function prints the graphs including axis and are of tree occupation */
void printgraph(int part, tree_arr *alltrees){
	
	double yval, xval;
	int treename;
	
	for (yval = YAXISDOMAIN; yval >=0; yval -= 2){
		/* Print y-axis */
		if ((int)yval%CHECKTEN == 0){
			printf("S%d: %2d +",part,  (int)yval);		/*Prints y axis values */
		} else {
			printf("S%d:    |", part);			/*Prints y axis lines */
		}
		
		/* Prints graph information */
		for (xval = 0; xval < XAXISDOMAIN; xval++){
			if((treename = gettreeloc(yval+YCENTER, xval+XCENTER, alltrees)) != INV){
				printf("%s",alltrees -> tree[treename].name);
			}else{
				printf(" ");
			}
		}
		
		printf("\n");
		
	}
	/*Prints x-axis */
	printxaxis(part);
	printf("\n \n");
			
}

/* Function prints the x-axis of the graph as intended */
void printxaxis(int stage){
	
	int i, j;
	
	printf("S%d:     ", stage);
	
	for (i = 0; i <XAXISNODECOUNT; i ++){
		printf("+");
		for (j = 0; j < XAXISLINECOUNT; j ++){
			printf("-");
		}
	}
	printf("+ \n");
	
	printf("S%d:     ",stage);

	
	for(i=0;i <= XAXISDOMAIN; i += INC){
		printf("%d", i);
		for(j = 0; j <XAXISSPACECOUNT; j++){
			printf(" ");
		}
	}
	
}

/* Function returns the array number of the tree present at the coordinates provided */
int gettreeloc(double yval, double xval, tree_arr *alltrees){
	

	int i, claim;
	double claim_dist = 0.0, treeloc = 0.0;
	
	
	claim = INV;  
	
	for (i = 0; i < alltrees -> tree_count; i++){
		/* Calculate the distance between the trunk and location on region map */
		if((alltrees->tree[i].name[0] == '?')){
			continue;
		}
		
		treeloc = tree_dist(
			xval, 
			yval, 
			alltrees -> tree[i].xloc, 
			alltrees -> tree[i].yloc);

		/* location is within the root radius */
		if(treeloc < alltrees -> tree[i].rootrad){	
			
			/* Trunk closest to location on map */
			if(treeloc < claim_dist || claim_dist == 0.0){	
				claim_dist = treeloc;
				claim = i;
			}
		}
	}
	
	/* Counts number of squares each tree occupies */
	if (claim != INV){
		alltrees -> tree[claim].squares += TRUE;
	}
	
	return claim;
	
}

/* Function calculates stressfactors and prints the next tree death.

Returns TRUE(1) if a tree has died.
Returns FALSE(0) if no trees dies.

*/
int stressfct(tree_arr *alltrees, double rainfall){
	
	int i, dead;
	word_t dead_tree;
	double sf = 0.0;
	
	struct stressfctr{
		word_t name;
		double stressfactor;
	} ;
	
	struct stressfctr stressfctrs[MAXVAL];

	for (i = 0; i < alltrees -> tree_count; i++){
		/* Exclude Trees that have died */
		if(alltrees->tree[i].name[0] == '?'){
			continue;
		}
		/* Calculate the annual rainfall needed for survival of each tree */
		alltrees -> tree[i].rainneed = 
		(double)(alltrees -> tree[i].water)/((alltrees -> tree[i].squares)*2);
		
		strcpy(stressfctrs[i].name, alltrees->tree[i].name);
		
		/*Calculate stress factor */
		stressfctrs[i].stressfactor =
		alltrees -> tree[i].rainneed/rainfall;
	
		/* Check for stress factor for more than 1 */
		if(stressfctrs[i].stressfactor >= STRESSOVER){
			
			/*Finds the first tree to die first */
			if (stressfctrs[i].stressfactor > sf ||sf == 0.0){
				sf = stressfctrs[i].stressfactor;
				strcpy(dead_tree, stressfctrs[i].name);
				dead = i;
			}
		}
	}
		
	if (sf != 0.0){
		printf("S4: tree %s has stress factor %.2lf and dies next\n", dead_tree, sf);
		
		/*Changes the names of dead trees to ? as indication of death */
		strcpy(alltrees->tree[dead].name, "?");
		return TRUE;
	} 
	
	return FALSE;
}

/*Function calculates every next tree death until no tree dies */
void checkstress(tree_arr *alltrees, double rainfall){
	int i, x, y;
	
	while(stressfct(alltrees, rainfall) != FALSE){
		for (i = 0; i < alltrees -> tree_count; i++){
			/*Resets the area tree occupies */
			alltrees-> tree[i].squares = 0;
		}
		 
		for(y = YAXISDOMAIN; y >=0; y -= 2){
			for (x = 0; x <= XAXISDOMAIN; x++){
				gettreeloc(y+YCENTER, x+XCENTER, alltrees);
			}
		}
	}
	printf("\n");
}
				
		
	
		
/* Prints the whole array of structures given */
void testprint(tree_arr alltrees){
	int i ;
	
	for (i = 0; i < alltrees.tree_count; i++){
		printf("name:%s\nxloc: %lf\nyloc: %lf\nrootrad: %lf\n"
		"water: %d\nsquares: %d\nrainneed: %lf\n",
			alltrees.tree[i].name,
			alltrees.tree[i].xloc,
			alltrees.tree[i].yloc,
			alltrees.tree[i].rootrad,
			alltrees.tree[i].water,
			alltrees.tree[i].squares,
			alltrees.tree[i].rainneed);
	}
}

/* 

Functions below are referenced from: 
Assignment 1 for COMP20005, Semester 1, 2018.
by Marishka Nicol Magness.

*/

/* Function prints general information from the data recieved */
void printintinfo(int line_count, double total_water){
	
	/* Total data lines */
	printf("S1: total data lines   =     %d trees", line_count);
	printf("\n");
	
	/* Total water needed */
	printf("S1: total water needed = %.3lf megaliters per year", total_water);
	printf("\n");
	
}

/* 

Functions below are referenced from: 
Programmming, Problem Solving, and Abstraction with C Revised Edition 
by Alistair Moffat .

*/

/* Function assigns data gotten into the preset struct */
int tree_info(treeinfo_t *tree){
	int validcheck;
	
	validcheck = scanf("%s %lf %lf %d %lf", 
		tree -> name,
		&tree -> xloc,
		&tree -> yloc,
		&tree -> water,
		&tree -> rootrad);
	tree -> squares = FALSE;
	if (validcheck != 5){
	 	return EOF;
	} else {
		return 0;
	}
}