#pragma once

#include "Matrix4f.h"
#include "Vector3f.h"

namespace MathLib
{
	inline Matrix4f Rotate(const Matrix4f& matrix, const float& angle, const Vector3f& axis)
	{
		return matrix;
	}

	inline Matrix4f LookAt(const Vector3f& eyePosition, const Vector3f& centerPosition, const Vector3f& upAxis)
	{
		return Matrix4f::Identity();
	}

	inline Matrix4f Perspective(const float& fieldOfView, const float& aspectRatio, const float& nearPlane, const float& farPlane)
	{
		return Matrix4f::Identity();
	}
}