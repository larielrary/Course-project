#include "Matrix.h"
#include <math.h>
#include <list>
#include <iostream>
#include <omp.h>

using namespace std;

class Matrix {
public:
	static double* LU(SLAU slau, int n) {
		double** a = new double* [n];
		for (int i = 0; i < n; i++) a[i] = new double[n];
		double* b = new double[n];
		CopyMatrix(slau.A, slau.B, a, b, n);

		double** L = new double* [n];
		for (int i = 0; i < n; i++) 
			L[i] = new double[n];
		double** U = new double* [n];
		for (int i = 0; i < n; i++)
			U[i] = new double[n];
		for (int i = 0; i < n; i++)
		{
			if (rand() % 2)
				b[i] = 1 - (rand() % 10);
			else
				b[i] = 1 + (rand() % 10);
		}
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				L[i][j] = 0;
				U[i][j] = 0;
			}
		}
		
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (j < i) L[j][i] = 0;
				else
				{
					L[j][i] = a[j][i];
					for (int k = 0; k < i; k++)
						L[j][i] += -L[j][k] * U[k][i];
				}
			}
			for (int j = 0; j < n; j++)
			{
				if (j < i) U[i][j] = 0;
				else if (j == i) U[i][j] = 1;
				else
				{
					U[i][j] = a[i][j] / L[i][i];
					for (int k = 0; k < i; k++)
						U[i][j] += -((L[i][k] * U[k][j]) / L[i][i]);
				}
			}
		}
		double* result = new double[n];


		double* y = new double[n];
		double tempVar = 0;
		for (int i = 0; i < n; i++)
		{
			if (i == 0)
			{
				y[0] = b[0] / L[0][0];
				continue;
			}
			for (int j = 0; j < i; j++)
				tempVar += y[j] * L[i][j];
			y[i] = (b[i] - tempVar) / L[i][i];
			tempVar = 0;
		}

		tempVar = 0;
		for (int i = n - 1; i >= 0; i--)
		{
			if (i == n - 1)
			{
				result[n - 1] = y[n - 1] / U[n - 1][n - 1];
				continue;
			}
			for (int j = n - 1; j > i; j--)
				tempVar += result[j] * U[i][j];
			result[i] = (y[i] - tempVar) / U[i][i];
			tempVar = 0;
		}
		return result;
	}

	static double* LUPar(SLAU slau, int n, int thread) 
	{
		double** a = new double* [n];
		for (int i = 0; i < n; i++) a[i] = new double[n];
		double* b = new double[n];
		CopyMatrix(slau.A, slau.B, a, b, n);

		double** L = new double* [n];
		for (int i = 0; i < n; i++) L[i] = new double[n];
		double** U = new double* [n];
		for (int i = 0; i < n; i++) U[i] = new double[n];
		for (int i = 0; i < n; i++)
		{
			if (rand() % 2)
				b[i] = 1 - (rand() % 10);
			else
				b[i] = 1 + (rand() % 10);
		}
		omp_set_num_threads(thread);
#pragma omp parallel for private(i,j)
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				L[i][j] = 0;
				U[i][j] = 0;
			}
		}
#pragma omp parallel for shared(U,n,k) private(i) schedule(static, 64)
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (j < i) L[j][i] = 0;
				else
				{
					L[j][i] = a[j][i];
					for (int k = 0; k < i; k++)
						L[j][i] += -L[j][k] * U[k][i];
				}
			}
			for (int j = 0; j < n; j++)
			{
				if (j < i) U[i][j] = 0;
				else if (j == i) U[i][j] = 1;
				else
				{
					U[i][j] = a[i][j] / L[i][i];
					for (int k = 0; k < i; k++)
						U[i][j] += -((L[i][k] * U[k][j]) / L[i][i]);
				}
			}
		}
		double* result = new double[n];


		double* y = new double[n];
		double tempVar = 0;
#pragma omp parallel for private(i)
		for (int i = 0; i < n; i++)
		{
			if (i == 0)
			{
				y[0] = b[0] / L[0][0];
				continue;
			}
			for (int j = 0; j < i; j++)
				tempVar += y[j] * L[i][j];
			y[i] = (b[i] - tempVar) / L[i][i];
			tempVar = 0;
		}

		tempVar = 0;
#pragma omp parallel for private(i,n)
		for (int i = n - 1; i >= 0; i--)
		{
			if (i == n - 1)
			{
				result[n - 1] = y[n - 1] / U[n - 1][n - 1];
				continue;
			}
			for (int j = n - 1; j > i; j--)
				tempVar += result[j] * U[i][j];
			result[i] = (y[i] - tempVar) / U[i][i];
			tempVar = 0;
		}
		return result;
	}

	static double* GaussPar(SLAU slau, int n, int thread)
	{
		double** a = new double* [n];
		for (int i = 0; i < n; i++) a[i] = new double[n];
		double* b = new double[n];
		CopyMatrix(slau.A, slau.B, a, b, n);

		double** matrixSLAU = new double* [n];
		for (int i = 0; i < n; i++)
		{
			matrixSLAU[i] = new double[n + 1];
		}
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				matrixSLAU[i][j] = a[i][j];
			}
			matrixSLAU[i][n] = b[i];
		}
		omp_set_num_threads(thread);
#pragma omp parallel for private(i,n)
		for (int i = 0; i < n; i++)
		{
			double maxItem = abs(matrixSLAU[i][i]);
			int maxRow = i;
			for (int k = i + 1; k < n; k++)
			{
				if (abs(matrixSLAU[k][i]) > maxItem)
				{
					maxItem = abs(matrixSLAU[k][i]);
					maxRow = k;
				}
			}
#pragma omp parallel for private(k)
			for (int k = i; k < n + 1; k++)
			{
				double tmp = matrixSLAU[maxRow][k];
				matrixSLAU[maxRow][k] = matrixSLAU[i][k];
				matrixSLAU[i][k] = tmp;
			}
#pragma omp parallel for private(k, j)
			for (int k = i + 1; k < n; k++)
			{
				double c = -matrixSLAU[k][i] / matrixSLAU[i][i];
				for (int j = i; j < n + 1; j++)
				{
					if (i == j)
					{
						matrixSLAU[k][j] = 0;
					}
					else
					{
						matrixSLAU[k][j] += c * matrixSLAU[i][j];
					}
				}
			}
		}
		double* result = new double[n];
#pragma omp parallel for private(i,n,k)
		for (int i = n - 1; i >= 0; i--)
		{
			result[i] = matrixSLAU[i][n] / matrixSLAU[i][i];
			for (int k = i - 1; k >= 0; k--)
			{
				matrixSLAU[k][n] -= matrixSLAU[k][i] * result[i];
			}
		}
		return result;
	}
	static void CopyMatrix(double** A, double* B, double** newA, double* newB, int n) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
			{
				newA[i][j] = A[i][j];
			}
			newB[i] = B[i];
		}
	}

};
