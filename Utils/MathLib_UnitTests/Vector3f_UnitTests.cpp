#include "pch.h"
#include "CppUnitTest.h"

#include "Vector3f.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathLibUnitTests
{
	TEST_CLASS(MathLibUnitTests)
	{
	public:
		
		const float TOLERANCE = 0.001f;

		/*
		* Tests an empty vector is initialised correctly
		*/
		TEST_METHOD(EmptyVectorInit)
		{
			Vector3f vec3;

			const float expectedX = 0.0f;
			const float expectedY = 0.0f;
			const float expectedZ = 0.0f;

			Assert::AreEqual(expectedX, vec3.X, TOLERANCE);
			Assert::AreEqual(expectedY, vec3.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, vec3.Z, TOLERANCE);
		}

		/*
		* Tests a vector initialised with values is initialised correctly
		*/
		TEST_METHOD(VectorInit)
		{
			Vector3f vec3(1.0f, 1.0f, 1.0f);

			const float expectedX = 1.0f;
			const float expectedY = 1.0f;
			const float expectedZ = 1.0f;

			Assert::AreEqual(expectedX, vec3.X, TOLERANCE);
			Assert::AreEqual(expectedY, vec3.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, vec3.Z, TOLERANCE);
		}

		/*
		* Tests the overloaded assignment operator
		*/
		TEST_METHOD(VectorAssignment)
		{
			Vector3f vecMain(2.1f, 3.1f, 0.0f);

			Vector3f vecSecond = vecMain;

			Assert::AreEqual(vecMain.X, vecSecond.X, TOLERANCE);
			Assert::AreEqual(vecMain.Y, vecSecond.Y, TOLERANCE);
			Assert::AreEqual(vecMain.Z, vecSecond.Z, TOLERANCE);
		}

		/*
		* Tests overloaded addition operator
		*/
		TEST_METHOD(VectorAddition)
		{
			Vector3f vec1(1.0f, 1.0f, 1.0f);
			Vector3f vec2(1.0f, 1.0f, 1.0f);

			Vector3f finalVec = vec1 + vec2;

			const float expectedX = 2.0f;
			const float expectedY = 2.0f;
			const float expectedZ = 2.0f;

			Assert::AreEqual(expectedX, finalVec.X, TOLERANCE);
			Assert::AreEqual(expectedY, finalVec.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, finalVec.Z, TOLERANCE);
		}

		/*
		* Tests overloaded addition-assignment operator
		*/
		TEST_METHOD(VectorAdditionAssignment)
		{
			Vector3f vec1(1.0f, 1.0f, 1.0f);
			Vector3f vec2(1.0f, 1.0f, 1.0f);

			vec1 += vec2;

			const float expectedX = 2.0f;
			const float expectedY = 2.0f;
			const float expectedZ = 2.0f;

			Assert::AreEqual(expectedX, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedY, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, vec1.Z, TOLERANCE);
		}

		/*
		* Tests overloaded subtraction operator
		*/
		TEST_METHOD(VectorSubtraction)
		{
			Vector3f vec1(2.0f, 2.0f, 2.0f);
			Vector3f vec2(1.0f, 1.0f, 1.0f);

			Vector3f finalVec = vec1 - vec2;

			const float expectedX = 1.0f;
			const float expectedY = 1.0f;
			const float expectedZ = 1.0f;

			Assert::AreEqual(expectedX, finalVec.X, TOLERANCE);
			Assert::AreEqual(expectedY, finalVec.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, finalVec.Z, TOLERANCE);
		}

		/*
		* Tests overloaded subtraction-assignment operator
		*/
		TEST_METHOD(VectorSubtractionAssignment)
		{
			Vector3f vec1(2.0f, 2.0f, 2.0f);
			Vector3f vec2(1.0f, 1.0f, 1.0f);

			vec1 -= vec2;

			const float expectedX = 1.0f;
			const float expectedY = 1.0f;
			const float expectedZ = 1.0f;

			Assert::AreEqual(expectedX, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedY, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, vec1.Z, TOLERANCE);
		}

		/*
		* Tests overloaded multiplication operator
		*/
		TEST_METHOD(VectorMultiplication)
		{
			Vector3f vec1(2.0f, 2.0f, 2.0f);
			Vector3f vec2(2.0f, 2.0f, 2.0f);

			Vector3f finalVec = vec1 * vec2;

			const float expectedX = 4.0f;
			const float expectedY = 4.0f;
			const float expectedZ = 4.0f;

			Assert::AreEqual(expectedX, finalVec.X, TOLERANCE);
			Assert::AreEqual(expectedY, finalVec.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, finalVec.Z, TOLERANCE);
		}

		/*
		* Tests overloaded equality operator
		*/
		TEST_METHOD(VectorEqualityCheck)
		{
			Vector3f vec1(2.0f, 2.0f, 2.0f);
			Vector3f vec2(2.0f, 2.0f, 2.0f);

			Vector3f vec3(1.0f, 1.0f, 1.0f);

			Assert::IsTrue(vec1 == vec2);
			Assert::IsFalse(vec1 == vec3);
		}

		/*
		* Tests overloaded inequality operator
		*/
		TEST_METHOD(VectorInequalityCheck)
		{
			Vector3f vec1(2.0f, 2.0f, 2.0f);
			Vector3f vec2(1.0f, 1.0f, 1.0f);

			Vector3f vec3(2.0f, 2.0f, 2.0f);

			Assert::IsTrue(vec1 != vec2);
			Assert::IsFalse(vec1 != vec3);
		}

		/*
		* Tests overloaded scalar addition operator for float
		*/
		TEST_METHOD(VectorScalarAdditionFloat)
		{
			const float scalar = 2.0f;

			Vector3f vec1(2.0f, 2.0f, 2.0f);

			Vector3f finalVec = vec1 + scalar;

			const float expectedX = 4.0f;
			const float expectedY = 4.0f;
			const float expectedZ = 4.0f;

			Assert::AreEqual(expectedX, finalVec.X, TOLERANCE);
			Assert::AreEqual(expectedY, finalVec.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, finalVec.Z, TOLERANCE);
		}

		/*
		* Tests overloaded scalar addition operator for double
		*/
		TEST_METHOD(VectorScalarAdditionDouble)
		{
			const double scalar = 2.0;

			Vector3f vec1(2.0f, 2.0f, 2.0f);

			Vector3f finalVec = vec1 + scalar;

			const float expectedX = 4.0f;
			const float expectedY = 4.0f;
			const float expectedZ = 4.0f;

			Assert::AreEqual(expectedX, finalVec.X, TOLERANCE);
			Assert::AreEqual(expectedY, finalVec.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, finalVec.Z, TOLERANCE);
		}

		/*
		* Tests overloaded scalar addition operator for int
		*/
		TEST_METHOD(VectorScalarAdditionInt)
		{
			const int scalar = 2;

			Vector3f vec1(2.0f, 2.0f, 2.0f);

			Vector3f finalVec = vec1 + scalar;

			const float expectedX = 4.0f;
			const float expectedY = 4.0f;
			const float expectedZ = 4.0f;

			Assert::AreEqual(expectedX, finalVec.X, TOLERANCE);
			Assert::AreEqual(expectedY, finalVec.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, finalVec.Z, TOLERANCE);
		}

		//TODO:

		/*
		* Tests overloaded scalar addition-assignment operator for float
		*/
		TEST_METHOD(VectorScalarAdditionAssignmentFloat)
		{
			const float scalar = 2.0f;

			Vector3f vec1(2.0f, 2.0f, 2.0f);

			vec1 += scalar;

			const float expectedX = 4.0f;
			const float expectedY = 4.0f;
			const float expectedZ = 4.0f;

			Assert::AreEqual(expectedX, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedY, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, vec1.Z, TOLERANCE);
		}

		/*
		* Tests overloaded scalar addition-assignment operator for double
		*/
		TEST_METHOD(VectorScalarAdditionAssignmentDouble)
		{
			const double scalar = 2.0;

			Vector3f vec1(2.0f, 2.0f, 2.0f);

			vec1 += scalar;

			const float expectedX = 4.0f;
			const float expectedY = 4.0f;
			const float expectedZ = 4.0f;

			Assert::AreEqual(expectedX, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedY, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, vec1.Z, TOLERANCE);
		}

		/*
		* Tests overloaded scalar addition-assignment operator for int
		*/
		TEST_METHOD(VectorScalarAdditionAssignmentInt)
		{
			const int scalar = 2;

			Vector3f vec1(2.0f, 2.0f, 2.0f);

			vec1 += scalar;

			const float expectedX = 4.0f;
			const float expectedY = 4.0f;
			const float expectedZ = 4.0f;

			Assert::AreEqual(expectedX, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedY, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, vec1.Z, TOLERANCE);
		}
	};
}
