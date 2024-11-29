#include "Enemy.h"

void Enemy::Initialize(Model* model, const Vector3& position)
{
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Enemy::Update()
{
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const Camera& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
