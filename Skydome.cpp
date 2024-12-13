#include "Skydome.h"

void Skydome::Initialize(Model* model) {
	//assert(model);
	model_ = model;
	worldTransform_.Initialize();
}

void Skydome::Update() { worldTransform_.UpdateMatrix(); }

void Skydome::Draw(const Camera& camera) { model_->Draw(worldTransform_, camera); }