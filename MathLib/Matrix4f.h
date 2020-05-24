#pragma once

#include "MathCommon.h"

class Matrix4f
{
public:
	float M[4][4];

public:
	Matrix4f()
	{
		M[0][0] = 0.0f;
		M[0][1] = 0.0f;
		M[0][2] = 0.0f;
		M[0][3] = 0.0f;

		M[1][0] = 0.0f;
		M[1][1] = 0.0f;
		M[1][2] = 0.0f;
		M[1][3] = 0.0f;

		M[2][0] = 0.0f;
		M[2][1] = 0.0f;
		M[2][2] = 0.0f;
		M[2][3] = 0.0f;

		M[3][0] = 0.0f;
		M[3][1] = 0.0f;
		M[3][2] = 0.0f;
		M[3][3] = 0.0f;
	}

	Matrix4f(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		M[0][0] = m00;
		M[0][1] = m01;
		M[0][2] = m02;
		M[0][3] = m03;

		M[1][0] = m10;
		M[1][1] = m11;
		M[1][2] = m12;
		M[1][3] = m13;

		M[2][0] = m20;
		M[2][1] = m21;
		M[2][2] = m22;
		M[2][3] = m23;

		M[3][0] = m30;
		M[3][1] = m31;
		M[3][2] = m32;
		M[3][3] = m33;
	}

	Matrix4f(const Matrix4f& other)
	{
		M[0][0] = other.M[0][0];
		M[0][1] = other.M[0][1];
		M[0][2] = other.M[0][2];
		M[0][3] = other.M[0][3];

		M[1][0] = other.M[1][0];
		M[1][1] = other.M[1][1];
		M[1][2] = other.M[1][2];
		M[1][3] = other.M[1][3];

		M[2][0] = other.M[2][0];
		M[2][1] = other.M[2][1];
		M[2][2] = other.M[2][2];
		M[2][3] = other.M[2][3];

		M[3][0] = other.M[3][0];
		M[3][1] = other.M[3][1];
		M[3][2] = other.M[3][2];
		M[3][3] = other.M[3][3];
	}

public:
	Matrix4f& operator=(const Matrix4f& other)
	{
		M[0][0] = other.M[0][0];
		M[0][1] = other.M[0][1];
		M[0][2] = other.M[0][2];
		M[0][3] = other.M[0][3];

		M[1][0] = other.M[1][0];
		M[1][1] = other.M[1][1];
		M[1][2] = other.M[1][2];
		M[1][3] = other.M[1][3];

		M[2][0] = other.M[2][0];
		M[2][1] = other.M[2][1];
		M[2][2] = other.M[2][2];
		M[2][3] = other.M[2][3];

		M[3][0] = other.M[3][0];
		M[3][1] = other.M[3][1];
		M[3][2] = other.M[3][2];
		M[3][3] = other.M[3][3];

		return *this;
	}

	Matrix4f operator+(const Matrix4f& other) const
	{
		Matrix4f result;

		result.M[0][0] = M[0][0] + other.M[0][0];
		result.M[0][1] = M[1][0] + other.M[1][0];
		result.M[0][2] = M[2][0] + other.M[2][0];
		result.M[0][3] = M[3][0] + other.M[3][0];

		result.M[1][0] = M[0][1] + other.M[0][1];
		result.M[1][1] = M[1][1] + other.M[1][1];
		result.M[1][2] = M[2][1] + other.M[2][1];
		result.M[1][3] = M[3][1] + other.M[3][1];

		result.M[2][0] = M[0][2] + other.M[0][1];
		result.M[2][1] = M[1][2] + other.M[1][1];
		result.M[2][2] = M[2][2] + other.M[2][1];
		result.M[2][3] = M[3][2] + other.M[3][1];

		result.M[3][0] = M[0][3] + other.M[0][1];
		result.M[3][1] = M[1][3] + other.M[1][1];
		result.M[3][2] = M[2][3] + other.M[2][1];
		result.M[3][3] = M[3][3] + other.M[3][1];

		return result;
	}

