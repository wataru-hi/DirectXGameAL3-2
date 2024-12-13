#pragma once

#include "KamataEngine.h"

using namespace KamataEngine;
class Player;
class Enemy;
class Skydome;
class RailCamera;
class PlayerBulllet;
class EnemyBullet;


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

	void AddPlayerBullet(std::shared_ptr<PlayerBulllet> playerBullet) { playerbullets_.push_back(playerBullet); }
	void AddEnemyBullet(std::shared_ptr<EnemyBullet> enemyBullet) { enemybullets_.push_back(enemyBullet); }

private:
	bool IsCollisionSphereAndSphere(const Vector3& posA, float radiusA, const Vector3& posB, float radiusB);
	void CheakAllCollisions();

	void BulletUpdate();
	void BulletDrow();

	void LoadEnemyPopDate();

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
	RailCamera* railCamera = nullptr;
	DebugCamera* debugCamera_ = nullptr;

	std::list<std::shared_ptr<PlayerBulllet>>playerbullets_;
	std::list<std::shared_ptr<EnemyBullet>>enemybullets_;

	bool isDebugCameraActive_ = false;
};

