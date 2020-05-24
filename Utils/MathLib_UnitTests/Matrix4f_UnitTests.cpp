#include "pch.h"
#include "CppUnitTest.h"

#include "Matrix4f.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathLib_UnitTests
{
	TEST_CLASS(Matrix4f_UnitTests)
	{
	public:
		const float TOLERANCE = 0.001f;

		/*
		* Tests an empty matrix is initialised correctly
		*/
		TEST_METHOD(EmptyMatrixInit)
		{
			Matrix4f matrix;

			const float expected00 = 0.0f;
			const float expected01 = 0.0f;
			const float expected02 = 0.0f;
			const float expected03 = 0.0f;

			const float expected10 = 0.0f;
			const float expected11 = 0.0f;
			const float expected12 = 0.0f;
			const float expected13 = 0.0f;

			const float expected20 = 0.0f;
			const float expected21 = 0.0f;
			const float expected22 = 0.0f;
			const float expected23 = 0.0f;

			const float expected30 = 0.0f;
			const float expected31 = 0.0f;
			const float expected32 = 0.0f;
			const float expected33 = 0.0f;

			Assert::AreEqual(expected00, matrix[0][0], TOLERANCE);
			Assert::AreEqual(expected01, matrix[0][1], TOLERANCE);
			Assert::AreEqual(expected02, matrix[0][2], TOLERANCE);
			Assert::AreEqual(expected03, matrix[0][3], TOLERANCE);

			Assert::AreEqual(expected10, matrix[1][0], TOLERANCE);
			Assert::AreEqual(expected11, matrix[1][1], TOLERANCE);
			Assert::AreEqual(expected12, matrix[1][2], TOLERANCE);
			Assert::AreEqual(expected13, matrix[1][3], TOLERANCE);

			Assert::AreEqual(expected20, matrix[2][0], TOLERANCE);
			Assert::AreEqual(expected21, matrix[2][1], TOLERANCE);
			Assert::AreEqual(expected22, matrix[2][2], TOLERANCE);
			Assert::AreEqual(expected23, matrix[2][3], TOLERANCE);

			Assert::AreEqual(expected30, matrix[3][0], TOLERANCE);
			Assert::AreEqual(expected31, matrix[3][1], TOLERANCE);
			Assert::AreEqual(expected32, matrix[3][2], TOLERANCE);
			Assert::AreEqual(expected33, matrix[3][3], TOLERANCE);
		}

		/*
		* Tests a vector initialised with matrix is initialised correctly
		*/
		TEST_METHOD(MatrixInitWithValues)
		{
			Matrix4f matrix(
				 1.0f,  2.0f,  3.0f,  4.0f,
				 5.0f,  6.0f,  7.0f,  8.0f,
				 9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			const float expected00 = 1.0f;
			const float expected01 = 2.0f;
			const float expected02 = 3.0f;
			const float expected03 = 4.0f;

			const float expected10 = 5.0f;
			const float expected11 = 6.0f;
			const float expected12 = 7.0f;
			const float expected13 = 8.0f;

			const float expected20 = 9.0f;
			const float expected21 = 10.0f;
			const float expected22 = 11.0f;
			const float expected23 = 12.0f;

			const float expected30 = 13.0f;
			const float expected31 = 14.0f;
			const float expected32 = 15.0f;
			const float expected33 = 16.0f;

			Assert::AreEqual(expected00, matrix[0][0], TOLERANCE);
			Assert::AreEqual(expected01, matrix[0][1], TOLERANCE);
			Assert::AreEqual(expected02, matrix[0][2], TOLERANCE);
			Assert::AreEqual(expected03, matrix[0][3], TOLERANCE);

			Assert::AreEqual(expected10, matrix[1][0], TOLERANCE);
			Assert::AreEqual(expected11, matrix[1][1], TOLERANCE);
			Assert::AreEqual(expected12, matrix[1][2], TOLERANCE);
			Assert::AreEqual(expected13, matrix[1][3], TOLERANCE);

			Assert::AreEqual(expected20, matrix[2][0], TOLERANCE);
			Assert::AreEqual(expected21, matrix[2][1], TOLERANCE);
			Assert::AreEqual(expected22, matrix[2][2], TOLERANCE);
			Assert::AreEqual(expected23, matrix[2][3], TOLERANCE);

			Assert::AreEqual(expected30, matrix[3][0], TOLERANCE);
			Assert::AreEqual(expected31, matrix[3][1], TOLERANCE);
			Assert::AreEqual(expected32, matrix[3][2], TOLERANCE);
			Assert::AreEqual(expected33, matrix[3][3], TOLERANCE);
		}

		/*
		*
		*/
		TEST_METHOD(MatrixCopyConstructor)
		{

		}

		/*
		* Tests the overloaded assignment operator
		*/
		TEST_METHOD(MatrixAssignment)
		{

		}

		/*
		* Tests overloaded addition operator
		*/
		TEST_METHOD(MatrixAddition)
		{

		}

		/*
		* Tests overloaded addition-assignment operator
		*/
		TEST_METHOD(MatrixAdditionAssignment)
		{

		}

		/*
		* Tests overloaded subtraction operator
		*/
		TEST_METHOD(MatrixSubtraction)
		{

		}

		/*
		* Tests overloaded subtraction-assignment operator
		*/
		TEST_METHOD(MatrixSubtractionAssignment)
		{

		}

		/*
		* Tests overloaded multiplication operator
		*/
		TEST_METHOD(MatrixMultiplication)
		{

		}

		/*
		* Tests overloaded equality operator
		*/
		TEST_METHOD(MatrixEqualityCheck)
		{

		}

		/*
		* Tests overloaded inequality operator
		*/
		TEST_METHOD(MatrixInequalityCheck)
		{

		}

		/*
		* Tests overloaded scalar addition operator
		*/
		TEST_METHOD(MatrixScalarAddition)
		{

		}

		/*
		* Tests overloaded scalar addition-assignment operator
		*/
		TEST_METHOD(MatrixScalarAdditionAssignment)
		{

		}

		/*
		* Tests overloaded scalar subtraction operator
		*/
		TEST_METHOD(MatrixScalarSubtraction)
		{

		}

		/*
		* Tests overloaded scalar subtraction-assignment operator
		*/
		TEST_METHOD(MatrixScalarSubtractionAssignment)
		{

		}

		/*
		* Tests overloaded scalar multiplication operator
		*/
		TEST_METHOD(MatrixScalarMultiplication)
		{

		}

		/*
		* Tests overloaded scalar multiplication-assignment operator
		*/
		TEST_METHOD(MatrixScalarMultiplicationAssignment)
		{

		}

		/*
		* Tests matrix transposition is correct
		*/
		TEST_METHOD(MatrixTranspose)
		{

		}

		/*
		* Tests the determinant of a matrix is calculated correctly
		*/
		TEST_METHOD(MatrixDeterminant)
		{

		}

		/*
		* Tests the inverse of a matrix is calculated correctly
		*/
		TEST_METHOD(MatrixInverse)
		{

		}

		/*
		* Tests the eigenvalue of a matrix is calculated correctly
		*/
		TEST_METHOD(MatrixEigenvalue)
		{

		}

		/*
		* Tests the eigenvector of a matrix is calculated correctly
		*/
		TEST_METHOD(MatrixEigenvector)
		{

		}
	};
}