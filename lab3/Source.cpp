#include <iostream>
#include <ctime>
#include <omp.h>
using namespace std;

int main() {
	int n = 10000;

	int* x = new int[n];
	int** A = new int* [n];
	for (int i = 0; i < n; i++)
	{
		A[i] = new int[n];
	}

	int* result_vector = new int[n];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			A[i][j] = 1;
	}
	for (int i = 0; i < n; i++) {
		x[i] = 1;
	}
	unsigned int start = clock();
#pragma omp parallel for schedule(static, 128)
	for (int i = 0; i < n; i++)
	{
		result_vector[i] = 0;

		for (int j = 0; j < n; j++)
		{
			result_vector[i] += A[i][j] * x[j];
		}
	}
	unsigned int end = clock();
	cout << "Static parallel: " << end - start << endl;
	float parallel_time = end - start;

//	start = clock();
//#pragma omp parallel for schedule(guided)
//	for (int i = 0; i < n; i++)
//	{
//		result_vector[i] = 0;
//
//		for (int j = 0; j < n; j++)
//		{
//			result_vector[i] += A[i][j] * x[j];
//		}
//	}
//	end = clock();
//	cout << "Guided parallel: " << end - start << endl;

	start = clock();
	for (int i = 0; i < n; i++)
	{
		result_vector[i] = 0;

		for (int j = 0; j < n; j++)
		{
			result_vector[i] += A[i][j] * x[j];
		}
	}
	end = clock();
	cout << "Not parallel: " << end - start << endl;
	float not_parallel_time = end - start;
	cout << not_parallel_time / parallel_time;
	delete[] x;
	delete[] result_vector;
	for (int i = 0; i < n; i++)
	{
		delete[] A[i];
	}
	return 0;
}