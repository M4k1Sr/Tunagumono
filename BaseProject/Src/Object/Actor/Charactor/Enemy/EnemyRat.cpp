#include <DxLib.h>
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource.h"
#include "../../../../Object/Common/Transform.h"
#include "../../../Collider/ColliderLine.h"
#include "../../../Collider/ColliderCapsule.h"
#include "../../../../Object/Common/AnimationController.h"
#include "../../../../Application.h"
#include "EnemyRat.h"

EnemyRat::EnemyRat(void)
{
}

EnemyRat::~EnemyRat(void)
{
}

void EnemyRat::InitLoad(void)
{
	// モデル読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_RAT));

}

void EnemyRat::InitTransform(void)
{
	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE, SCALE, SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.pos = VGet(0.0f,100.0f,1500.0f);
	transform_.Update();
}

void EnemyRat::InitCollider(void)
{
	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::RAT, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// 主に壁や木などの衝突で仕様するカプセルコライダ
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::RAT, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);

}

void EnemyRat::InitAnimation(void)
{
	// アニメーションコントローラー
	animationController_ =
		new AnimationController(transform_.modelId);

	// アニメーション追加
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 20.0f, Application::PATH_MODEL + "Player/Idle.mv1");

	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);

}

void EnemyRat::InitPost(void)
{
}

void EnemyRat::UpdateProcess(void)
{
}

void EnemyRat::UpdateProcessPost(void)
{
}
