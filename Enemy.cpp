#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle, const Vector3& position)
{
	assert(model);
	model_ = model;

	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Enemy::Update()
{
	static const float moveSpeed = -0.1f;

	worldTransform_.translation_.z += moveSpeed;

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const Camera& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
