# This is a cython wrapper around core dtw module written in C.
#  
#   Author: Sankalp gulati
#   email: sankalp.gulati@gmai.com
#   Affiliation: Universitat Pompeu Fabra
#   
#   License: to be decided !!!   
#

cdef extern from "dtw.h": 
    
	ctypedef struct DTW_path:
		int plen
		int *px
		int *py

	cdef enum:
		Euclidean, SqEuclidean
					
	ctypedef struct MatrixSize:
		int Nrow
		int Ncol			
				
	ctypedef struct Config:
		int DistMthd
		#float mod_val
		#int mod_dim
		double *DistWghts
		
	
	double dtwNd_std(double *x, double*y, MatrixSize*size_x, MatrixSize*size_y, int NFeatDim, double*cost, Config*myConfig)
	double dtw1d_std(double *x, double*y, int x_len, int y_len, double*cost, int dist_type)
	int path(double *cost, int n, int m, int startx, int starty, DTW_path *p)
	double dist4Path(double *x, double*y, MatrixSize*size_x, MatrixSize*size_y, int NFeatDim, DTW_path* path_t, int path_len, Config* myConfig)
	double dtw_std_C(double *x, double*y, int x_len, int y_len, double*cost, int dist_type)