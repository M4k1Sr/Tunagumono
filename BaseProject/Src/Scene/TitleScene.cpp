#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Object/Common/Transform.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Actor/SkyDome.h"
#include "../Application.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
	:
	imgTitle_(-1),
	imgPush_(-1),
	bigPlanet_(),
	rollPlanet_(),
	player_(),
	animationController_(nullptr),
	skyDome_(nullptr),
	SceneBase()
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	// 画像読み込み
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;

	imgPush_ = resMng_.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;

	// 定点カメラ
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// メイン惑星
	bigPlanet_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PIT_FALL_PLANET));
	bigPlanet_.scl = AsoUtility::VECTOR_ONE;
	bigPlanet_.quaRot = Quaternion::Identity();
	bigPlanet_.quaRotLocal = Quaternion::Identity();
	bigPlanet_.pos = AsoUtility::VECTOR_ZERO;
	bigPlanet_.Update();


	// 回転惑星
	rollPlanet_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::ROLL_PLANET));
	rollPlanet_.scl = VGet(ROLL_PLANET_SCALE, ROLL_PLANET_SCALE, ROLL_PLANET_SCALE);
	rollPlanet_.quaRot = Quaternion::Identity();
	rollPlanet_.quaRotLocal = Quaternion::Euler(ROLL_PLANET_ANGLE);
	rollPlanet_.pos = ROLL_PLANET_POS;
	rollPlanet_.Update();


	// ニンゲン
	player_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	player_.scl = VGet(PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE);
	player_.quaRot = Quaternion::Euler(PLAYER_ANGLE);
	player_.quaRotLocal = Quaternion::Euler(PLAYER_LOCAL_ANGLE);
	player_.pos = PLAYER_POS;
	player_.Update();

	// アニメーションコントローラー
	animationController_ = 
		new AnimationController(player_.modelId);
	animationController_->Add(0, 20.0f,Application::PATH_MODEL + "Player/Run.mv1");
	animationController_->Play(0, true);

	// スカイドーム
	skyDome_ = new SkyDome(empty_);
	skyDome_->Init();

}

void TitleScene::Update(void)
{

	// シーン遷移
	auto const& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	// Z軸回転
	//RollPlanet_.quaRot = Quaternion::Mult(
	//	RollPlanet_.quaRot,
	//	Quaternion::AngleAxis(AsoUtility::Deg2RadF(-1.0f),
	//	AsoUtility::AXIS_Z));

	rollPlanet_.quaRot = rollPlanet_.quaRot.Mult(
		Quaternion::Euler(0.0f,0.0f,AsoUtility::Deg2RadF(-1.0f)));

	// 回転惑星の更新
	rollPlanet_.Update();

	animationController_->Update();

	skyDome_->Update();
}

void TitleScene::Draw(void)
{
	// スカイドーム
	skyDome_->Draw();

	// モデル描画
	// 惑星モデル
	MV1DrawModel(bigPlanet_.modelId);
	MV1DrawModel(rollPlanet_. modelId);

	// ニンゲンモデル
	MV1DrawModel(player_.modelId);

	// タイトル描画
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, IMG_TITLE_POS_Y, 1.0f, 0.0f, imgTitle_, true);
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, IMG_PUSH_POS_Y, 1.0f, 0.0f, imgPush_, true);

}	

void TitleScene::Release(void)
{
	// アニメーションコントローラー解放
	animationController_->Release();
	delete animationController_;

	// スカイドーム解放
	skyDome_->Release();
	delete skyDome_;
}
