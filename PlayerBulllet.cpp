#include "PlayerBulllet.h"
#include <cassert>

using namespace MathUtility;


void PlayerBulllet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);
	model_ = model;
	
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;
}

void PlayerBulllet::Update()
{
	if (--dethTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();
}

void PlayerBulllet::Draw(const Camera& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void PlayerBulllet::OnCollision()
{
	isDead_ = true;
}
