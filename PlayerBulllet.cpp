#include "PlayerBulllet.h"
#include <cassert>



void PlayerBulllet::Initialize(Model* model, const Vector3& position)
{
	assert(model);

	model_ = model;
	
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void PlayerBulllet::Update()
{
	worldTransform_.UpdateMatrix();
}

void PlayerBulllet::Draw(const Camera& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
