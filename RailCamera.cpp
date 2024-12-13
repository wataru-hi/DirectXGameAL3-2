#include "RailCamera.h"

using namespace KamataEngine;
using namespace MathUtility;


void RailCamera::Initialize(Vector3& pos, Vector3& rotate) {
// ワールドトランスフォームの初期設定
worldTransform_.Initialize();
worldTransform_.translation_ = pos;
worldTransform_.rotation_ = rotate;

camera_.farZ = 1000.0f;
camera_.Initialize();
}

void RailCamera::Update() {

// ワールドトランスフォームの座標の数値を加算したりする(移動)
worldTransform_.translation_ += Vector3(0, 0, 0.1f);
// ワールドトランスフォームの座標の数値を加算したりする(移動)
// 
// カメラオブジェクトのワールド行列からビュー行列を計算する
camera_.matView = Inverse(worldTransform_.matWorld_);

#ifdef _DEBUG
ImGui::Begin("camera");
ImGui::SliderFloat3("camera.translation_", &worldTransform_.translation_.x, -10.0f, 10.0f);
ImGui::SliderFloat3("camera.rotation_", &worldTransform_.rotation_.x, -10.0f, 10.0f);
ImGui::End();
#endif // DEBUG

}