	Matrix4f& operator+=(const Matrix4f& other)
	{
		M[0][0] += other.M[0][0];
		M[1][0] += other.M[1][0];
		M[2][0] += other.M[2][0];
		M[3][0] += other.M[3][0];

		M[0][1] += other.M[0][1];
		M[1][1] += other.M[1][1];
		M[2][1] += other.M[2][1];
		M[3][1] += other.M[3][1];

		M[0][2] += other.M[0][1];
		M[1][2] += other.M[1][1];
		M[2][2] += other.M[2][1];
		M[3][2] += other.M[3][1];

		M[0][3] += other.M[0][1];
		M[1][3] += other.M[1][1];
		M[2][3] += other.M[2][1];
		M[3][3] += other.M[3][1];

		return *this;
	}

	Matrix4f operator-(const Matrix4f& other) const
	{
		Matrix4f result;

		result.M[0][0] = M[0][0] - other.M[0][0];
		result.M[0][1] = M[1][0] - other.M[1][0];
		result.M[0][2] = M[2][0] - other.M[2][0];
		result.M[0][3] = M[3][0] - other.M[3][0];

		result.M[1][0] = M[0][1] - other.M[0][1];
		result.M[1][1] = M[1][1] - other.M[1][1];
		result.M[1][2] = M[2][1] - other.M[2][1];
		result.M[1][3] = M[3][1] - other.M[3][1];

		result.M[2][0] = M[0][2] - other.M[0][1];
		result.M[2][1] = M[1][2] - other.M[1][1];
		result.M[2][2] = M[2][2] - other.M[2][1];
		result.M[2][3] = M[3][2] - other.M[3][1];

		result.M[3][0] = M[0][3] - other.M[0][1];
		result.M[3][1] = M[1][3] - other.M[1][1];
		result.M[3][2] = M[2][3] - other.M[2][1];
		result.M[3][3] = M[3][3] - other.M[3][1];

		return result;
	}

	Matrix4f& operator-=(const Matrix4f& other)
	{
		M[0][0] -= other.M[0][0];
		M[1][0] -= other.M[1][0];
		M[2][0] -= other.M[2][0];
		M[3][0] -= other.M[3][0];

		M[0][1] -= other.M[0][1];
		M[1][1] -= other.M[1][1];
		M[2][1] -= other.M[2][1];
		M[3][1] -= other.M[3][1];

		M[0][2] -= other.M[0][1];
		M[1][2] -= other.M[1][1];
		M[2][2] -= other.M[2][1];
		M[3][2] -= other.M[3][1];

		M[0][3] -= other.M[0][1];
		M[1][3] -= other.M[1][1];
		M[2][3] -= other.M[2][1];
		M[3][3] -= other.M[3][1];

		return *this;
	}

