#include "KamataEngine.h"
#include "Affin.h"

using namespace KamataEngine;

void WorldTransform::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = MakeAffinMatrix(scale_, rotation_, translation_);

	// 定数バッファに転送する
	TransferMatrix();
}