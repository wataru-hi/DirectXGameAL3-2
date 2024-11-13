#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;


Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeRotateMatrix(const Vector3& radian );
Matrix4x4 MakeAffinMatrix(const Vector3& scale, const Vector3& lotate, const Vector3& translate);

