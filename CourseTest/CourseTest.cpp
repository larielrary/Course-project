#include "pch.h"
#include "CppUnitTest.h"
#include "../CourseProject/CourseProject.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CourseTest
{
	TEST_CLASS(CourseTest)
	{
	public:
		
		TEST_METHOD(TestLUTrue)
		{
			int n = 3;
			SLAU slau;
			slau.A = new double* [n];
			slau.B = new double [n];
			for (int i = 0; i < n; i++) slau.A[i] = new double[n];
			slau.A[0][0] = 10;
			slau.A[0][1] = -7;
			slau.A[0][2] = 0;
			slau.B[0] = 1;

			slau.A[1][0] = -3;
			slau.A[1][1] = 6;
			slau.A[1][2] = 2;
			slau.B[1] = 3;

			slau.A[2][0] = 5;
			slau.A[2][1] = -1;
			slau.A[2][2] = 5;
			slau.B[2] = 2;

			Matrix matrix;

			double* actual = new double[n];
			actual = Matrix::LU(slau, n);

			double* rezult = new double[n];

			rezult[0] = 0.067;
			rezult[1] = -0.145;
			rezult[2] = 2.52;
			for (int i = 0; i < n; i++) {
				actual[i] = round(actual[i] * 100) / 100;
				rezult[i] = round(rezult[i] * 100) / 100;
			}
			Assert::AreEqual(actual[2], rezult[2]);

		}
		TEST_METHOD(TestLUFalse)
		{
			int n = 3;
			SLAU slau;
			slau.A = new double* [n];
			slau.B = new double[n];
			for (int i = 0; i < n; i++) slau.A[i] = new double[n];
			slau.A[0][0] = 10;
			slau.A[0][1] = -7;
			slau.A[0][2] = 0;
			slau.B[0] = 1;

			slau.A[1][0] = -3;
			slau.A[1][1] = 6;
			slau.A[1][2] = 2;
			slau.B[1] = 3;

			slau.A[2][0] = 5;
			slau.A[2][1] = -1;
			slau.A[2][2] = 5;
			slau.B[2] = 2;

			Matrix matrix;

			double* actual = new double[n];
			actual = Matrix::LU(slau, n);

			double* rezult = new double[n];

			rezult[0] = 0.067;
			rezult[1] = -0.145;
			rezult[2] = 0.394;
			for (int i = 0; i < n; i++) {
				actual[i] = round(actual[i] * 100) / 100;
				rezult[i] = round(rezult[i] * 100) / 100;
			}
			Assert::AreEqual(actual[2], rezult[2]);
		}
		TEST_METHOD(TestLUParTrue)
		{
			int n = 3;
			SLAU slau;
			slau.A = new double* [n];
			slau.B = new double[n];
			for (int i = 0; i < n; i++) slau.A[i] = new double[n];
			slau.A[0][0] = 10;
			slau.A[0][1] = -7;
			slau.A[0][2] = 0;
			slau.B[0] = 1;

			slau.A[1][0] = -3;
			slau.A[1][1] = 6;
			slau.A[1][2] = 2;
			slau.B[1] = 3;

			slau.A[2][0] = 5;
			slau.A[2][1] = -1;
			slau.A[2][2] = 5;
			slau.B[2] = 2;

			Matrix matrix;

			double* actual = new double[n];
			actual = Matrix::LUPar(slau, n, 4);

			double* rezult = new double[n];

			rezult[0] = 0.067;
			rezult[1] = -0.145;
			rezult[2] = 0.85;
			for (int i = 0; i < n; i++) {
				actual[i] = round(actual[i] * 100) / 100;
				rezult[i] = round(rezult[i] * 100) / 100;
			}
			Assert::AreEqual(actual[2], rezult[2]);
		}
		TEST_METHOD(TestLUParFalse)
		{
			int n = 3;
			SLAU slau;
			slau.A = new double* [n];
			slau.B = new double[n];
			for (int i = 0; i < n; i++) slau.A[i] = new double[n];
			slau.A[0][0] = 10;
			slau.A[0][1] = -7;
			slau.A[0][2] = 0;
			slau.B[0] = 1;

			slau.A[1][0] = -3;
			slau.A[1][1] = 6;
			slau.A[1][2] = 2;
			slau.B[1] = 3;

			slau.A[2][0] = 5;
			slau.A[2][1] = -1;
			slau.A[2][2] = 5;
			slau.B[2] = 2;

			Matrix matrix;

			double* actual = new double[n];
			actual = Matrix::LUPar(slau, n, 4);

			double* rezult = new double[n];

			rezult[0] = 0.067;
			rezult[1] = -0.145;
			rezult[2] = 0.394;
			for (int i = 0; i < n; i++) {
				actual[i] = round(actual[i] * 100) / 100;
				rezult[i] = round(rezult[i] * 100) / 100;
			}
			Assert::AreEqual(actual[2], rezult[2]);
		}
		TEST_METHOD(TestGaussParTrue)
		{
			int n = 3;
			SLAU slau;
			slau.A = new double* [n];
			slau.B = new double[n];
			for (int i = 0; i < n; i++) slau.A[i] = new double[n];
			slau.A[0][0] = 1;
			slau.A[0][1] = 1;
			slau.A[0][2] = 1;
			slau.B[0] = 1;

			slau.A[1][0] = 1;
			slau.A[1][1] = 1;
			slau.A[1][2] = 1;
			slau.B[1] = 1;

			slau.A[2][0] = 6;
			slau.A[2][1] = 5;
			slau.A[2][2] = 5;
			slau.B[2] = 6;

			Matrix matrix;

			double* actual = new double[n];
			actual = Matrix::GaussPar(slau, n, 4);

			double* rezult = new double[n];

			rezult[0] = 0.75;
			rezult[1] = 0.93;
			rezult[2] = -0.17;
			for (int i = 0; i < n; i++) {
				actual[i] = round(actual[i] * 100) / 100;
				rezult[i] = round(rezult[i] * 100) / 100;
			}
			Assert::AreEqual(actual[2], rezult[2]);
		}
		TEST_METHOD(TestGaussParFalse)
		{
			int n = 3;
			SLAU slau;
			slau.A = new double* [n];
			slau.B = new double[n];
			for (int i = 0; i < n; i++) slau.A[i] = new double[n];
			slau.A[0][0] = 10;
			slau.A[0][1] = -7;
			slau.A[0][2] = 0;
			slau.B[0] = 1;

			slau.A[1][0] = -3;
			slau.A[1][1] = 6;
			slau.A[1][2] = 2;
			slau.B[1] = 3;

			slau.A[2][0] = 5;
			slau.A[2][1] = -1;
			slau.A[2][2] = 5;
			slau.B[2] = 2;

			Matrix matrix;

			double* actual = new double[n];
			actual = Matrix::GaussPar(slau, n, 4);

			double* rezult = new double[n];

			rezult[0] = 0.067;
			rezult[1] = -0.145;
			rezult[2] = 0.394;
			for (int i = 0; i < n; i++) {
				actual[i] = round(actual[i] * 100) / 100;
				rezult[i] = round(rezult[i] * 100) / 100;
			}
			Assert::AreEqual(actual[2], rezult[2]);
		}
		TEST_METHOD(CheckZeroLine)
		{
			int n = 3;
			SLAU slau;
			slau.A = new double* [n];
			for (int i = 0; i < n; i++) slau.A[i] = new double[n];
			slau.B = new double[n];

			slau.A[0][0] = 4;
			slau.A[0][1] = 5;
			slau.A[0][2] = 6;
			slau.B[0] = 1;

			slau.A[1][0] = 1;
			slau.A[1][1] = 2;
			slau.A[1][2] = 3;
			slau.B[1] = 78;

			slau.A[2][0] = 0;
			slau.A[2][1] = 0;
			slau.A[2][2] = 0;
			slau.B[2] = 4;

			bool check = CheckMatrix(slau, n);
			Assert::AreEqual(false, check);
		}
	};
}
