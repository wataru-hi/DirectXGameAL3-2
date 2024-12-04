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

	//worldTransform_.translation_.z += moveSpeed;
	Move();

	PrintImGui();

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const Camera& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Enemy::Move()
{
	switch (phase_)
	{
	case Phase::Apprach:
		worldTransform_.translation_.z -= 0.1f;
		if(worldTransform_.translation_.z < 0.0f)
		{
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		worldTransform_.translation_.x -= 0.1f;
		worldTransform_.translation_.y += 0.1f;
	}
}

void Enemy::PrintImGui()
{
	float pos[] = { worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z};

	ImGui::Begin("enemy");
	ImGui::DragFloat3("pos",pos, 0.1f);
	ImGui::End();

	Set(&(*pos), &worldTransform_.translation_);
}
