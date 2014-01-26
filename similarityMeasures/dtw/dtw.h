/*  
   Author: Sankalp gulati
   email: sankalp.gulati@gmai.com
   Affiliation: Universitat Pompeu Fabra
   
   License: to be decided !!!   
*/

#include <stdlib.h>


#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif


enum 
{
Euclidean=0,
SqEuclidean
};



typedef struct DTW_path
{
  int plen;
  int *px;
  int *py;
}DTW_path;

typedef struct Config
{
	int DistMthd;
	//float mod_val;
	//int mod_dim;
	double *DistWghts;
}Config;

typedef struct MatrixSize
{
	int Nrow;
	int Ncol;
}MatrixSize;

typedef double (*DistMethods)(double, double);

double dtwNd_std(double *x, double*y, MatrixSize*size_x, MatrixSize*size_y, int NFeatDim, double*cost, Config*myConfig);

double dtw1d_std(double *x, double*y, int x_len, int y_len, double*cost, int dist_type);

int path(double *cost, int n, int m, int startx, int starty, DTW_path *p);

double dist4Path(double *x, double*y, MatrixSize*size_x, MatrixSize*size_y, int NFeatDim, DTW_path* path_t, int path_len, Config* myConfig);

double dtw1d_BandConstraint45(double *x, double*y, int x_len, int y_len, double*cost, int dist_type, int bandwidth);

double computeKeoghsLB(double *U, double *L, double * accLB, double *data,int lenMotif, double bsf);

double dtw1dBandConst(double *x, double*y, int x_len, int y_len, double**cost, int dist_type, int bandwidth, double bsf, double *accLB);

double dtw1dBandConst_old(double *x, double*y, int x_len, int y_len, double*cost, int dist_type, int bandwidth, double bsf, double *accLB);