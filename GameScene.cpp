#include "GameScene.h"

#include "Enemy.h"
#include "Player.h"

#include <cassert>

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

	// playerTextureHandle_ = TextureManager::Load("mario.jpg");
	playerModel_ = Model::CreateFromOBJ("player");

	enemyTextureHandle_ = TextureManager::Load("Enemy.png");
	enemyModel_ = Model::Create();

	camera.Initialize();

	mainCameraP = Vector3{0.0f, 0.0f, -20.0f};
	mainCameraR = camera.rotation_;
	subCameraP = Vector3{0.0f, 20.0f, 0.0f};
	subCameraR = Vector3{1.57f, 0.0f, 0.0f};

	player_ = new Player();
	player_->Initialize(playerModel_, playerTextureHandle_);

	enemy_ = new Enemy();
	enemy_->Initialize(enemyModel_, enemyTextureHandle_, Vector3(0, 2, 20));

	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&camera);
}

void GameScene::Update() {
	if (cameraLerpTimer > 0.0f) {
		cameraLerpTimer -= 1.0f / 60.0f;
	}

	player_->Update(isMainCamera);
	// enemy_->Update();

	float manCameraPos[] = {mainCameraP.x, mainCameraP.y, mainCameraP.z};
	float manCameraRot[] = {mainCameraR.x, mainCameraR.y, mainCameraR.z};

	float subCameraPos[] = {subCameraP.x, subCameraP.y, subCameraP.z};
	float subCamereRot[] = {subCameraR.x, subCameraR.y, subCameraR.z};

	const float dragSpeed = 0.01f;

	ImGui::Begin("camera");
	ImGui::Text("cameraPos %.3f, %.3f, %.3f", camera.translation_.x, camera.translation_.y, camera.translation_.z);
	ImGui::Text("cameraRot %.3f, %.3f, %.3f", camera.rotation_.x, camera.rotation_.y, camera.rotation_.z);
	ImGui::Text("mainCamera");
	ImGui::DragFloat3("mainCameraPos", manCameraPos, dragSpeed);
	ImGui::DragFloat3("mainCameraRot", manCameraRot, dragSpeed);
	ImGui::Text("subCamera");
	ImGui::DragFloat3("subCameraPos", subCameraPos, dragSpeed);
	ImGui::DragFloat3("subCameraRot", subCamereRot, dragSpeed);
	ImGui::End();

	Set(&(*manCameraPos), &mainCameraP);
	Set(&(*manCameraRot), &mainCameraR);
	Set(&(*subCameraPos), &subCameraP);
	Set(&(*subCamereRot), &subCameraR);

	if (input_->TriggerKey(DIK_F)) {
		isMainCamera = !isMainCamera;
		cameraLerpTimer = cameraLerpTime;
	}

	if (cameraLerpTimer > 0.0f) {
		if (isMainCamera) {
			camera.translation_ = LerpVec(mainCameraP, subCameraP, cameraLerpTimer);
			camera.rotation_ = LerpVec(mainCameraR, subCameraR, cameraLerpTimer);
		}
		if (!isMainCamera) {
			camera.translation_ = LerpVec(subCameraP, mainCameraP, cameraLerpTimer);
			camera.rotation_ = LerpVec(subCameraR, mainCameraR, cameraLerpTimer);
		}
	}

	/*if (isMainCamera) {
	    camera.translation_ = mainCameraP;
	    camera.rotation_ = mainCameraR;
	} else {
	    camera.translation_ = subCameraP;
	    camera.rotation_ = subCameraR;
	}*/
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
		//	enemy_->Draw(camera);
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
