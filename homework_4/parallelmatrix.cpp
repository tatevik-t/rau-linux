#include "parallelmatrix.h"
#include <stdlib.h> 
#include <time.h> 
#include <pthread.h>

// constructor with arguments n*m
ParallelMatrix::ParallelMatrix(int n_, int m_) :  n(n_), m(m_)
{
    matrix = new int*[n];
    matrix_all = new int[n*m];
    for(int i = 0; i < n; ++i){
        matrix[i] = &matrix_all[i*m];
    }
}

// destructor
ParallelMatrix::~ParallelMatrix()
{
    delete [] matrix_all;
    delete [] matrix;
}

// initialize the matrix with random ints
void ParallelMatrix::Init()
{
    // srand for seed to make random numbers a little bit randomer
    srand (time(NULL));

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            matrix[i][j] = rand() % 10000;
        }
    }
}

// method for normal sum calculation
int ParallelMatrix::Sum(){
    int sum = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            sum += matrix[i][j];
        }
    }
    return sum;
}

// struct for arguments for sum_arr function
struct sum_args
{
    int start_idx;
    int count;
    ParallelMatrix* m;
};

// friend function for thread sum calculation 
void* sum_arr(void* arg){
    // casting arguments
    sum_args* args = (sum_args*) arg;
	int local_sum = 0;

    // run through matrix assuming it is an array
	for(int i = args->start_idx; i < args->start_idx + args->count; i++)
	{
		local_sum += args->m->matrix_all[i];
	}

    // recasting int
	return (void*) (new int(local_sum));
}

int ParallelMatrix::SumParallel(int threads_n){
    int sum = 0;
    
    // threads' array
    pthread_t* threads = new pthread_t[threads_n];
    // argumnets' array
	sum_args* arguments = new sum_args[threads_n]; 
	
    // create threads
    for(int i = 0; i < threads_n; ++i)
	{
		arguments[i].start_idx = i*(n*m/threads_n);
		arguments[i].count = n*m / threads_n;
        arguments[i].m = this;
		int result = pthread_create(&threads[i], NULL, sum_arr, &arguments[i]);
		if(result != 0)
		{
			exit(result);
		}
	}
	
	void* local_sum;
    // join threads
	for(int i = 0; i < threads_n; ++i)
	{
		int joined = pthread_join(threads[i], &local_sum);
		if (joined != 0)
		{
			exit(joined);
		}
		int* local_sum_int = (int*) local_sum;
		sum += *local_sum_int;
		delete local_sum_int;
	}
	delete [] threads;
	delete [] arguments;
    return sum;
}