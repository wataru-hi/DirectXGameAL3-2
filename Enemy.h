#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;
class Enemy
{
public:
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	void Update();

	void Draw(const Camera& viewProjection);

	bool IsDead() const { return isDead_; }
private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0;

	bool isDead_ = false;
};

