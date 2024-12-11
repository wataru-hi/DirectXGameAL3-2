#pragma once

#include <KamataEngine.h>

using namespace KamataEngine;
using namespace MathUtility;
class Player;
class Enemy;

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
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	void Set(float* a, Vector3* b) { b->x = a[0], b->y = a[1], b->z = a[2]; }
	
	const float cameraLerpTime = 1.0f;
	float cameraLerpTimer;

	Vector3 LerpVec(const Vector3& v1, const Vector3& v2, float t) {
		return Vector3(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t));
	}

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// 
	uint32_t playerTextureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;
	Model* playerModel_ = nullptr;
	Model* enemyModel_ = nullptr;
	Camera camera;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	//std::list<std::shared_ptr<Enemy>> enemies_;
	DebugCamera* debugCamera_ = nullptr;

	Vector3 mainCameraP;
	Vector3 subCameraP;
	Vector3 mainCameraR;
	Vector3 subCameraR;
	bool isMainCamera = true;

	bool isDebugCameraActive_ = false;
};

