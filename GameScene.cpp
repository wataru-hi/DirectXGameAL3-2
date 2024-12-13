#include "GameScene.h"


#include "Skydome.h"
#include "RailCamera.h"

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

	skydomeModel_ = Model::CreateFromOBJ("SkyDome"); // Skydome.objを読み込む
    skydome_ = new Skydome();
    skydome_->Initialize(skydomeModel_);

	playerTextureHandle_ = TextureManager::Load("mario.jpg");
	playerModel_ = Model::Create();

	enemyTextureHandle_ = TextureManager::Load("Enemy.png");
	enemyModel_ = Model::Create();

	camera.Initialize();

	player_ = new Player();
	

	enemy_ = new Enemy();
	enemy_->Initialize(enemyModel_, enemyTextureHandle_, Vector3(10, 2, 20));
	enemy_->SetPlayer(player_);

	railCamera = new RailCamera();
	railCamera->Initialize(camera.translation_, camera.rotation_);

	player_->SetParent(&railCamera->GetWorldTransform());

	Vector3 playerPostion( 0.0f, 0.0f, 10.0f);
	player_->Initialize(playerModel_, playerTextureHandle_, playerPostion);

	player_->SetGameScene(this);
	enemy_->SetGameScene(this);

	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&camera);
}

void GameScene::Update() {
	 skydome_->Update();
	
	player_->Update();
	enemy_->Update();

	BulletUpdate();
	CheakAllCollisions();

	railCamera->Update();
	camera.matView = railCamera->GetCamera().matView;
	camera.matProjection = railCamera->GetCamera().matProjection;
	camera.UpdateMatrix();
	

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
	
	 skydome_->Draw(camera); 

	if (enemy_ != nullptr) {
		enemy_->Draw(camera);
	}

	BulletDrow();
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

	for (const auto& bullet : enemybullets_) {
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
	for (const auto& bullet : playerbullets_) {
		Vector3 bulletPos = bullet->GetWorldPosition();
		float bulletRadius = radius;

		if (IsCollisionSphereAndSphere(enemyPos, enemyRadius, bulletPos, bulletRadius)) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
}

void GameScene::BulletUpdate()
{
	playerbullets_.remove_if([](std::shared_ptr<PlayerBulllet> a) { return a->IsDead(); });
	enemybullets_.remove_if([](std::shared_ptr<EnemyBullet> a) { return a->IsDead(); });
	for (std::shared_ptr<PlayerBulllet> Bullet : playerbullets_)
	{
		Bullet->Update();
	}
	for (std::shared_ptr<EnemyBullet> Bullet : enemybullets_)
	{
		Bullet->Update();
	}
}

void GameScene::BulletDrow()
{
	for (std::shared_ptr<PlayerBulllet> Bullet : playerbullets_)
	{
		Bullet->Draw(camera);
	}
	for (std::shared_ptr<EnemyBullet> Bullet : enemybullets_)
	{
		Bullet->Draw(camera);
	}
}