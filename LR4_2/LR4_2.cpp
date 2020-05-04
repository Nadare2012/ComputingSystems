// SelectGrain.cpp : Defines the entry point for the console application.

#include <conio.h>
#include <iomanip>
#include <iostream>
#include <ctime>

#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

using namespace tbb;
using namespace std;

class dot_product_calc
{
	double* p_matrix_;
	double* p_vector_;
	double* p_result_;
	int size_;
public:
	void operator()(const blocked_range<int>& r) const
	{
		const auto begin = r.begin();
		const auto end = r.end();
		for (auto i = begin; i != end; i++)
		{
			p_result_[i] = 0;
			for (auto j = 0; j < size_; j++)
				p_result_[i] += p_matrix_[i * size_ + j] * p_vector_[j];
		}
	}

	dot_product_calc(double* pm, double* pv, double* pr, const int sz) :
		p_matrix_(pm), p_vector_(pv), p_result_(pr), size_(sz)
	{
	}
};

// Function for simple definition of matrix and vector elements 
void dummy_data_initialization(double* p_matrix, double* p_vector,
                               const int size)
{
	for (auto i = 0; i < size; i++)
	{
		p_vector[i] = 1;
		for (auto j = 0; j < size; j++)
			p_matrix[i * size + j] = i;
	}
}

// Function for random definition of matrix and vector elements 
void random_data_initialization(double* p_matrix, double* p_vector,
                                const int size)
{
	srand(static_cast<unsigned>(clock()));
	for (auto i = 0; i < size; i++)
	{
		p_vector[i] = rand() / 1000.0;
		for (auto j = 0; j < size; j++)
			p_matrix[i * size + j] = rand() / 1000.0;
	}
}


// Function for memory allocation and definition of object’s elements 

void process_initialization(double*& p_matrix, double*& p_vector,
                            double*& p_result, int& size, int& grain_size)
{
	// Size of initial matrix and vector definition 
	do
	{
		printf("\nEnter size of the initial objects: ");
		scanf_s("%d", &size);
		printf("\nChosen objects size = %d\n", size);
		if (size <= 0) printf("\nSize of objects must be greater than 0!\n");
	}
	while (size <= 0);

	do
	{
		printf("\nEnter grain size: ");
		scanf_s("%d", &grain_size);
		printf("\nChosen grain size = %d\n", grain_size);
		if (grain_size <= 0) printf("\nGrain size must be greater than 0!\n");
	}
	while (grain_size <= 0);

	// Memory allocation 
	p_matrix = new double[static_cast<long long>(size) * size];
	p_vector = new double[size];
	p_result = new double[size];

	// Definition of matrix and vector elements 
	dummy_data_initialization(p_matrix, p_vector, size);
}

// Function for parallel matrix-vector multiplication 
void parallel_result_calculation(double* p_matrix, double* p_vector,
                                 double* p_result, const int size,
                                 const int grain_size)
{
	parallel_for(blocked_range<int>(0, size, grain_size),
	             dot_product_calc(p_matrix, p_vector, p_result, size));
}


// Function for computational process termination 
void process_termination(const double* p_matrix, const double* p_vector,
                         const double* p_result)
{
	delete[] p_matrix;
	delete[] p_vector;
	delete[] p_result;
}


int main()
{
	double* p_matrix = nullptr;
	double* p_vector = nullptr;
	double* p_result = nullptr;

	auto size = 0;
	auto grain_size = 0;
	task_scheduler_init init;

	// Memory allocation and definition of objects' elements
	printf("Parallel matrix-vector multiplication program\n");
	process_initialization(p_matrix, p_vector, p_result, size, grain_size);

	auto min_duration = DBL_MAX;
	auto min_grain_size = grain_size;
	for (auto current_grain_size = grain_size; current_grain_size >= 1;
	     current_grain_size /= 2)
	{
		// Matrix-vector multiplication. Parallel version based on TBB 
		const auto start = tick_count::now();
		parallel_result_calculation(p_matrix, p_vector, p_result, size,
		                            current_grain_size);
		const auto finish = tick_count::now();
		const auto duration = (finish - start).seconds();
		if (duration < min_duration)
		{
			min_duration = duration;
			min_grain_size = current_grain_size;
		}
		printf("Grain size is %d, execution time is %f s.\n",
		       current_grain_size, duration);
	}

	// Printing the time spent by matrix-vector multiplication 
	printf("Minimal grain size is %d, minimal execution time is %f s.\n",
	       min_grain_size, min_duration);

	// Computational process termination 
	process_termination(p_matrix, p_vector, p_result);
	_getch();
	return 0;
}
