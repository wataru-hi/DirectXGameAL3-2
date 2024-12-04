#pragma once

#include "KamataEngine.h"

using namespace KamataEngine;

class EnemyBullet
{
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const Camera& viewProjection);
private:
	WorldTransform worldTransform_;
	Model* model;

	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t dethTimer_ = kLifeTime;
	bool isDead_ = false;
}

