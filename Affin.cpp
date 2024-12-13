#include "Affin.h"
using namespace MathUtility;
Vector3 GetWorldPos(const WorldTransform& v)
{
	return { v.matWorld_.m[3][0], v.matWorld_.m[3][1], v.matWorld_.m[3][2] };
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result = {};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}

		}
	}

	return result;
}

Matrix4x4 MakeRotateMatrix(const Vector3& radian)
{
	Matrix4x4 result = { 0 };

	Matrix4x4 rotateX = MakeRotateXMatrix(radian.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(radian.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(radian.z);

	result = Multiply(rotateX, Multiply(rotateY, rotateZ));

	return result;
}


Matrix4x4 MakeAffinMatrix(const Vector3& scale, const Vector3& lotate, const Vector3& translate) {
	Matrix4x4 result = {0};

	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateMatrix = MakeRotateMatrix(lotate);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	result = Multiply(scaleMatrix, rotateMatrix);
	result = Multiply(result, translateMatrix);

	return result;
}

