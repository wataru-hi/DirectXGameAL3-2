#pragma once
#include "KamataEngine.h"
#include "Affin.h"

using namespace KamataEngine;

class Skydome {
public:
    void Initialize(Model* model);

    void Update();

    void Draw(const Camera& viewProjection);

private:
    Model* model_ = nullptr;
    WorldTransform worldTransform_;
};