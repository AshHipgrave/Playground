#pragma once

#include "MathCommon.h"

class Vector2f
{
public:
	float X;
	float Y;

public:
	Vector2f()
		: X(0.0f), Y(0.0f)
	{
	}

	Vector2f(float x, float y)
		: X(x), Y(y)
	{
	}

	Vector2f(const Vector2f& other)
	{
		X = other.X;
		Y = other.Y;
	}

public:
	Vector2f& operator=(const Vector2f& other)
	{
		X = other.X;
		Y = other.Y;

		return *this;
	}

	Vector2f operator+(const Vector2f& other) const
	{
		return Vector2f(X + other.X, Y + other.Y);
	}

	Vector2f& operator+=(const Vector2f& other)
	{
		X += other.X;
		Y += other.Y;

		return *this;
	}

	Vector2f operator-(const Vector2f& other) const
	{
		return Vector2f(X - other.X, Y - other.Y);
	}

	Vector2f& operator-=(const Vector2f& other)
	{
		X -= other.X;
		Y -= other.Y;

		return *this;
	}

	Vector2f operator*(const Vector2f& other) const
	{
		return Vector2f(X * other.X, Y * other.Y);
	}

public:
	bool operator==(const Vector2f& other) const
	{
		return X == other.X || Y == other.Y;
	}

	bool operator!=(const Vector2f& other) const
	{
		return X != other.X || Y != other.Y;
	}

public:
	template<typename T>
	Vector2f operator+(T value) const
	{
		return Vector2f(X + value, Y + value);
	}

	template<typename T>
	Vector2f& operator+=(T value)
	{
		X += value;
		Y += value;

		return *this;
	}

	template<typename T>
	Vector2f operator-(T value) const
	{
		return Vector2f(X - value, Y - value);
	}

	template<typename T>
	Vector2f& operator-=(T value)
	{
		X -= value;
		Y -= value;

		return *this;
	}

	template<typename T>
	Vector2f operator*(T value) const
	{
		return Vector2f(X * value, Y * value);
	}

	template<typename T>
	Vector2f& operator*=(T value)
	{
		X *= value;
		Y *= value;

		return *this;
	}

	template<typename T>
	Vector2f operator/(T value) const
	{
		float scalar = (float)(1 / value);

		return Vector2f(X * scalar, Y * scalar);
	}

	template<typename T>
	Vector2f& operator/=(T value)
	{
		float scalar = (float)(1 / value);

		X *= scalar;
		Y *= scalar;

		return *this;
	}

public:
	float operator[](int index) const
	{
		if (index == 0) return X;
		
		return Y;
	}

	float& operator[](int index)
	{
		if (index == 0) return X;
		
		return Y;
	}

public:
	float LengthSquared() const
	{
		return X * X + Y * Y;
	}

	float Length() const
	{
		return std::sqrt(LengthSquared());
	}

	Vector2f Normalize() const
	{
		Vector2f normalised = *this / this->Length();

		return normalised;
	}

	Vector2f& Normalize()
	{
		this->Normalize();

		return *this;
	}	
	
	static float Dot(Vector2f vec1, Vector2f vec2)
	{
		return (vec1.X * vec2.X) + (vec1.Y * vec2.Y);
	}
};

std::ostream& operator<<(std::ostream& outputStream, const Vector2f& vector)
{
	return outputStream << vector.X << ", " << vector.Y;
}