	Matrix4f operator*(const Matrix4f& other) const
	{
		Matrix4f result;

		result.M[0][0] = M[0][0] * other.M[0][0] + M[0][1] * other.M[1][0] + M[0][2] * other.M[2][0] + M[0][3] * other.M[3][0];
		result.M[0][1] = M[0][0] * other.M[0][1] + M[0][1] * other.M[1][1] + M[0][2] * other.M[2][1] + M[0][3] * other.M[3][1];
		result.M[0][2] = M[0][0] * other.M[0][2] + M[0][1] * other.M[1][2] + M[0][2] * other.M[2][2] + M[0][3] * other.M[3][2];
		result.M[0][3] = M[0][0] * other.M[0][3] + M[0][1] * other.M[1][3] + M[0][2] * other.M[2][3] + M[0][3] * other.M[3][3];

		result.M[1][0] = M[1][0] * other.M[0][0] + M[1][1] * other.M[1][0] + M[1][2] * other.M[2][0] + M[1][3] * other.M[3][0];
		result.M[1][1] = M[1][0] * other.M[0][1] + M[1][1] * other.M[1][1] + M[1][2] * other.M[2][1] + M[1][3] * other.M[3][1];
		result.M[1][2] = M[1][0] * other.M[0][2] + M[1][1] * other.M[1][2] + M[1][2] * other.M[2][2] + M[1][3] * other.M[3][2];
		result.M[1][3] = M[1][0] * other.M[0][3] + M[1][1] * other.M[1][3] + M[1][2] * other.M[2][3] + M[1][3] * other.M[3][3];

		result.M[2][0] = M[2][0] * other.M[0][0] + M[2][1] * other.M[1][0] + M[2][2] * other.M[2][0] + M[2][3] * other.M[3][0];
		result.M[2][1] = M[2][0] * other.M[0][1] + M[2][1] * other.M[1][1] + M[2][2] * other.M[2][1] + M[2][3] * other.M[3][1];
		result.M[2][2] = M[2][0] * other.M[0][2] + M[2][1] * other.M[1][2] + M[2][2] * other.M[2][2] + M[2][3] * other.M[3][2];
		result.M[2][3] = M[2][0] * other.M[0][3] + M[2][1] * other.M[1][3] + M[2][2] * other.M[2][3] + M[2][3] * other.M[3][3];

		result.M[3][0] = M[3][0] * other.M[0][0] + M[3][1] * other.M[1][0] + M[3][2] * other.M[2][0] + M[3][3] * other.M[3][0];
		result.M[3][1] = M[3][0] * other.M[0][1] + M[3][1] * other.M[1][1] + M[3][2] * other.M[2][1] + M[3][3] * other.M[3][1];
		result.M[3][2] = M[3][0] * other.M[0][2] + M[3][1] * other.M[1][2] + M[3][2] * other.M[2][2] + M[3][3] * other.M[3][2];
		result.M[3][3] = M[3][0] * other.M[0][3] + M[3][1] * other.M[1][3] + M[3][2] * other.M[2][3] + M[3][3] * other.M[3][3];

		return result;
	}

public:
	bool operator==(const Matrix4f& other) const
	{
		return
			M[0][0] == other.M[0][0] &&
			M[1][0] == other.M[1][0] &&
			M[2][0] == other.M[2][0] &&
			M[3][0] == other.M[3][0] &&

			M[0][1] == other.M[0][1] &&
			M[1][1] == other.M[1][1] &&
			M[2][1] == other.M[2][1] &&
			M[3][1] == other.M[3][1] &&

			M[0][2] == other.M[0][1] &&
			M[1][2] == other.M[1][1] &&
			M[2][2] == other.M[2][1] &&
			M[3][2] == other.M[3][1] &&

			M[0][3] == other.M[0][1] &&
			M[1][3] == other.M[1][1] &&
			M[2][3] == other.M[2][1] &&
			M[3][3] == other.M[3][1];
	}

	bool operator!=(const Matrix4f& other) const
	{
		return
			M[0][0] != other.M[0][0] ||
			M[1][0] != other.M[1][0] ||
			M[2][0] != other.M[2][0] ||
			M[3][0] != other.M[3][0] ||

			M[0][1] != other.M[0][1] ||
			M[1][1] != other.M[1][1] ||
			M[2][1] != other.M[2][1] ||
			M[3][1] != other.M[3][1] ||

			M[0][2] != other.M[0][1] ||
			M[1][2] != other.M[1][1] ||
			M[2][2] != other.M[2][1] ||
			M[3][2] != other.M[3][1] ||

			M[0][3] != other.M[0][1] ||
			M[1][3] != other.M[1][1] ||
			M[2][3] != other.M[2][1] ||
			M[3][3] != other.M[3][1];
	}

public:
	template<typename T>
	Matrix4f operator+(T value) const
	{
		Matrix4f result;

		result.M[0][0] = M[0][0] + value;
		result.M[0][1] = M[1][0] + value;
		result.M[0][2] = M[2][0] + value;
		result.M[0][3] = M[3][0] + value;

		result.M[1][0] = M[0][1] + value;
		result.M[1][1] = M[1][1] + value;
		result.M[1][2] = M[2][1] + value;
		result.M[1][3] = M[3][1] + value;

		result.M[2][0] = M[0][2] + value;
		result.M[2][1] = M[1][2] + value;
		result.M[2][2] = M[2][2] + value;
		result.M[2][3] = M[3][2] + value;

		result.M[3][0] = M[0][3] + value;
		result.M[3][1] = M[1][3] + value;
		result.M[3][2] = M[2][3] + value;
		result.M[3][3] = M[3][3] + value;

		return result;
	}

