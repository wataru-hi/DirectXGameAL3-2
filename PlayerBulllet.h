#pragma once

#include "KamataEngine.h"

using namespace KamataEngine;

class PlayerBulllet
{
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const Camera& viewProjection);
private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
};
