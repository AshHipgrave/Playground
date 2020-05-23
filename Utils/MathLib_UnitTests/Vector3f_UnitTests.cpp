#include "pch.h"
#include "CppUnitTest.h"

#include "Vector3f.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathLib_UnitTests
{
	TEST_CLASS(Vector3f_UnitTests)
	{
	public:
		const float TOLERANCE = 0.001f;

		/*
		* Tests an empty vector is initialised correctly
		*/
		TEST_METHOD(EmptyVectorInit)
		{
			const Vector3f vec3;

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
			const Vector3f vec3(1.0f, 1.0f, 1.0f);

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
			const Vector3f vec1(2.1f, 3.1f, 0.0f);

			const Vector3f vec2 = vec1;

			Assert::AreEqual(vec1.X, vec2.X, TOLERANCE);
			Assert::AreEqual(vec1.Y, vec2.Y, TOLERANCE);
			Assert::AreEqual(vec1.Z, vec2.Z, TOLERANCE);
		}

		/*
		* Tests overloaded addition operator
		*/
		TEST_METHOD(VectorAddition)
		{
			const Vector3f vec1(1.0f, 1.0f, 1.0f);
			const Vector3f vec2(1.0f, 1.0f, 1.0f);

			const Vector3f finalVec = vec1 + vec2;

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

			const Vector3f vec2(1.0f, 1.0f, 1.0f);

			vec1 += vec2;

			const Vector3f expectedResult(2.0f, 2.0f, 2.0f);

			Assert::AreEqual(expectedResult.X, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedResult.Y, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedResult.Z, vec1.Z, TOLERANCE);
		}

		/*
		* Tests overloaded subtraction operator
		*/
		TEST_METHOD(VectorSubtraction)
		{
			const Vector3f vec1(2.0f, 2.0f, 2.0f);
			const Vector3f vec2(1.0f, 1.0f, 1.0f);

			const Vector3f calculatedResult = vec1 - vec2;

			const Vector3f expectedResult(1.0f, 1.0f, 1.0f);

			Assert::AreEqual(expectedResult.X, calculatedResult.X, TOLERANCE);
			Assert::AreEqual(expectedResult.Y, calculatedResult.Y, TOLERANCE);
			Assert::AreEqual(expectedResult.Z, calculatedResult.Z, TOLERANCE);
		}

		/*
		* Tests overloaded subtraction-assignment operator
		*/
		TEST_METHOD(VectorSubtractionAssignment)
		{
			Vector3f vec1(2.0f, 2.0f, 2.0f);
			const Vector3f vec2(1.0f, 1.0f, 1.0f);

			vec1 -= vec2;

			const Vector3f expectedResult(1.0f, 1.0f, 1.0f);

			Assert::AreEqual(expectedResult.X, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedResult.Y, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedResult.Z, vec1.Z, TOLERANCE);
		}

		/*
		* Tests overloaded multiplication operator
		*/
		TEST_METHOD(VectorMultiplication)
		{
			const Vector3f vec1(2.0f, 2.0f, 2.0f);
			const Vector3f vec2(2.0f, 2.0f, 2.0f);

			const Vector3f calculatedResult = vec1 * vec2;

			const Vector3f expectedResult(4.0f, 4.0f, 4.0f);

			Assert::AreEqual(expectedResult.X, calculatedResult.X, TOLERANCE);
			Assert::AreEqual(expectedResult.Y, calculatedResult.Y, TOLERANCE);
			Assert::AreEqual(expectedResult.Z, calculatedResult.Z, TOLERANCE);
		}

		/*
		* Tests overloaded equality operator
		*/
		TEST_METHOD(VectorEqualityCheck)
		{
			const Vector3f vec1(2.0f, 2.0f, 2.0f);
			const Vector3f vec2(2.0f, 2.0f, 2.0f);

			const Vector3f vec3(1.0f, 1.0f, 1.0f);

			Assert::IsTrue(vec1 == vec2);
			Assert::IsFalse(vec1 == vec3);
		}

		/*
		* Tests overloaded inequality operator
		*/
		TEST_METHOD(VectorInequalityCheck)
		{
			const Vector3f vec1(2.0f, 2.0f, 2.0f);
			const Vector3f vec2(1.0f, 1.0f, 1.0f);

			const Vector3f vec3(2.0f, 2.0f, 2.0f);

			Assert::IsTrue(vec1 != vec2);
			Assert::IsFalse(vec1 != vec3);
		}

		/*
		* Tests overloaded scalar addition operator
		*/
		TEST_METHOD(VectorScalarAddition)
		{
			const float scalar = 2.0f;

			const Vector3f vec1(2.0f, 2.0f, 2.0f);

			const Vector3f calculatedResult = vec1 + scalar;

			const Vector3f expectedResult(4.0f, 4.0f, 4.0f);

			Assert::AreEqual(expectedResult.X, calculatedResult.X, TOLERANCE);
			Assert::AreEqual(expectedResult.Y, calculatedResult.Y, TOLERANCE);
			Assert::AreEqual(expectedResult.Z, calculatedResult.Z, TOLERANCE);
		}

		/*
		* Tests overloaded scalar addition-assignment operator
		*/
		TEST_METHOD(VectorScalarAdditionAssignment)
		{
			const float scalar = 2.0f;

			Vector3f vec1(2.0f, 2.0f, 2.0f);

			vec1 += scalar;

			const Vector3f expectedResult(4.0f, 4.0f, 4.0f);

			Assert::AreEqual(expectedResult.X, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedResult.Y, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedResult.Z, vec1.Z, TOLERANCE);
		}

		/*
		* Tests overloaded scalar subtraction operator
		*/
		TEST_METHOD(VectorScalarSubtraction)
		{
			const float scalar = 1.0f;

			const Vector3f vec1(2.0f, 2.0f, 2.0f);

			const Vector3f calculatedResult = vec1 - scalar;

			const Vector3f expectedResult(1.0f, 1.0f, 1.0f);

			Assert::AreEqual(expectedResult.X, calculatedResult.X, TOLERANCE);
			Assert::AreEqual(expectedResult.Y, calculatedResult.Y, TOLERANCE);
			Assert::AreEqual(expectedResult.Z, calculatedResult.Z, TOLERANCE);
		}

		/*
		* Tests overloaded scalar subtraction-assignment operator
		*/
		TEST_METHOD(VectorScalarSubtractionAssignment)
		{
			const float scalar = 1.0f;

			Vector3f vec1(2.0f, 2.0f, 2.0f);

			vec1 -= scalar;

			const Vector3f expectedResult(1.0f, 1.0f, 1.0f);

			Assert::AreEqual(expectedResult.X, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedResult.Y, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedResult.Z, vec1.Z, TOLERANCE);
		}

		/*
		* Tests overloaded scalar multiplication operator
		*/
		TEST_METHOD(VectorScalarMultiplication)
		{
			const float scalar = 2.0f;

			const Vector3f vec1(2.0f, 2.0f, 2.0f);

			const Vector3f calculatedResult = vec1 * scalar;

			const Vector3f expectedResult(4.0f, 4.0f, 4.0f);

			Assert::AreEqual(expectedResult.X, calculatedResult.X, TOLERANCE);
			Assert::AreEqual(expectedResult.Y, calculatedResult.Y, TOLERANCE);
			Assert::AreEqual(expectedResult.Z, calculatedResult.Z, TOLERANCE);
		}

		/*
		* Tests overloaded scalar multiplication-assignment operator
		*/
		TEST_METHOD(VectorScalarMultiplicationAssignment)
		{
			const float scalar = 2.0f;

			Vector3f vec1(2.0f, 2.0f, 2.0f);

			vec1 *= scalar;

			const Vector3f expectedResult(4.0f, 4.0f, 4.0f);

			Assert::AreEqual(expectedResult.X, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedResult.Y, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedResult.Z, vec1.Z, TOLERANCE);
		}

		/*
		* Tests overloaded scalar division operator
		*/
		TEST_METHOD(VectorScalarDivision)
		{
			const float scalar = 2.0f;

			const Vector3f vec1(4.0f, 4.0f, 4.0f);

			const Vector3f calculatedResult = vec1 / scalar;

			const Vector3f expectedResult(2.0f, 2.0f, 2.0f);

			Assert::AreEqual(expectedResult.X, calculatedResult.X, TOLERANCE);
			Assert::AreEqual(expectedResult.Y, calculatedResult.Y, TOLERANCE);
			Assert::AreEqual(expectedResult.Z, calculatedResult.Z, TOLERANCE);
		}

		/*
		* Tests overloaded scalar division-assignment operator
		*/
		TEST_METHOD(VectorScalarDivisionAssignment)
		{
			const float scalar = 2.0f;

			Vector3f vec1(4.0f, 4.0f, 4.0f);

			vec1 /= scalar;

			const Vector3f expectedResult(2.0f, 2.0f, 2.0f);

			Assert::AreEqual(expectedResult.X, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedResult.Y, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedResult.Z, vec1.Z, TOLERANCE);
		}

		/*
		* Tests accessing a specific index of a vector
		*/
		TEST_METHOD(VectorIndexOperator)
		{
			const Vector3f vec1(1.0f, 2.0f, 3.0f);

			const float currentX = vec1[0];
			const float currentY = vec1[1];
			const float currentZ = vec1[2];

			const float expectedX = 1.0f;
			const float expectedY = 2.0f;
			const float expectedZ = 3.0f;

			Assert::AreEqual(expectedX, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedY, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, vec1.Z, TOLERANCE);
		}

		/*
		* Tests accessing a specific index of a vector by reference
		*/
		TEST_METHOD(VectorIndexOperatorRef)
		{
			const Vector3f vec1(1.0f, 2.0f, 3.0f);

			const float& currentX = vec1[0];
			const float& currentY = vec1[1];
			const float& currentZ = vec1[2];

			const float expectedX = 1.0f;
			const float expectedY = 2.0f;
			const float expectedZ = 3.0f;

			Assert::AreEqual(expectedX, vec1.X, TOLERANCE);
			Assert::AreEqual(expectedY, vec1.Y, TOLERANCE);
			Assert::AreEqual(expectedZ, vec1.Z, TOLERANCE);
		}

		/*
		* Tests the squared length of a vector is calculated correctly
		*/
		TEST_METHOD(VectorLengthSquared)
		{
			const Vector3f vec1(1.0f, 2.0f, 3.0f);

			const float expectedResult = 14.0f;

			const float calculatedResult = vec1.LengthSquared();

			Assert::AreEqual(expectedResult, calculatedResult, TOLERANCE);
		}

		/*
		* Tests the length of a vector is calculated correctly
		*/
		TEST_METHOD(VectorLength)
		{
			const Vector3f vec1(1.0f, 2.0f, 3.0f);

			const float expectedResult = 3.741657386;

			const float calculatedResult = vec1.Length();

			Assert::AreEqual(expectedResult, calculatedResult, TOLERANCE);
		}

		/*
		* Tests a normalised vector is calculated correctly
		*/
		TEST_METHOD(VectorNormalised)
		{
			const Vector3f vec1(20.0f, 50.0f, 10.0f);

			const Vector3f expectedNormalisedVector(0.3651483f, 0.9128709f, 0.1825741f);

			const Vector3f calculatedNormalisedVector = vec1.Normalize();

			Assert::AreEqual(expectedNormalisedVector.X, calculatedNormalisedVector.X, TOLERANCE);
			Assert::AreEqual(expectedNormalisedVector.Y, calculatedNormalisedVector.Y, TOLERANCE);
			Assert::AreEqual(expectedNormalisedVector.Z, calculatedNormalisedVector.Z, TOLERANCE);
		}

		/*
		* Tests the Dot product of a vector is calculated correctly
		*/
		TEST_METHOD(VectorDotProduct)
		{
			const Vector3f vec1(1.0f, 2.0f, 3.0f);
			const Vector3f vec2(1.0f, 5.0f, 7.0f);

			const float expectedDotProduct = 32.0f;

			const float calculatedDotProduct = Vector3f::Dot(vec1, vec2);

			Assert::AreEqual(expectedDotProduct, calculatedDotProduct, TOLERANCE);
		}

		/*
		* Tests the Cross product of a vector is calculated correctly
		*/
		TEST_METHOD(VectorCrossProduct)
		{
			const Vector3f vec1(1.0f, 2.0f, 3.0f);
			const Vector3f vec2(1.0f, 5.0f, 7.0f);

			const Vector3f expectedCrossProduct(-1.0f, -4.0f, 3.0f);

			const Vector3f calculatedCrossProduct = Vector3f::Cross(vec1, vec2);

			Assert::AreEqual(expectedCrossProduct.X, calculatedCrossProduct.X, TOLERANCE);
			Assert::AreEqual(expectedCrossProduct.Y, calculatedCrossProduct.Y, TOLERANCE);
			Assert::AreEqual(expectedCrossProduct.Z, calculatedCrossProduct.Z, TOLERANCE);
		}
	};
}