	template<typename T>
	Matrix4f& operator+=(T value)
	{
		M[0][0] += value;
		M[1][0] += value;
		M[2][0] += value;
		M[3][0] += value;

		M[0][1] += value;
		M[1][1] += value;
		M[2][1] += value;
		M[3][1] += value;

		M[0][2] += value;
		M[1][2] += value;
		M[2][2] += value;
		M[3][2] += value;

		M[0][3] += value;
		M[1][3] += value;
		M[2][3] += value;
		M[3][3] += value;

		return *this;
	}

	template<typename T>
	Matrix4f operator-(T value) const
	{
		Matrix4f result;

		result.M[0][0] = M[0][0] - value;
		result.M[0][1] = M[1][0] - value;
		result.M[0][2] = M[2][0] - value;
		result.M[0][3] = M[3][0] - value;

		result.M[1][0] = M[0][1] - value;
		result.M[1][1] = M[1][1] - value;
		result.M[1][2] = M[2][1] - value;
		result.M[1][3] = M[3][1] - value;

		result.M[2][0] = M[0][2] - value;
		result.M[2][1] = M[1][2] - value;
		result.M[2][2] = M[2][2] - value;
		result.M[2][3] = M[3][2] - value;

		result.M[3][0] = M[0][3] - value;
		result.M[3][1] = M[1][3] - value;
		result.M[3][2] = M[2][3] - value;
		result.M[3][3] = M[3][3] - value;

		return result;
	}

	template<typename T>
	Matrix4f& operator-=(T value)
	{
		M[0][0] -= value;
		M[1][0] -= value;
		M[2][0] -= value;
		M[3][0] -= value;

		M[0][1] -= value;
		M[1][1] -= value;
		M[2][1] -= value;
		M[3][1] -= value;

		M[0][2] -= value;
		M[1][2] -= value;
		M[2][2] -= value;
		M[3][2] -= value;

		M[0][3] -= value;
		M[1][3] -= value;
		M[2][3] -= value;
		M[3][3] -= value;

		return *this;
	}

	template<typename T>
	Matrix4f operator*(T value) const
	{
		Matrix4f result;

		result.M[0][0] = M[0][0] * value;
		result.M[0][1] = M[1][0] * value;
		result.M[0][2] = M[2][0] * value;
		result.M[0][3] = M[3][0] * value;

		result.M[1][0] = M[0][1] * value;
		result.M[1][1] = M[1][1] * value;
		result.M[1][2] = M[2][1] * value;
		result.M[1][3] = M[3][1] * value;

		result.M[2][0] = M[0][2] * value;
		result.M[2][1] = M[1][2] * value;
		result.M[2][2] = M[2][2] * value;
		result.M[2][3] = M[3][2] * value;

		result.M[3][0] = M[0][3] * value;
		result.M[3][1] = M[1][3] * value;
		result.M[3][2] = M[2][3] * value;
		result.M[3][3] = M[3][3] * value;

		return result;
	}

