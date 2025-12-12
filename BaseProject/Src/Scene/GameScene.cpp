#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Actor/Stage.h"
#include "../Object/Actor/SkyDome.h"
#include "../Object/Actor/Charactor/Player.h"
#include "../Object/Actor/Charactor/Object/ObjectManager.h"
#include "GameScene.h"

GameScene::GameScene(void)
	:
	stage_(nullptr),
	skyDome_(nullptr),
	player_(nullptr),
	objMng_(nullptr),
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

	// オブジェクト初期化
	objMng_ = new ObjectManager();
	objMng_->Init();

	// ステージモデルのコライダーをプレイヤーに登録
	const ColliderBase* stageCollider =
		stage_->GetOwnCollider(static_cast<int>(Stage::COLLIDER_TYPE::MODEL));
	player_->AddHitCollider(stageCollider);

	// ステージモデルのコライダーをオブジェクトに登録
	objMng_->AddHitCollider(stageCollider);

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(&player_->GetTransform());
	camera->AddHitCollider(stageCollider);
	camera->ChangeMode(Camera::MODE::FOLLOW);

}

void GameScene::Update(void)
{
	// ステージ更新
	stage_->Update();

	// プレイヤー更新
	player_->Update();

	// スカイドーム更新
	skyDome_->Update();

	// オブジェクト更新
	objMng_->Update();

	// シーン遷移
	auto const& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_P))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::DEBUG);
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

	// オブジェクト描画
	objMng_->Draw();

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

	// オブジェクト解放
	objMng_->Release();
	delete objMng_;

}
