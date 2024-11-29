#pragma once

#include "KamataEngine.h"

using namespace KamataEngine;

class PlayerBulllet
{
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const Camera& viewProjection);

	bool IsDead() const { return isDead_; }
private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t dethTimer_ = kLifeTime;
	bool isDead_ = false;
};
