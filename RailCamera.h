#pragma once
#include"KamataEngine.h"
using namespace KamataEngine;
/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {
public:
/// <summary>
/// 初期化
/// </summary>
void Initialize(Vector3& pos, Vector3& rotate);

/// <summary>
/// 更新
/// </summary>
void Update();

/// <summary>
/// ワールド変換データを取得する
/// </summary>
/// <returns></returns>
const WorldTransform& GetWorldTransform() const { return worldTransform_; }

/// <summary>
/// 
/// </summary>
/// <returns></returns>
const Camera& GetCamera() { return camera_; }

private:
// ワールド変換データ
WorldTransform worldTransform_;
// カメラ
Camera camera_;

};