	template<typename T>
	Matrix4f& operator*=(T value)
	{
		M[0][0] *= value;
		M[1][0] *= value;
		M[2][0] *= value;
		M[3][0] *= value;

		M[0][1] *= value;
		M[1][1] *= value;
		M[2][1] *= value;
		M[3][1] *= value;

		M[0][2] *= value;
		M[1][2] *= value;
		M[2][2] *= value;
		M[3][2] *= value;

		M[0][3] *= value;
		M[1][3] *= value;
		M[2][3] *= value;
		M[3][3] *= value;

		return *this;
	}

public:
	float operator()(int col, int row) const
	{
		return M[col][row];
	}

public:
	Matrix4f Transpose() const
	{
		Matrix4f result;

		result.M[0][0] = M[0][0];
		result.M[0][1] = M[1][0];
		result.M[0][2] = M[2][0];
		result.M[0][3] = M[3][0];

		result.M[1][0] = M[0][1];
		result.M[1][1] = M[1][1];
		result.M[1][2] = M[2][1];
		result.M[1][3] = M[3][1];

		result.M[2][0] = M[0][2];
		result.M[2][1] = M[1][2];
		result.M[2][2] = M[2][2];
		result.M[2][3] = M[3][2];

		result.M[3][0] = M[0][3];
		result.M[3][1] = M[1][3];
		result.M[3][2] = M[2][3];
		result.M[3][3] = M[3][3];

		return result;
	}

	float Determinant() const
	{
		return	M[0][0] * (
			M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
			M[2][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) +
			M[3][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
			) -
			M[1][0] * (
				M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
				M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
				M[3][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2])
				) +
			M[2][0] * (
				M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) -
				M[1][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
				M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
				) -
			M[3][0] * (
				M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) -
				M[1][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2]) +
				M[2][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
				);
	}

	Matrix4f Inverse() const
	{
		const float determinant = Determinant();

		if (determinant == 0.0f)
		{
			return Matrix4f::Identity();

			return Matrix4f(); //Matrix4f::Identity;
		}
		else
		{
			Matrix4f temp;
			Matrix4f result;
		}
	}

	void MakeIdentity()
	{
		M[0][0] = 1.0f;
		M[0][1] = 0.0f;
		M[0][2] = 0.0f;
		M[0][3] = 0.0f;

		M[1][0] = 0.0f;
		M[1][1] = 1.0f;
		M[1][2] = 0.0f;
		M[1][3] = 0.0f;

		M[2][0] = 0.0f;
		M[2][1] = 0.0f;
		M[2][2] = 1.0f;
		M[2][3] = 0.0f;

		M[3][0] = 0.0f;
		M[3][1] = 0.0f;
		M[3][2] = 0.0f;
		M[3][3] = 1.0f;
	}

	float Eigenvalue() const
	{
		//TODO
	}

	float Eigenvector() const
	{
		//TODO
	}

public:
	static Matrix4f Identity()
	{
		return Matrix4f(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	class Proxy
	{
	public:
		Proxy(float* _array)
			: _array(_array)
		{
		}

		float operator[](int index)
		{
			return _array[index];
		}
	private:
		float* _array;
	};

	Proxy operator[](int index)
	{
		return Proxy(M[index]);
	}
};

inline std::ostream& operator<<(std::ostream& outputStream, const Matrix4f& matrix)
{
	outputStream << "[" << matrix.M[0][0] << ", " << matrix.M[1][0] << ", " << matrix.M[2][0] << ", " << matrix.M[3][0] << "]," << "\n";
	outputStream << "[" << matrix.M[0][1] << ", " << matrix.M[1][1] << ", " << matrix.M[2][1] << ", " << matrix.M[3][1] << "]," << "\n";
	outputStream << "[" << matrix.M[0][2] << ", " << matrix.M[1][2] << ", " << matrix.M[2][2] << ", " << matrix.M[3][2] << "]," << "\n";
	outputStream << "[" << matrix.M[0][3] << ", " << matrix.M[1][3] << ", " << matrix.M[2][3] << ", " << matrix.M[3][3] << "]";

	return outputStream;
}