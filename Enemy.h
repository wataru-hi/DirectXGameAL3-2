#pragma once
#include "KamataEngine.h"

#include "Affin.h"

class Player;
class GameScene;

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

	void OnCollision();

	 void SetGameScene(GameScene* gameScene){ gameScene_ = gameScene; }
private:
	void Fire();

	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0;

	bool isDead_ = false;

	Phase phase_ = Phase::Apprach;

	float Timer = 3.0f;
	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;
};