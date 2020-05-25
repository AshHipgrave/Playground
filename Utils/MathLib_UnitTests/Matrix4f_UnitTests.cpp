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
			Matrix4f mat1;

			const float expectedValue = 0.0f;

			Assert::AreEqual(expectedValue, mat1[0][0], TOLERANCE);
			Assert::AreEqual(expectedValue, mat1[0][1], TOLERANCE);
			Assert::AreEqual(expectedValue, mat1[0][2], TOLERANCE);
			Assert::AreEqual(expectedValue, mat1[0][3], TOLERANCE);

			Assert::AreEqual(expectedValue, mat1[1][0], TOLERANCE);
			Assert::AreEqual(expectedValue, mat1[1][1], TOLERANCE);
			Assert::AreEqual(expectedValue, mat1[1][2], TOLERANCE);
			Assert::AreEqual(expectedValue, mat1[1][3], TOLERANCE);

			Assert::AreEqual(expectedValue, mat1[2][0], TOLERANCE);
			Assert::AreEqual(expectedValue, mat1[2][1], TOLERANCE);
			Assert::AreEqual(expectedValue, mat1[2][2], TOLERANCE);
			Assert::AreEqual(expectedValue, mat1[2][3], TOLERANCE);

			Assert::AreEqual(expectedValue, mat1[3][0], TOLERANCE);
			Assert::AreEqual(expectedValue, mat1[3][1], TOLERANCE);
			Assert::AreEqual(expectedValue, mat1[3][2], TOLERANCE);
			Assert::AreEqual(expectedValue, mat1[3][3], TOLERANCE);
		}

		/*
		* Tests a vector initialised with matrix is initialised correctly
		*/
		TEST_METHOD(MatrixInitWithValues)
		{
			Matrix4f mat1(
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

			Assert::AreEqual(expected00, mat1[0][0], TOLERANCE);
			Assert::AreEqual(expected01, mat1[0][1], TOLERANCE);
			Assert::AreEqual(expected02, mat1[0][2], TOLERANCE);
			Assert::AreEqual(expected03, mat1[0][3], TOLERANCE);

			Assert::AreEqual(expected10, mat1[1][0], TOLERANCE);
			Assert::AreEqual(expected11, mat1[1][1], TOLERANCE);
			Assert::AreEqual(expected12, mat1[1][2], TOLERANCE);
			Assert::AreEqual(expected13, mat1[1][3], TOLERANCE);

			Assert::AreEqual(expected20, mat1[2][0], TOLERANCE);
			Assert::AreEqual(expected21, mat1[2][1], TOLERANCE);
			Assert::AreEqual(expected22, mat1[2][2], TOLERANCE);
			Assert::AreEqual(expected23, mat1[2][3], TOLERANCE);

			Assert::AreEqual(expected30, mat1[3][0], TOLERANCE);
			Assert::AreEqual(expected31, mat1[3][1], TOLERANCE);
			Assert::AreEqual(expected32, mat1[3][2], TOLERANCE);
			Assert::AreEqual(expected33, mat1[3][3], TOLERANCE);
		}

		/*
		* Tests copy ctor works correctly
		*/
		TEST_METHOD(MatrixCopyConstructor)
		{
			const Matrix4f mat1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4f mat2 = mat1;

			Assert::AreEqual(mat1[0][0], mat2[0][0], TOLERANCE);
			Assert::AreEqual(mat1[0][1], mat2[0][1], TOLERANCE);
			Assert::AreEqual(mat1[0][2], mat2[0][2], TOLERANCE);
			Assert::AreEqual(mat1[0][3], mat2[0][3], TOLERANCE);

			Assert::AreEqual(mat1[1][0], mat2[1][0], TOLERANCE);
			Assert::AreEqual(mat1[1][1], mat2[1][1], TOLERANCE);
			Assert::AreEqual(mat1[1][2], mat2[1][2], TOLERANCE);
			Assert::AreEqual(mat1[1][3], mat2[1][3], TOLERANCE);

			Assert::AreEqual(mat1[2][0], mat2[2][0], TOLERANCE);
			Assert::AreEqual(mat1[2][1], mat2[2][1], TOLERANCE);
			Assert::AreEqual(mat1[2][2], mat2[2][2], TOLERANCE);
			Assert::AreEqual(mat1[2][3], mat2[2][3], TOLERANCE);

			Assert::AreEqual(mat1[3][0], mat2[3][0], TOLERANCE);
			Assert::AreEqual(mat1[3][1], mat2[3][1], TOLERANCE);
			Assert::AreEqual(mat1[3][2], mat2[3][2], TOLERANCE);
			Assert::AreEqual(mat1[3][3], mat2[3][3], TOLERANCE);
		}

		/*
		* Tests the overloaded assignment operator
		*/
		TEST_METHOD(MatrixAssignment)
		{
			const Matrix4f mat1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4f mat2;

			mat2 = mat1;

			Assert::AreEqual(mat1[0][0], mat2[0][0], TOLERANCE);
			Assert::AreEqual(mat1[0][1], mat2[0][1], TOLERANCE);
			Assert::AreEqual(mat1[0][2], mat2[0][2], TOLERANCE);
			Assert::AreEqual(mat1[0][3], mat2[0][3], TOLERANCE);

			Assert::AreEqual(mat1[1][0], mat2[1][0], TOLERANCE);
			Assert::AreEqual(mat1[1][1], mat2[1][1], TOLERANCE);
			Assert::AreEqual(mat1[1][2], mat2[1][2], TOLERANCE);
			Assert::AreEqual(mat1[1][3], mat2[1][3], TOLERANCE);

			Assert::AreEqual(mat1[2][0], mat2[2][0], TOLERANCE);
			Assert::AreEqual(mat1[2][1], mat2[2][1], TOLERANCE);
			Assert::AreEqual(mat1[2][2], mat2[2][2], TOLERANCE);
			Assert::AreEqual(mat1[2][3], mat2[2][3], TOLERANCE);

			Assert::AreEqual(mat1[3][0], mat2[3][0], TOLERANCE);
			Assert::AreEqual(mat1[3][1], mat2[3][1], TOLERANCE);
			Assert::AreEqual(mat1[3][2], mat2[3][2], TOLERANCE);
			Assert::AreEqual(mat1[3][3], mat2[3][3], TOLERANCE);
		}

		/*
		* Tests overloaded addition operator
		*/
		TEST_METHOD(MatrixAddition)
		{
			const Matrix4f mat1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			const Matrix4f mat2(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			const Matrix4f expectedResult(
				2.0f, 4.0f, 6.0f, 8.0f,
				10.0f, 12.0f, 14.0f, 16.0f,
				18.0f, 20.0f, 22.0f, 24.0f,
				26.0f, 28.0f, 30.0f, 32.0f);

			const Matrix4f calculatedResult = mat1 + mat2;

			Assert::AreEqual(expectedResult[0][0], calculatedResult[0][0], TOLERANCE);
			Assert::AreEqual(expectedResult[0][1], calculatedResult[0][1], TOLERANCE);
			Assert::AreEqual(expectedResult[0][2], calculatedResult[0][2], TOLERANCE);
			Assert::AreEqual(expectedResult[0][3], calculatedResult[0][3], TOLERANCE);

			Assert::AreEqual(expectedResult[1][0], calculatedResult[1][0], TOLERANCE);
			Assert::AreEqual(expectedResult[1][1], calculatedResult[1][1], TOLERANCE);
			Assert::AreEqual(expectedResult[1][2], calculatedResult[1][2], TOLERANCE);
			Assert::AreEqual(expectedResult[1][3], calculatedResult[1][3], TOLERANCE);

			Assert::AreEqual(expectedResult[2][0], calculatedResult[2][0], TOLERANCE);
			Assert::AreEqual(expectedResult[2][1], calculatedResult[2][1], TOLERANCE);
			Assert::AreEqual(expectedResult[2][2], calculatedResult[2][2], TOLERANCE);
			Assert::AreEqual(expectedResult[2][3], calculatedResult[2][3], TOLERANCE);

			Assert::AreEqual(expectedResult[3][0], calculatedResult[3][0], TOLERANCE);
			Assert::AreEqual(expectedResult[3][1], calculatedResult[3][1], TOLERANCE);
			Assert::AreEqual(expectedResult[3][2], calculatedResult[3][2], TOLERANCE);
			Assert::AreEqual(expectedResult[3][3], calculatedResult[3][3], TOLERANCE);
		}

		/*
		* Tests overloaded addition-assignment operator
		*/
		TEST_METHOD(MatrixAdditionAssignment)
		{
			const Matrix4f mat1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4f mat2(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			const Matrix4f expectedResult(
				2.0f, 4.0f, 6.0f, 8.0f,
				10.0f, 12.0f, 14.0f, 16.0f,
				18.0f, 20.0f, 22.0f, 24.0f,
				26.0f, 28.0f, 30.0f, 32.0f);

			mat2 += mat1;

			Assert::AreEqual(expectedResult[0][0], mat2[0][0], TOLERANCE);
			Assert::AreEqual(expectedResult[0][1], mat2[0][1], TOLERANCE);
			Assert::AreEqual(expectedResult[0][2], mat2[0][2], TOLERANCE);
			Assert::AreEqual(expectedResult[0][3], mat2[0][3], TOLERANCE);

			Assert::AreEqual(expectedResult[1][0], mat2[1][0], TOLERANCE);
			Assert::AreEqual(expectedResult[1][1], mat2[1][1], TOLERANCE);
			Assert::AreEqual(expectedResult[1][2], mat2[1][2], TOLERANCE);
			Assert::AreEqual(expectedResult[1][3], mat2[1][3], TOLERANCE);

			Assert::AreEqual(expectedResult[2][0], mat2[2][0], TOLERANCE);
			Assert::AreEqual(expectedResult[2][1], mat2[2][1], TOLERANCE);
			Assert::AreEqual(expectedResult[2][2], mat2[2][2], TOLERANCE);
			Assert::AreEqual(expectedResult[2][3], mat2[2][3], TOLERANCE);

			Assert::AreEqual(expectedResult[3][0], mat2[3][0], TOLERANCE);
			Assert::AreEqual(expectedResult[3][1], mat2[3][1], TOLERANCE);
			Assert::AreEqual(expectedResult[3][2], mat2[3][2], TOLERANCE);
			Assert::AreEqual(expectedResult[3][3], mat2[3][3], TOLERANCE);
		}

		/*
		* Tests overloaded subtraction operator
		*/
		TEST_METHOD(MatrixSubtraction)
		{
			const Matrix4f mat1(
				2.0f, 4.0f, 6.0f, 8.0f,
				10.0f, 12.0f, 14.0f, 16.0f,
				18.0f, 20.0f, 22.0f, 24.0f,
				26.0f, 28.0f, 30.0f, 32.0f);

			const Matrix4f mat2(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			const Matrix4f expectedResult(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			const Matrix4f calculatedResult = mat1 - mat2;

			Assert::AreEqual(expectedResult[0][0], calculatedResult[0][0], TOLERANCE);
			Assert::AreEqual(expectedResult[0][1], calculatedResult[0][1], TOLERANCE);
			Assert::AreEqual(expectedResult[0][2], calculatedResult[0][2], TOLERANCE);
			Assert::AreEqual(expectedResult[0][3], calculatedResult[0][3], TOLERANCE);

			Assert::AreEqual(expectedResult[1][0], calculatedResult[1][0], TOLERANCE);
			Assert::AreEqual(expectedResult[1][1], calculatedResult[1][1], TOLERANCE);
			Assert::AreEqual(expectedResult[1][2], calculatedResult[1][2], TOLERANCE);
			Assert::AreEqual(expectedResult[1][3], calculatedResult[1][3], TOLERANCE);

			Assert::AreEqual(expectedResult[2][0], calculatedResult[2][0], TOLERANCE);
			Assert::AreEqual(expectedResult[2][1], calculatedResult[2][1], TOLERANCE);
			Assert::AreEqual(expectedResult[2][2], calculatedResult[2][2], TOLERANCE);
			Assert::AreEqual(expectedResult[2][3], calculatedResult[2][3], TOLERANCE);

			Assert::AreEqual(expectedResult[3][0], calculatedResult[3][0], TOLERANCE);
			Assert::AreEqual(expectedResult[3][1], calculatedResult[3][1], TOLERANCE);
			Assert::AreEqual(expectedResult[3][2], calculatedResult[3][2], TOLERANCE);
			Assert::AreEqual(expectedResult[3][3], calculatedResult[3][3], TOLERANCE);
		}

		/*
		* Tests overloaded subtraction-assignment operator
		*/
		TEST_METHOD(MatrixSubtractionAssignment)
		{
			const Matrix4f mat1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4f mat2(
				2.0f, 4.0f, 6.0f, 8.0f,
				10.0f, 12.0f, 14.0f, 16.0f,
				18.0f, 20.0f, 22.0f, 24.0f,
				26.0f, 28.0f, 30.0f, 32.0f);

			const Matrix4f expectedResult(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			mat2 -= mat1;

			Assert::AreEqual(expectedResult[0][0], mat2[0][0], TOLERANCE);
			Assert::AreEqual(expectedResult[0][1], mat2[0][1], TOLERANCE);
			Assert::AreEqual(expectedResult[0][2], mat2[0][2], TOLERANCE);
			Assert::AreEqual(expectedResult[0][3], mat2[0][3], TOLERANCE);

			Assert::AreEqual(expectedResult[1][0], mat2[1][0], TOLERANCE);
			Assert::AreEqual(expectedResult[1][1], mat2[1][1], TOLERANCE);
			Assert::AreEqual(expectedResult[1][2], mat2[1][2], TOLERANCE);
			Assert::AreEqual(expectedResult[1][3], mat2[1][3], TOLERANCE);

			Assert::AreEqual(expectedResult[2][0], mat2[2][0], TOLERANCE);
			Assert::AreEqual(expectedResult[2][1], mat2[2][1], TOLERANCE);
			Assert::AreEqual(expectedResult[2][2], mat2[2][2], TOLERANCE);
			Assert::AreEqual(expectedResult[2][3], mat2[2][3], TOLERANCE);

			Assert::AreEqual(expectedResult[3][0], mat2[3][0], TOLERANCE);
			Assert::AreEqual(expectedResult[3][1], mat2[3][1], TOLERANCE);
			Assert::AreEqual(expectedResult[3][2], mat2[3][2], TOLERANCE);
			Assert::AreEqual(expectedResult[3][3], mat2[3][3], TOLERANCE);
		}

		/*
		* Tests overloaded multiplication operator
		*/
		TEST_METHOD(MatrixMultiplication)
		{
			const Matrix4f mat1(
				2.0f, 4.0f, 6.0f, 8.0f,
				10.0f, 12.0f, 14.0f, 16.0f,
				18.0f, 20.0f, 22.0f, 24.0f,
				26.0f, 28.0f, 30.0f, 32.0f);

			const Matrix4f mat2(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			const Matrix4f expectedResult(
				180.0f, 200.0f, 220.0f, 240.0f,
				404.0f, 456.0f, 508.0f, 560.0f,
				628.0f, 712.0f, 796.0f, 880.0f,
				852.0f, 968.0f, 1084.0f, 1200.0f);

			const Matrix4f calculatedResult = mat1 * mat2;

			Assert::AreEqual(expectedResult[0][0], calculatedResult[0][0], TOLERANCE);
			Assert::AreEqual(expectedResult[0][1], calculatedResult[0][1], TOLERANCE);
			Assert::AreEqual(expectedResult[0][2], calculatedResult[0][2], TOLERANCE);
			Assert::AreEqual(expectedResult[0][3], calculatedResult[0][3], TOLERANCE);

			Assert::AreEqual(expectedResult[1][0], calculatedResult[1][0], TOLERANCE);
			Assert::AreEqual(expectedResult[1][1], calculatedResult[1][1], TOLERANCE);
			Assert::AreEqual(expectedResult[1][2], calculatedResult[1][2], TOLERANCE);
			Assert::AreEqual(expectedResult[1][3], calculatedResult[1][3], TOLERANCE);

			Assert::AreEqual(expectedResult[2][0], calculatedResult[2][0], TOLERANCE);
			Assert::AreEqual(expectedResult[2][1], calculatedResult[2][1], TOLERANCE);
			Assert::AreEqual(expectedResult[2][2], calculatedResult[2][2], TOLERANCE);
			Assert::AreEqual(expectedResult[2][3], calculatedResult[2][3], TOLERANCE);

			Assert::AreEqual(expectedResult[3][0], calculatedResult[3][0], TOLERANCE);
			Assert::AreEqual(expectedResult[3][1], calculatedResult[3][1], TOLERANCE);
			Assert::AreEqual(expectedResult[3][2], calculatedResult[3][2], TOLERANCE);
			Assert::AreEqual(expectedResult[3][3], calculatedResult[3][3], TOLERANCE);
		}

		/*
		* Tests overloaded equality operator
		*/
		TEST_METHOD(MatrixEqualityCheck)
		{
			const Matrix4f mat1(
				2.0f, 4.0f, 6.0f, 8.0f,
				10.0f, 12.0f, 14.0f, 16.0f,
				18.0f, 20.0f, 22.0f, 24.0f,
				26.0f, 28.0f, 30.0f, 32.0f);

			const Matrix4f mat2(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			const Matrix4f mat3(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Assert::IsTrue(mat2 == mat3);
			Assert::IsFalse(mat1 == mat2);
		}

		/*
		* Tests overloaded inequality operator
		*/
		TEST_METHOD(MatrixInequalityCheck)
		{
			const Matrix4f mat1(
				2.0f, 4.0f, 6.0f, 8.0f,
				10.0f, 12.0f, 14.0f, 16.0f,
				18.0f, 20.0f, 22.0f, 24.0f,
				26.0f, 28.0f, 30.0f, 32.0f);

			const Matrix4f mat2(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			const Matrix4f mat3(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Assert::IsTrue(mat1 != mat2);
			Assert::IsFalse(mat2 != mat3);
		}

		/*
		* Tests overloaded scalar addition operator
		*/
		TEST_METHOD(MatrixScalarAddition)
		{
			const float scalar = 2.0f;

			const Matrix4f mat1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4f mat2 = mat1 + scalar;

			const Matrix4f expectedResult(
				3.0f, 4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f, 10.0f,
				11.0f, 12.0f, 13.0f, 14.0f,
				15.0f, 16.0f, 17.0f, 18.0f);

			Assert::AreEqual(expectedResult[0][0], mat2[0][0], TOLERANCE);
			Assert::AreEqual(expectedResult[0][1], mat2[0][1], TOLERANCE);
			Assert::AreEqual(expectedResult[0][2], mat2[0][2], TOLERANCE);
			Assert::AreEqual(expectedResult[0][3], mat2[0][3], TOLERANCE);

			Assert::AreEqual(expectedResult[1][0], mat2[1][0], TOLERANCE);
			Assert::AreEqual(expectedResult[1][1], mat2[1][1], TOLERANCE);
			Assert::AreEqual(expectedResult[1][2], mat2[1][2], TOLERANCE);
			Assert::AreEqual(expectedResult[1][3], mat2[1][3], TOLERANCE);

			Assert::AreEqual(expectedResult[2][0], mat2[2][0], TOLERANCE);
			Assert::AreEqual(expectedResult[2][1], mat2[2][1], TOLERANCE);
			Assert::AreEqual(expectedResult[2][2], mat2[2][2], TOLERANCE);
			Assert::AreEqual(expectedResult[2][3], mat2[2][3], TOLERANCE);

			Assert::AreEqual(expectedResult[3][0], mat2[3][0], TOLERANCE);
			Assert::AreEqual(expectedResult[3][1], mat2[3][1], TOLERANCE);
			Assert::AreEqual(expectedResult[3][2], mat2[3][2], TOLERANCE);
			Assert::AreEqual(expectedResult[3][3], mat2[3][3], TOLERANCE);
		}

		/*
		* Tests overloaded scalar addition-assignment operator
		*/
		TEST_METHOD(MatrixScalarAdditionAssignment)
		{
			const float scalar = 2.0f;

			Matrix4f mat1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			mat1 += scalar;

			const Matrix4f expectedResult(
				3.0f, 4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f, 10.0f,
				11.0f, 12.0f, 13.0f, 14.0f,
				15.0f, 16.0f, 17.0f, 18.0f);

			Assert::AreEqual(expectedResult[0][0], mat1[0][0], TOLERANCE);
			Assert::AreEqual(expectedResult[0][1], mat1[0][1], TOLERANCE);
			Assert::AreEqual(expectedResult[0][2], mat1[0][2], TOLERANCE);
			Assert::AreEqual(expectedResult[0][3], mat1[0][3], TOLERANCE);

			Assert::AreEqual(expectedResult[1][0], mat1[1][0], TOLERANCE);
			Assert::AreEqual(expectedResult[1][1], mat1[1][1], TOLERANCE);
			Assert::AreEqual(expectedResult[1][2], mat1[1][2], TOLERANCE);
			Assert::AreEqual(expectedResult[1][3], mat1[1][3], TOLERANCE);

			Assert::AreEqual(expectedResult[2][0], mat1[2][0], TOLERANCE);
			Assert::AreEqual(expectedResult[2][1], mat1[2][1], TOLERANCE);
			Assert::AreEqual(expectedResult[2][2], mat1[2][2], TOLERANCE);
			Assert::AreEqual(expectedResult[2][3], mat1[2][3], TOLERANCE);

			Assert::AreEqual(expectedResult[3][0], mat1[3][0], TOLERANCE);
			Assert::AreEqual(expectedResult[3][1], mat1[3][1], TOLERANCE);
			Assert::AreEqual(expectedResult[3][2], mat1[3][2], TOLERANCE);
			Assert::AreEqual(expectedResult[3][3], mat1[3][3], TOLERANCE);
		}

		/*
		* Tests overloaded scalar subtraction operator
		*/
		TEST_METHOD(MatrixScalarSubtraction)
		{
			const float scalar = 2.0f;

			const Matrix4f mat1(
				3.0f, 4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f, 10.0f,
				11.0f, 12.0f, 13.0f, 14.0f,
				15.0f, 16.0f, 17.0f, 18.0f);

			Matrix4f mat2 = mat1 - scalar;

			const Matrix4f expectedResult(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Assert::AreEqual(expectedResult[0][0], mat2[0][0], TOLERANCE);
			Assert::AreEqual(expectedResult[0][1], mat2[0][1], TOLERANCE);
			Assert::AreEqual(expectedResult[0][2], mat2[0][2], TOLERANCE);
			Assert::AreEqual(expectedResult[0][3], mat2[0][3], TOLERANCE);

			Assert::AreEqual(expectedResult[1][0], mat2[1][0], TOLERANCE);
			Assert::AreEqual(expectedResult[1][1], mat2[1][1], TOLERANCE);
			Assert::AreEqual(expectedResult[1][2], mat2[1][2], TOLERANCE);
			Assert::AreEqual(expectedResult[1][3], mat2[1][3], TOLERANCE);

			Assert::AreEqual(expectedResult[2][0], mat2[2][0], TOLERANCE);
			Assert::AreEqual(expectedResult[2][1], mat2[2][1], TOLERANCE);
			Assert::AreEqual(expectedResult[2][2], mat2[2][2], TOLERANCE);
			Assert::AreEqual(expectedResult[2][3], mat2[2][3], TOLERANCE);

			Assert::AreEqual(expectedResult[3][0], mat2[3][0], TOLERANCE);
			Assert::AreEqual(expectedResult[3][1], mat2[3][1], TOLERANCE);
			Assert::AreEqual(expectedResult[3][2], mat2[3][2], TOLERANCE);
			Assert::AreEqual(expectedResult[3][3], mat2[3][3], TOLERANCE);
		}

		/*
		* Tests overloaded scalar subtraction-assignment operator
		*/
		TEST_METHOD(MatrixScalarSubtractionAssignment)
		{
			const float scalar = 2.0f;

			Matrix4f mat1(
				3.0f, 4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f, 10.0f,
				11.0f, 12.0f, 13.0f, 14.0f,
				15.0f, 16.0f, 17.0f, 18.0f);

			mat1 -= scalar;

			const Matrix4f expectedResult(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Assert::AreEqual(expectedResult[0][0], mat1[0][0], TOLERANCE);
			Assert::AreEqual(expectedResult[0][1], mat1[0][1], TOLERANCE);
			Assert::AreEqual(expectedResult[0][2], mat1[0][2], TOLERANCE);
			Assert::AreEqual(expectedResult[0][3], mat1[0][3], TOLERANCE);

			Assert::AreEqual(expectedResult[1][0], mat1[1][0], TOLERANCE);
			Assert::AreEqual(expectedResult[1][1], mat1[1][1], TOLERANCE);
			Assert::AreEqual(expectedResult[1][2], mat1[1][2], TOLERANCE);
			Assert::AreEqual(expectedResult[1][3], mat1[1][3], TOLERANCE);

			Assert::AreEqual(expectedResult[2][0], mat1[2][0], TOLERANCE);
			Assert::AreEqual(expectedResult[2][1], mat1[2][1], TOLERANCE);
			Assert::AreEqual(expectedResult[2][2], mat1[2][2], TOLERANCE);
			Assert::AreEqual(expectedResult[2][3], mat1[2][3], TOLERANCE);

			Assert::AreEqual(expectedResult[3][0], mat1[3][0], TOLERANCE);
			Assert::AreEqual(expectedResult[3][1], mat1[3][1], TOLERANCE);
			Assert::AreEqual(expectedResult[3][2], mat1[3][2], TOLERANCE);
			Assert::AreEqual(expectedResult[3][3], mat1[3][3], TOLERANCE);
		}

		/*
		* Tests overloaded scalar multiplication operator
		*/
		TEST_METHOD(MatrixScalarMultiplication)
		{
			const float scalar = 2.0f;

			const Matrix4f mat1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4f mat2 = mat1 * scalar;

			const Matrix4f expectedResult(
				2.0f, 4.0f, 6.0f, 8.0f,
				10.0f, 12.0f, 14.0f, 16.0f,
				18.0f, 20.0f, 22.0f, 24.0f,
				26.0f, 28.0f, 30.0f, 32.0f);

			Assert::AreEqual(expectedResult[0][0], mat2[0][0], TOLERANCE);
			Assert::AreEqual(expectedResult[0][1], mat2[0][1], TOLERANCE);
			Assert::AreEqual(expectedResult[0][2], mat2[0][2], TOLERANCE);
			Assert::AreEqual(expectedResult[0][3], mat2[0][3], TOLERANCE);

			Assert::AreEqual(expectedResult[1][0], mat2[1][0], TOLERANCE);
			Assert::AreEqual(expectedResult[1][1], mat2[1][1], TOLERANCE);
			Assert::AreEqual(expectedResult[1][2], mat2[1][2], TOLERANCE);
			Assert::AreEqual(expectedResult[1][3], mat2[1][3], TOLERANCE);

			Assert::AreEqual(expectedResult[2][0], mat2[2][0], TOLERANCE);
			Assert::AreEqual(expectedResult[2][1], mat2[2][1], TOLERANCE);
			Assert::AreEqual(expectedResult[2][2], mat2[2][2], TOLERANCE);
			Assert::AreEqual(expectedResult[2][3], mat2[2][3], TOLERANCE);

			Assert::AreEqual(expectedResult[3][0], mat2[3][0], TOLERANCE);
			Assert::AreEqual(expectedResult[3][1], mat2[3][1], TOLERANCE);
			Assert::AreEqual(expectedResult[3][2], mat2[3][2], TOLERANCE);
			Assert::AreEqual(expectedResult[3][3], mat2[3][3], TOLERANCE);
		}

		/*
		* Tests overloaded scalar multiplication-assignment operator
		*/
		TEST_METHOD(MatrixScalarMultiplicationAssignment)
		{
			const float scalar = 2.0f;

			Matrix4f mat1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			mat1 *= scalar;

			const Matrix4f expectedResult(
				2.0f, 4.0f, 6.0f, 8.0f,
				10.0f, 12.0f, 14.0f, 16.0f,
				18.0f, 20.0f, 22.0f, 24.0f,
				26.0f, 28.0f, 30.0f, 32.0f);

			Assert::AreEqual(expectedResult[0][0], mat1[0][0], TOLERANCE);
			Assert::AreEqual(expectedResult[0][1], mat1[0][1], TOLERANCE);
			Assert::AreEqual(expectedResult[0][2], mat1[0][2], TOLERANCE);
			Assert::AreEqual(expectedResult[0][3], mat1[0][3], TOLERANCE);

			Assert::AreEqual(expectedResult[1][0], mat1[1][0], TOLERANCE);
			Assert::AreEqual(expectedResult[1][1], mat1[1][1], TOLERANCE);
			Assert::AreEqual(expectedResult[1][2], mat1[1][2], TOLERANCE);
			Assert::AreEqual(expectedResult[1][3], mat1[1][3], TOLERANCE);

			Assert::AreEqual(expectedResult[2][0], mat1[2][0], TOLERANCE);
			Assert::AreEqual(expectedResult[2][1], mat1[2][1], TOLERANCE);
			Assert::AreEqual(expectedResult[2][2], mat1[2][2], TOLERANCE);
			Assert::AreEqual(expectedResult[2][3], mat1[2][3], TOLERANCE);

			Assert::AreEqual(expectedResult[3][0], mat1[3][0], TOLERANCE);
			Assert::AreEqual(expectedResult[3][1], mat1[3][1], TOLERANCE);
			Assert::AreEqual(expectedResult[3][2], mat1[3][2], TOLERANCE);
			Assert::AreEqual(expectedResult[3][3], mat1[3][3], TOLERANCE);
		}

		/*
		* Tests matrix transposition is correct
		*/
		TEST_METHOD(MatrixTranspose)
		{
			const Matrix4f mat1(
				1.0f, 1.0f, 1.0f, 1.0f,
				2.0f, 2.0f, 2.0f, 2.0f,
				3.0f, 3.0f, 3.0f, 3.0f,
				4.0f, 4.0f, 4.0f, 4.0f);

			const Matrix4f mat2 = mat1.Transpose();

			const Matrix4f expectedResult(
				1.0f, 2.0f, 3.0f, 4.0f,
				1.0f, 2.0f, 3.0f, 4.0f,
				1.0f, 2.0f, 3.0f, 4.0f,
				1.0f, 2.0f, 3.0f, 4.0f);

			Assert::AreEqual(expectedResult[0][0], mat2[0][0], TOLERANCE);
			Assert::AreEqual(expectedResult[0][1], mat2[0][1], TOLERANCE);
			Assert::AreEqual(expectedResult[0][2], mat2[0][2], TOLERANCE);
			Assert::AreEqual(expectedResult[0][3], mat2[0][3], TOLERANCE);

			Assert::AreEqual(expectedResult[1][0], mat2[1][0], TOLERANCE);
			Assert::AreEqual(expectedResult[1][1], mat2[1][1], TOLERANCE);
			Assert::AreEqual(expectedResult[1][2], mat2[1][2], TOLERANCE);
			Assert::AreEqual(expectedResult[1][3], mat2[1][3], TOLERANCE);

			Assert::AreEqual(expectedResult[2][0], mat2[2][0], TOLERANCE);
			Assert::AreEqual(expectedResult[2][1], mat2[2][1], TOLERANCE);
			Assert::AreEqual(expectedResult[2][2], mat2[2][2], TOLERANCE);
			Assert::AreEqual(expectedResult[2][3], mat2[2][3], TOLERANCE);

			Assert::AreEqual(expectedResult[3][0], mat2[3][0], TOLERANCE);
			Assert::AreEqual(expectedResult[3][1], mat2[3][1], TOLERANCE);
			Assert::AreEqual(expectedResult[3][2], mat2[3][2], TOLERANCE);
			Assert::AreEqual(expectedResult[3][3], mat2[3][3], TOLERANCE);
		}

		/*
		* Tests the determinant of a matrix is calculated correctly
		*/
		TEST_METHOD(MatrixDeterminant)
		{
			const Matrix4f mat1(
				1.0f, 3.0f, 5.0f, 9.0f,
				1.0f, 3.0f, 1.0f, 7.0f,
				4.0f, 3.0f, 9.0f, 7.0f,
				5.0f, 2.0f, 0.0f, 9.0f);

			const float calculatedDeterminant = mat1.Determinant();

			const float expectedDeterminant = -376.0f;

			Assert::AreEqual(expectedDeterminant, calculatedDeterminant, TOLERANCE);
		}

		/*
		* Tests the inverse of a matrix is calculated correctly
		*/
		TEST_METHOD(MatrixInverse)
		{
			//TODO
		}

		/*
		* Tests the eigenvalue of a matrix is calculated correctly
		*/
		TEST_METHOD(MatrixEigenvalue)
		{
			//TODO
		}

		/*
		* Tests the eigenvector of a matrix is calculated correctly
		*/
		TEST_METHOD(MatrixEigenvector)
		{
			//TODO
		}

		/*
		* Tests a matrix can be converted into an Identity matrix correctly
		*/
		TEST_METHOD(MatrixMakeIdentity)
		{
			Matrix4f mat1(
				1.0f, 3.0f, 5.0f, 9.0f,
				1.0f, 3.0f, 1.0f, 7.0f,
				4.0f, 3.0f, 9.0f, 7.0f,
				5.0f, 2.0f, 0.0f, 9.0f);

			mat1.MakeIdentity();

			const Matrix4f identity = Matrix4f::Identity();

			Assert::AreEqual(identity[0][0], mat1[0][0], TOLERANCE);
			Assert::AreEqual(identity[0][1], mat1[0][1], TOLERANCE);
			Assert::AreEqual(identity[0][2], mat1[0][2], TOLERANCE);
			Assert::AreEqual(identity[0][3], mat1[0][3], TOLERANCE);

			Assert::AreEqual(identity[1][0], mat1[1][0], TOLERANCE);
			Assert::AreEqual(identity[1][1], mat1[1][1], TOLERANCE);
			Assert::AreEqual(identity[1][2], mat1[1][2], TOLERANCE);
			Assert::AreEqual(identity[1][3], mat1[1][3], TOLERANCE);

			Assert::AreEqual(identity[2][0], mat1[2][0], TOLERANCE);
			Assert::AreEqual(identity[2][1], mat1[2][1], TOLERANCE);
			Assert::AreEqual(identity[2][2], mat1[2][2], TOLERANCE);
			Assert::AreEqual(identity[2][3], mat1[2][3], TOLERANCE);

			Assert::AreEqual(identity[3][0], mat1[3][0], TOLERANCE);
			Assert::AreEqual(identity[3][1], mat1[3][1], TOLERANCE);
			Assert::AreEqual(identity[3][2], mat1[3][2], TOLERANCE);
			Assert::AreEqual(identity[3][3], mat1[3][3], TOLERANCE);
		}
	};
}