#pragma once

#include <cmath>

#include "Matrix4f.h"
#include "Vector3f.h"

namespace MathLib
{
	inline Matrix4f Rotate(const Matrix4f& matrix, const float& angle, const Vector3f& axis)
	{
		float c = std::cos(angle);
		float s = std::sin(angle);
		float d = 1.0f - c;

		float x = axis.X * d;
		float y = axis.Y * d;
		float z = axis.Z * d;

		float axay = x * axis.Y;
		float axaz = x * axis.Z;
		float ayaz = y * axis.Z;

		return Matrix4f(
			c + x * axis.Z, axay - s * axis.Z, axaz + s * axis.Y, 0.0f,
			axay + s * axis.Z, c + y * axis.Y, ayaz - s * axis.X, 0.0f,
			axaz - s * axis.Y, ayaz + s * axis.X, c + z * axis.Z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline Matrix4f LookAtLH(const Vector3f& eyePosition, const Vector3f& center, const Vector3f& upAxis)
	{
		Vector3f zAxis = (eyePosition - center).Normalize();
		Vector3f xAxis = Vector3f::Cross(upAxis, zAxis).Normalize();
		Vector3f yAxis = Vector3f::Cross(zAxis, xAxis);

		float xDot = Vector3f::Dot(xAxis, eyePosition);
		float yDot = Vector3f::Dot(yAxis, eyePosition);
		float zDot = Vector3f::Dot(zAxis, eyePosition);

		return Matrix4f(
			xAxis.X, yAxis.Y, zAxis.X, 0.0f,
			xAxis.Y, yAxis.Y, zAxis.Y, 0.0f,
			xAxis.Z, yAxis.Z, zAxis.Z, 0.0f,
			-xDot, -yDot, -zDot, 1.0f);
	}

	inline Matrix4f LookAtRH(const Vector3f& eyePosition, const Vector3f& center, const Vector3f& upAxis)
	{
		Vector3f zAxis = (eyePosition - center).Normalize();
		Vector3f xAxis = Vector3f::Cross(upAxis, zAxis).Normalize();
		Vector3f yAxis = Vector3f::Cross(zAxis, xAxis);

		float xDot = Vector3f::Dot(xAxis, eyePosition);
		float yDot = Vector3f::Dot(yAxis, eyePosition);
		float zDot = Vector3f::Dot(zAxis, eyePosition);

		return Matrix4f(
			xAxis.X, yAxis.Y, zAxis.X, 0.0f,
			xAxis.Y, yAxis.Y, zAxis.Y, 0.0f,
			xAxis.Z, yAxis.Z, zAxis.Z, 0.0f,
			xDot, yDot, zDot, 1.0f);
	}

	inline Matrix4f PerspectiveFoVLH(const float& fieldOfView, const float& aspectRatio, const float& nearPlane, const float& farPlane)
	{
		float yScale = MathLib::Cot(fieldOfView / 2);
		float xScale = yScale / aspectRatio;

		float zFar = farPlane / (farPlane - nearPlane);
		float zNear = nearPlane * farPlane / (farPlane - nearPlane);

		return Matrix4f(
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f,   zFar, 1.0f,
			0.0f, 0.0f, -zNear, 0.0f);
	}

	inline Matrix4f PerspectiveFoVRH(const float& fieldOfView, const float& aspectRatio, const float& nearPlane, const float& farPlane)
	{
		float yScale = MathLib::Cot(fieldOfView / 2);
		float xScale = yScale / aspectRatio;

		float zFar = farPlane / (nearPlane - farPlane);
		float zNear = nearPlane * farPlane / (nearPlane - farPlane);

		return Matrix4f(
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f,  zFar, -1.0f,
			0.0f, 0.0f, zNear, 0.0f);
	}
}