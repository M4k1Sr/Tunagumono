#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Actor/Stage.h"
#include "../Object/Actor/SkyDome.h"
#include "../Object/Actor/Charactor/Player.h"
#include "GameScene.h"

GameScene::GameScene(void)
	:
	stage_(nullptr),
	skyDome_(nullptr),
	player_(nullptr),
	SceneBase()
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	// ステージ初期化
	stage_ = new Stage();
	stage_->Init();

	// プレイヤー初期化
	player_ = new Player();
	player_->Init();

	// スカイドーム初期化
	skyDome_ = new SkyDome(player_->GetTransform());
	skyDome_->Init();
	
	// ステージモデルのコライダーをプレイヤーに登録
	const ColliderBase* stageCollider =
		stage_->GetOwnCollider(static_cast<int>(Stage::COLLIDER_TYPE::MODEL));
	player_->AddHitCollider(stageCollider);

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(&player_->GetTransform());
	camera->AddHitCollider(stageCollider);
	camera->ChangeMode(Camera::MODE::FOLLOW);

}

void GameScene::Update(void)
{
	stage_->Update();

	player_->Update();

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

	skyDome_->Draw();

	stage_->Draw();

	player_->Draw();

}

void GameScene::Release(void)
{
	stage_->Release();
	delete stage_;

	skyDome_->Release();
	delete skyDome_;

	player_->Release();
	delete skyDome_;

}
