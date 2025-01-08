#include "GameScene.h"

#include "RailCamera.h"
#include "Skydome.h"

#include "Enemy.h"
#include "Player.h"

#include "EnemyBullet.h"
#include "PlayerBulllet.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace MathUtility;
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete playerModel_;
	delete enemyModel_;
	delete debugCamera_;

	enemies.clear();
	playerbullets_.clear();
	enemybullets_.clear();
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

	railCamera = new RailCamera();
	railCamera->Initialize(camera.translation_, camera.rotation_);


	player_ = new Player();
	player_->SetParent(&railCamera->GetWorldTransform());

	Vector3 playerPostion(0.0f, 0.0f, 20.0f);
	player_->Initialize(playerModel_, playerTextureHandle_, playerPostion);

	player_->SetGameScene(this);

	//csvファイルデータ
	LoadEnemyPopDate();

	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&camera);

	gameEnd = false;
	dead = false;
	time = 0;
	enemyKillCount = 0;
	BGMHandle = audio_->LoadWave("Viper.mp3");
	

}

void GameScene::Update() {
	UpdateEnemyPopDate();

	if (!start)
	{
		
	BGMStart = audio_->PlayWave(BGMHandle, true);
	start = true;
	}

	time++;
	if (time == 1100)
	{
		gameEnd = true;
	}
	if (enemyKillCount == 2)
	{
		gameEnd = true;
	}
	skydome_->Update();
	for (std::shared_ptr<Enemy> enemy : enemies)
	{
		enemy->Update();
	}

	enemies.remove_if([](std::shared_ptr<Enemy> a) { return a->IsDead(); });

	player_->Update();
	

	BulletUpdate();
	CheakAllCollisions();

	railCamera->Update();
	camera.matView = railCamera->GetCamera().matView;
	camera.matProjection = railCamera->GetCamera().matProjection;
	camera.TransferMatrix();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

	ImGui::Begin("p");
	ImGui::Text("%d",time);
	ImGui::End();
#endif
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera.matView = debugCamera_->GetCamera().matView;
		camera.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクションの転送
		camera.TransferMatrix();
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

	for (std::shared_ptr<Enemy> enemy : enemies)
	{
		enemy->Draw(camera);
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
			gameEnd = true;
			dead = true;
		}
	}

	// 敵とプレイヤーの弾の当たり判定
	for(std::shared_ptr<Enemy> enemy : enemies){
		Vector3 enemyPos = enemy->GetWorldPosition();
		float enemyRadius = radius;
		for (const auto& bullet : playerbullets_) {
			Vector3 bulletPos = bullet->GetWorldPosition();
			float bulletRadius = radius;

			if (IsCollisionSphereAndSphere(enemyPos, enemyRadius, bulletPos, bulletRadius)) {
				enemy->OnCollision();
				bullet->OnCollision();
				enemyKillCount++;
			}
		}
	}
}

void GameScene::BulletUpdate() {
	playerbullets_.remove_if([](std::shared_ptr<PlayerBulllet> a) { return a->IsDead(); });
	enemybullets_.remove_if([](std::shared_ptr<EnemyBullet> a) { return a->IsDead(); });
	for (std::shared_ptr<PlayerBulllet> Bullet : playerbullets_) {
		Bullet->Update();
	}
	for (std::shared_ptr<EnemyBullet> Bullet : enemybullets_) {
		Bullet->Update();
	}
}

void GameScene::BulletDrow() {
	for (std::shared_ptr<PlayerBulllet> Bullet : playerbullets_) {
		Bullet->Draw(camera);
	}
	for (std::shared_ptr<EnemyBullet> Bullet : enemybullets_) {
		Bullet->Draw(camera);
	}
}

void GameScene::LoadEnemyPopDate() {
	std::ifstream file;

	file.open("Resources/EnemyPop.csv"); // Added closing quote
#ifdef _DEBUG
	assert(file.is_open());
#endif
	enemyPosCommand << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopDate() {
	std::string line;
	if (popIsDirei) {
		waitTimer--;
		if (waitTimer <= 0)
		{
			popIsDirei = false;
		}
		return;
	}

	while (std::getline(enemyPosCommand, line)) {
		std::istringstream line_stream(line);

		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			std::shared_ptr<Enemy> enemy(new Enemy);
			enemy->Initialize(enemyModel_, enemyTextureHandle_, Vector3(x, y, z));
			enemy->SetPlayer(player_);
			enemy->SetGameScene(this);
			enemies.push_back(enemy);
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ','); // 待機時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			popIsDirei = true;
			waitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}
