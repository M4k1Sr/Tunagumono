#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Actor/Stage.h"
#include "../Object/Actor/SkyDome.h"
#include "../Object/Actor/Charactor/Player.h"
#include "../Object/Actor/Charactor/SubPlayer.h"
#include "GameScene.h"

GameScene::GameScene(void)
	:
	stage_(nullptr),
	skyDome_(nullptr),
	player_(nullptr),
	subPlayer_(nullptr),
	SceneBase()
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	// ステージ
	stage_ = new Stage();
	stage_->Init();

	// プレイヤー
	player_ = new Player();
	player_->Init();

	// サブプレイヤー
	subPlayer_ = new SubPlayer();
	subPlayer_->Init();

	// スカイドーム
	skyDome_ = new SkyDome(player_->GetTransform());
	skyDome_->Init();

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(&player_->GetTransform());
	camera->ChangeMode(Camera::MODE::FOLLOW);

	// ステージモデルのコライダーをプレイヤーに登録
	const ColliderBase* stageCollider =
		stage_->GetOwnCollider(static_cast<int>(Stage::COLLIDER_TYPE::MODEL));
	player_->AddHitCollider(stageCollider);

	// カメラに登録
	camera->AddHitCollider(stageCollider);
}

void GameScene::Update(void)
{
	// ステージ更新
	stage_->Update();
	// プレイヤー更新
	player_->Update();
	// サブプレイヤー更新
	subPlayer_->Update();
	// スカイドーム更新
	skyDome_->Update();

	// シーン遷移
	auto const& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

}

void GameScene::Draw(void)
{
	// スカイドーム描画
	skyDome_->Draw();
	// ステージ描画
	stage_->Draw();
	// プレイヤー描画
	player_->Draw();
	// サブプレイヤー描画
	subPlayer_->Draw();

}

void GameScene::Release(void)
{
	// ステージ解放
	stage_->Release();
	delete stage_;

	// スカイドーム解放
	skyDome_->Release();
	delete skyDome_;

	// プレイヤー解放
	player_->Release();
	delete skyDome_;

	// サブプレイヤー解放
	subPlayer_->Release();
	delete subPlayer_;

}
