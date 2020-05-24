#pragma once

#include "MathCommon.h"

class Vector4f
{
public:
	float X;
	float Y;
	float Z;
	float W;

public:
	Vector4f()
		: X(0.0f), Y(0.0f), Z(0.0f), W(0.0f)
	{
	}

	Vector4f(float x, float y, float z, float w)
		: X(x), Y(y), Z(z), W(w)
	{
	}

	Vector4f(const Vector4f& other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;
		W = other.W;
	}

public:
	Vector4f& operator=(const Vector4f& other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;
		W = other.W;

		return *this;
	}

	Vector4f operator+(const Vector4f& other) const
	{
		return Vector4f(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
	}

	Vector4f& operator+=(const Vector4f& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		W += other.W;

		return *this;
	}

	Vector4f operator-(const Vector4f& other) const
	{
		return Vector4f(X - other.X, Y - other.Y, Z - other.Z, W - other.W);
	}

	Vector4f& operator-=(const Vector4f& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		W -= other.W;

		return *this;
	}

	Vector4f operator*(const Vector4f& other) const
	{
		return Vector4f(X * other.X, Y * other.Y, Z * other.Z, W * other.W);
	}

public:
	bool operator==(const Vector4f& other) const
	{
		return X == other.X || Y == other.Y || Z == other.Z || W == other.W;
	}

	bool operator!=(const Vector4f& other) const
	{
		return X != other.X || Y != other.Y || Z != other.Z || W != other.W;
	}

public:
	template<typename T>
	Vector4f operator+(T value) const
	{
		return Vector4f(X + value, Y + value, Z + value, W + value);
	}

	template<typename T>
	Vector4f& operator+=(T value)
	{
		X += value;
		Y += value;
		Z += value;
		W += value;

		return *this;
	}

	template<typename T>
	Vector4f operator-(T value) const
	{
		return Vector4f(X - value, Y - value, Z - value, W - value);
	}

	template<typename T>
	Vector4f& operator-=(T value)
	{
		X -= value;
		Y -= value;
		Z -= value;
		W -= value;

		return *this;
	}

	template<typename T>
	Vector4f operator*(T value) const
	{
		return Vector4f(X * value, Y * value, Z * value, W * value);
	}

	template<typename T>
	Vector4f& operator*=(T value)
	{
		X *= value;
		Y *= value;
		Z *= value;
		W *= value;

		return *this;
	}

	template<typename T>
	Vector4f operator/(T value) const
	{
		float scalar = (float)(1 / value);

		return Vector4f(X * scalar, Y * scalar, Z * scalar, W * scalar);
	}

	template<typename T>
	Vector4f& operator/=(T value)
	{
		float scalar = (float)(1 / value);

		X *= scalar;
		Y *= scalar;
		Z *= scalar;
		W *= scalar;

		return *this;
	}

public:
	float operator[](int index) const
	{
		if (index == 0) return X;
		if (index == 1) return Y;
		if (index == 2) return Z;

		return W;
	}

	float& operator[](int index)
	{
		if (index == 0) return X;
		if (index == 1) return Y;
		if (index == 2) return Z;

		return W;
	}

public:
	float LengthSquared() const
	{
		return X * X + Y * Y + Z * Z + W * W;
	}

	float Length() const
	{
		return std::sqrt(LengthSquared());
	}

	Vector4f Normalize() const
	{
		Vector4f normalised = *this / this->Length();

		return normalised;
	}

	Vector4f& Normalize()
	{
		this->Normalize();

		return *this;
	}

public:
	static float Dot(Vector4f vec1, Vector4f vec2)
	{
		return (vec1.X * vec2.X) + (vec1.Y * vec2.Y) + (vec1.Z * vec2.Z) + (vec1.W * vec2.W);
	}
};

std::ostream& operator<<(std::ostream& outputStream, const Vector4f& vector)
{
	return outputStream << vector.X << ", " << vector.Y << ", " << vector.Z << ", " << vector.W;
}