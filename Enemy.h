#pragma once
#include "KamataEngine.h"

#include "Affin.h"

class EnemyBullet;
class Player;

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

	void SetPlayer(Player* player){ player_ = player; }

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition(){ return GetWorldPos(worldTransform_); }

	const std::list<std::shared_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

	void OnCollision();
private:
	void fire();

	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0;

	bool isDead_ = false;

	Phase phase_ = Phase::Apprach;

	std::list<std::shared_ptr<EnemyBullet>> bullets_;

	float Timer = 3.0f;
	Player* player_ = nullptr;
};