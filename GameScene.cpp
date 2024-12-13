#include "GameScene.h"

#include "Enemy.h"
#include "Player.h"

#include "EnemyBullet.h"
#include "PlayerBulllet.h"
using namespace MathUtility;
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete playerModel_;
	delete enemyModel_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	playerTextureHandle_ = TextureManager::Load("mario.jpg");
	playerModel_ = Model::Create();

	enemyTextureHandle_ = TextureManager::Load("Enemy.png");
	enemyModel_ = Model::Create();

	camera.Initialize();

	player_ = new Player();
	player_->Initialize(playerModel_, playerTextureHandle_);

	enemy_ = new Enemy();
	enemy_->Initialize(enemyModel_, enemyTextureHandle_, Vector3(10, 2, 20));
	enemy_->SetPlayer(player_);

	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&camera);
}

void GameScene::Update() {
	player_->Update();
	enemy_->Update();
	CheakAllCollisions();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera.matView = debugCamera_->GetCamera().matView;
		camera.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクションの転送
		camera.TransferMatrix();
	} else {
		camera.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	if (enemy_ != nullptr) {
		enemy_->Draw(camera);
	}
	player_->Draw(camera);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

bool GameScene::IsCollisionSphereAndSphere(const Vector3& posA, float radiusA, const Vector3& posB, float radiusB) {
	float distance = Length((posA - posB));
	return distance <= radiusA + radiusB;
}

void GameScene::CheakAllCollisions() {
	float radius = 1.0f;
	Vector3 playerPos = player_->GetWorldPosition();
	float playerRadius = radius;
	const std::list<std::shared_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

	for (const auto& bullet : enemyBullets) {
		Vector3 bulletPos = bullet->GetWorldPosition();
		float bulletRadius = radius;

		if (IsCollisionSphereAndSphere(playerPos, playerRadius, bulletPos, bulletRadius)) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}

	// 敵とプレイヤーの弾の当たり判定
	Vector3 enemyPos = enemy_->GetWorldPosition();
	float enemyRadius = radius;
	const std::list<std::shared_ptr<PlayerBulllet>>& playerBullets = player_->GetBullets();
	for (const auto& bullet : playerBullets) {
		Vector3 bulletPos = bullet->GetWorldPosition();
		float bulletRadius = radius;

		if (IsCollisionSphereAndSphere(enemyPos, enemyRadius, bulletPos, bulletRadius)) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
}
