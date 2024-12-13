#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);
	model_  = model;

	worldTransform_.Initialize();
	worldTransform_.translation_  = position;

	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	if (--dethTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_.z += velocity_.z;

	ImGui::Begin("bullet");
	ImGui::Text("%f, %f, %f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const Camera& camera)
{
	model_->Draw(worldTransform_, camera);
}
