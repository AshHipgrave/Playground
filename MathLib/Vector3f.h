#pragma once

#include "MathCommon.h"

class Vector3f
{
public:
	float X;
	float Y;
	float Z;

public:
	Vector3f()
		: X(0.0f), Y(0.0f), Z(0.0f)
	{
	}

	Vector3f(float x, float y, float z)
		: X(x), Y(y), Z(z)
	{
	}

	Vector3f(const Vector3f& other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;
	}

public:
	Vector3f& operator=(const Vector3f& other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;

		return *this;
	}

	Vector3f operator+(const Vector3f& other) const
	{
		return Vector3f(X + other.X, Y + other.Y, Z + other.Z);
	}

	Vector3f& operator+=(const Vector3f& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;

		return *this;
	}

	Vector3f operator-(const Vector3f& other) const
	{
		return Vector3f(X - other.X, Y - other.Y, Z - other.Z);
	}

	Vector3f& operator-=(const Vector3f& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;

		return *this;
	}

	Vector3f operator*(const Vector3f& other) const
	{
		return Vector3f(X * other.X, Y * other.Y, Z * other.Z);
	}

public:
	bool operator==(const Vector3f& other) const
	{
		return X == other.X || Y == other.Y || Z == other.Z;
	}

	bool operator!=(const Vector3f& other) const
	{
		return X != other.X || Y != other.Y || Z != other.Z;
	}

public:
	template<typename T>
	Vector3f operator+(T value) const
	{
		return Vector3f(X + value, Y + value, Z + value);
	}

	template<typename T>
	Vector3f& operator+=(T value)
	{
		X += value;
		Y += value;
		Z += value;

		return *this;
	}

	template<typename T>
	Vector3f operator-(T value) const
	{
		return Vector3f(X - value, Y - value, Z - value);
	}

	template<typename T>
	Vector3f& operator-=(T value)
	{
		X -= value;
		Y -= value;
		Z -= value;

		return *this;
	}

	template<typename T>
	Vector3f operator*(T value) const
	{
		return Vector3f(X * value, Y * value, Z * value);
	}

	template<typename T>
	Vector3f& operator*=(T value)
	{
		X *= value;
		Y *= value;
		Z *= value;

		return *this;
	}

	template<typename T>
	Vector3f operator/(T value) const
	{
		float scalar = (float)(1 / value);

		return Vector3f(X * scalar, Y * scalar, Z * scalar);
	}

	template<typename T>
	Vector3f& operator/=(T value)
	{
		float scalar = (float)(1 / value);

		X *= scalar;
		Y *= scalar;
		Z *= scalar;

		return *this;
	}

public:
	float operator[](int index) const
	{
		if (index == 0) return X;
		if (index == 1) return Y;

		return Z;
	}

	float& operator[](int index)
	{
		if (index == 0) return X;
		if (index == 1) return Y;

		return Z;
	}

public:
	float LengthSquared() const
	{
		return X * X + Y * Y + Z * Z;
	}

	float Length() const
	{
		return std::sqrt(LengthSquared());
	}

	Vector3f Normalize() const
	{
		Vector3f normalised = *this / this->Length();

		return normalised;
	}

public:
	static float Dot(Vector3f vec1, Vector3f vec2)
	{
		return (vec1.X * vec2.X) + (vec1.Y * vec2.Y) + (vec1.Z * vec2.Z);
	}

	static Vector3f Cross(Vector3f vec1, Vector3f vec2)
	{
		return Vector3f(
			vec1.Y * vec2.Z - vec1.Z * vec2.Y,
			vec1.Z * vec2.X - vec1.X * vec2.Z,
			vec1.X * vec2.Y - vec1.Y * vec2.X);
	}
};

inline std::ostream& operator<<(std::ostream& outputStream, const Vector3f& vector)
{
	return outputStream << vector.X << ", " << vector.Y << ", " << vector.Z;
}