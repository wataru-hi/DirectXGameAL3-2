#pragma once

#include "KamataEngine.h"

using namespace KamataEngine;
class Player;
class Enemy;
class Skydome;

class GameScene
{
	public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	bool IsCollisionSphereAndSphere(const Vector3& posA, float radiusA, const Vector3& posB, float radiusB);
	void CheakAllCollisions();

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// 
	uint32_t playerTextureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;

	Model* playerModel_ = nullptr;
	Model* enemyModel_ = nullptr;
    Model* skydomeModel_ = nullptr;
	
	Camera camera;
	
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	
	Skydome* skydome_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;

	bool isDebugCameraActive_ = false;
};

