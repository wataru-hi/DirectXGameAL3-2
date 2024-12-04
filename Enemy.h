#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

enum class Phase {
	Apprach,
	Leave,
};

class Enemy {
public:
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	void Update();

	void Draw(const Camera& viewProjection);

	void Move();

	void PrintImGui();

	void Set(float* a, Vector3* b) { b->x = a[0], b->y = a[1], b->z = a[2]; }

	bool IsDead() const { return isDead_; }

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0;

	bool isDead_ = false;

	Phase phase_ = Phase::Apprach;
};
