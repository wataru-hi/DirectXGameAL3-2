#pragma once

#include "KamataEngine.h"
#include "PlayerBulllet.h"

using namespace KamataEngine;

class Player
{
public:
	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(Camera& camera);

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0;
	Input* input_ = nullptr;

	PlayerBulllet* Bullet_ = nullptr;

	//キャラクターの移動速さ
	const float kCaracterSpeed = 0.2f;
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	void Rotate();
	void Attack();
};