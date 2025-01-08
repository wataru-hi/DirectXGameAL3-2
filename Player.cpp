#include "Player.h"
#include "Affin.h"
#include "PlayerBulllet.h"
#include "GameScene.h"

using namespace MathUtility;

Player::~Player()
{
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 pos)
{
	//assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	shot = audio_ ->LoadWave("Shot.mp3");
}

void Player::Update()
{

	Vector3 move = {0,0,0};
	BulletTimer -= 1.0f / 60.0f;

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
	
#ifdef _DEBUG

	ImGui::Begin("data");
	ImGui::DragFloat3("PlayerPos", &worldTransform_.translation_.x, 0.1f, 1.0f);
	ImGui::End();

#endif // _DEBUG

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	Attack();

	worldTransform_.UpdateMatrix();
}

void Player::Draw(Camera& camera)
{
	model_->Draw(worldTransform_, camera, textureHandle_);
}
void Player::OnCollision()
{
}
void Player::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
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
	
	if (input_->PushKey(DIK_SPACE) && BulletTimer < 0.0f)
	{
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		std::shared_ptr<PlayerBulllet> bullet = std::make_shared<PlayerBulllet>();
		bullet->Initialize(model_, GetWorldPosition(), velocity);
		gameScene_->AddPlayerBullet(bullet);
		BulletTimer = BulletTime;

		audio_->PlayWave(shot);
	}
}
