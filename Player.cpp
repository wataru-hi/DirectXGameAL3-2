#include "Player.h"
#include "Affin.h"

using namespace MathUtility;

Player::~Player()
{
}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	input_ = Input::GetInstance();
}

void Player::Update()
{
	bullets_.remove_if([](std::shared_ptr<PlayerBulllet> bullet) {return bullet->IsDead(); });

	Vector3 move = {0,0,0};

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCaracterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)){
		move.x += kCaracterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCaracterSpeed;
	}
	else if (input_->PushKey(DIK_UP)){
		move.y += kCaracterSpeed;
	}
	
	Rotate();

	worldTransform_.translation_ += move;
	
	float PlayerTranslate[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z};

	ImGui::Begin("data");
	ImGui::DragFloat3("PlayerPos", PlayerTranslate, 0.1f, 1.0f);
	ImGui::End();

	worldTransform_.translation_.x = PlayerTranslate[0];
	worldTransform_.translation_.y = PlayerTranslate[1];
	worldTransform_.translation_.z = PlayerTranslate[2];

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	Attack();

	for (std::shared_ptr<PlayerBulllet> bullet : bullets_)
	{
		bullet->Update();
	}

	worldTransform_.UpdateMatrix();
}

void Player::Draw(Camera& camera)
{
	model_->Draw(worldTransform_, camera, textureHandle_);
	for (std::shared_ptr<PlayerBulllet> bullet : bullets_)
	{
		bullet->Draw(camera);
	}
}

void Player::Rotate()
{
	const float kRotSpeed = 0.02f;

	if (input_->PushKey(DIK_A))
	{
		worldTransform_.rotation_.y += kRotSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		worldTransform_.rotation_.y -= kRotSpeed;
	}

}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE))
	{
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		std::shared_ptr<PlayerBulllet> bullet(new PlayerBulllet);
		bullet->Initialize(model_, worldTransform_.translation_, velocity);
		bullets_.push_back(bullet);
			
	}
}
