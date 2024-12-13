#include "Enemy.h"
#include "EnemyBullet.h"

#include "Player.h"

using namespace MathUtility;

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

	Timer -= 1.0f / 60.0f;

	//worldTransform_.translation_.z += moveSpeed;
	//Move();

	PrintImGui();

	if (Timer < 0.0f)
	{
		fire();
		Timer = 1.5f;
	}

	for (std::shared_ptr<EnemyBullet> bullet : bullets_)
	{
		bullet->Update();
	}
	bullets_.remove_if([](const std::shared_ptr<EnemyBullet>& bullet) { return bullet->IsDead();});

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const Camera& camera)
{
	model_->Draw(worldTransform_, camera);

	for (std::shared_ptr<EnemyBullet> enemyBullet : bullets_)
	{
		enemyBullet->Draw(camera);
	}
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
	ImGui::Text("Timer %f", Timer);
	ImGui::End();

	Set(&(*pos), &worldTransform_.translation_);
}

void Enemy::OnCollision()
{
}

void Enemy::fire()
{
	const float kBulletSpeed = 1.0f;//弾の速さ

	const Vector3 playerPos = player_->GetWorldPosition();
	const Vector3 enemyPos = GetWorldPosition();
	
	Vector3 direction = playerPos - enemyPos;
	Normalize(direction);
	direction * kBulletSpeed;

	std::shared_ptr<EnemyBullet> bullet(new EnemyBullet);
	bullet->Initialize(model_, worldTransform_.translation_, direction);
	bullets_.push_back(bullet);
}
