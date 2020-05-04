#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include "tbb/tick_count.h"

using namespace tbb;

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
                            double*& p_result, int& size)
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

	// Memory allocation 
	p_matrix = new double[static_cast<long long>(size) * size];
	p_vector = new double[size];
	p_result = new double[size];

	// Definition of matrix and vector elements 
	random_data_initialization(p_matrix, p_vector, size);
}

// Function for formatted matrix output 
void print_matrix(double* p_matrix, const int row_count, const int col_count)
{
	for (auto i = 0; i < row_count; i++)
	{
		for (auto j = 0; j < col_count; j++)
			printf("%7.4f ", p_matrix[i * row_count + j]);
		printf("\n");
	}
}

// Function for formatted vector output 
void print_vector(double* p_vector, const int size)
{
	for (auto i = 0; i < size; i++)
		printf("%7.4f ", p_vector[i]);
}

// Function for matrix-vector multiplication 
void result_calculation(const double* p_matrix, const double* p_vector,
                        double* p_result, const int size)
{
	for (auto i = 0; i < size; i++)
	{
		p_result[i] = 0;
		for (auto j = 0; j < size; j++)
			p_result[i] += p_matrix[i * size + j] * p_vector[j];
	}
}

// Function for computational process termination 
void process_termination(const double* p_matrix, const double* p_vector,
                         const double* p_result)
{
	delete[] p_matrix;
	delete[] p_vector;
	delete[] p_result;
}

void main()
{
	double* p_matrix = nullptr; // The first argument - initial matrix 
	double* p_vector = nullptr; // The second argument - initial vector 
	double* p_result = nullptr; // Result vector for matrix-vector 
	auto size = 0; // Sizes of initial matrix and vector 

	// Memory allocation and definition of objects' elements 
	printf("Serial matrix-vector multiplication program\n");
	process_initialization(p_matrix, p_vector, p_result, size);

	// Matrix and vector output 
	printf("Initial Matrix \n");
	print_matrix(p_matrix, size, size);
	printf("Initial Vector \n");
	print_vector(p_vector, size);

	// Matrix-vector multiplication 
	const auto start = tick_count::now();
	result_calculation(p_matrix, p_vector, p_result, size);
	const auto finish = tick_count::now();
	const auto duration = (finish - start).seconds();

	// Printing the result vector 
	printf("\n Result Vector: \n");
	print_vector(p_result, size);

	// Printing the time spent by matrix-vector multiplication 
	printf("\n Time of execution: %f\n", duration);

	// Computational process termination 
	process_termination(p_matrix, p_vector, p_result);
	_getch();
}
