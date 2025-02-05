#include "GameScene.h"
#include <KamataEngine.h>
using namespace KamataEngine;
#include <map>

enum class SceneSelect {
	Title,
	Game,
	Clear,
	Over,
};

std::map<SceneSelect, SceneSelect> ChangeScene{
    {SceneSelect::Title, SceneSelect::Game },
    {SceneSelect::Clear, SceneSelect::Title},
    {SceneSelect::Over,  SceneSelect::Title},
};

std::map<bool, SceneSelect> SetScene{
    {false, SceneSelect::Clear},
    {true,  SceneSelect::Over },
};

void Drowback(DirectXCommon* dxCommon);

SceneSelect scene = SceneSelect::Title;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	GameScene* gameScene = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow();

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();

		if (scene == SceneSelect::Game) {
			gameScene->Update();
			if (gameScene->isEnd()) {
				scene = SetScene[gameScene->isDead()];
				gameScene->ClearScene();
			}
		} else {
			if (input->TriggerKey(DIK_SPACE)) {
				scene = ChangeScene[scene];
				gameScene->Initialize();
			}
		}

		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		if (scene == SceneSelect::Game) {
			gameScene->Draw();
		} else {
			Drowback(dxCommon);
		}

		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	delete gameScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}

void Drowback(DirectXCommon* dxCommon) {
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	uint32_t title = TextureManager::Load("Title.png");
	Sprite* titleSprite = Sprite::Create(title, {0, 0});
	uint32_t die = TextureManager::Load("Die.png");
	Sprite* dieSprite = Sprite::Create(die, {0,0});
	uint32_t clear = TextureManager::Load("clear.png");
	Sprite* clearSprite = Sprite::Create(clear, {0, 0});

	ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();

	Sprite::PreDraw(commandList);

	if (scene == SceneSelect::Title) {
		titleSprite->Draw();
	}
	if (scene == SceneSelect::Over) {
		dieSprite->Draw();
	}
	if (scene == SceneSelect::Clear) {
		clearSprite->Draw();
	}

	Sprite::PostDraw();
}
