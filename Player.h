#pragma once

#include "KamataEngine.h"
#include "Affin.h"

using namespace KamataEngine;

class GameScene;

class Player
{
public:
	~Player();
	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);

	void Update();

	void Draw(Camera& camera);

	Vector3 GetWorldPosition(){ return GetWorldPos(worldTransform_); };

	 void OnCollision();

	 void SetParent(const WorldTransform* parent);

	 void SetGameScene(GameScene* gameScene){ gameScene_ = gameScene; }
private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0;
	Input* input_ = nullptr;

	void Rotate();
	void Attack();

	//キャラクターの移動速さ
	const float kCaracterSpeed = 0.2f;
	const float kMoveLimitX = 13.0f;
	const float kMoveLimitY = 7.0f;

	const float kBulletSpeed = 1.0f;

	const float BulletTime = 1.5f;
	float BulletTimer = 0.0f;

	GameScene* gameScene_;

	Audio* audio_= nullptr;
	uint32_t shot = 0;

};