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
		* Tests overloaded addition operator for vector + vector math
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
		* Tests overloaded addition operator for vector + vector math
		*/
		TEST_METHOD(VectorSubtraction)
		{

		}
	};
}
