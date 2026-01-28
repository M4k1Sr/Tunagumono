#include <DxLib.h>
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource.h"
#include "../../../../Object/Common/Transform.h"
#include "../../../Collider/ColliderLine.h"
#include "../../../Collider/ColliderCapsule.h"
#include "../../../Collider/ColliderModel.h"
#include "../../../../Object/Common/AnimationController.h"
#include "../../../../Application.h"
#include"./ObjectBase.h"
#include "ObjectTile.h"

ObjectTile::ObjectTile(const ObjectBase::ObjectData& data)
	:
	ObjectBase(data),
	prevPos_(AsoUtility::VECTOR_ZERO),
	velocity_(AsoUtility::VECTOR_ZERO)
{
}

ObjectTile::~ObjectTile(void)
{
}

void ObjectTile::InitLoad(void)
{
	// 基底クラスのリソースロード
	ObjectBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::OBJECT_TILE));

}

void ObjectTile::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE, SCALE*2, SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void ObjectTile::InitCollider(void)
{

	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::TILE, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::TILE,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}

void ObjectTile::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(transform_.modelId);

}

void ObjectTile::InitPost(void)
{	
	
	// 基底クラスの初期化後処理
	prevPos_ = transform_.pos;
	velocity_ = AsoUtility::VECTOR_ZERO;


	// 初期遷移状態初期処理登録
	stateChanges_.emplace(static_cast<int>(STATE::NONE),
		std::bind(&ObjectTile::ChangeStateNone, this));

	stateChanges_.emplace(static_cast<int>(STATE::STOP),
		std::bind(&ObjectTile::ChangeStateStop, this));

	stateChanges_.emplace(static_cast<int>(STATE::UP),
		std::bind(&ObjectTile::ChangeStateUp, this));

	stateChanges_.emplace(static_cast<int>(STATE::DOWN),
		std::bind(&ObjectTile::ChangeStateDown, this));

	stateChanges_.emplace(static_cast<int>(STATE::END),
		std::bind(&ObjectTile::ChangeStateEnd, this));

	// 初期状態設定
	ChangeState(STATE::UP);

}

void ObjectTile::UpdateProcess(void)
{

	// 状態別更新
	stateUpdate_();

	
}

void ObjectTile::UpdateProcessPost(void)
{
	transform_.Update();

	ObjectBase::UpdateProcessPost();
}

void ObjectTile::ChangeState(STATE state)
{

	state_ = state;

	// 各状態の初期状態設定
	ObjectBase::ChangeState(static_cast<int>(state_));

}

void ObjectTile::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&ObjectTile::UpdateNone, this);

}

void ObjectTile::ChangeStateStop(void)
{
	stateUpdate_ = std::bind(&ObjectTile::UpdateStop, this);

}

void ObjectTile::ChangeStateUp(void)
{
	
	// 経過時間
	moveTimer_ = 0.0f;
	// 移動時間
	moveTime_ = MOVE_TIME;

	// 初期位置
	startPos_ = transform_.pos;
	// 移動する場所
	movePlacePos_ = VAdd(startPos_, VScale(AsoUtility::DIR_U, MOVE_UP_TILE));

	stateUpdate_ = std::bind(&ObjectTile::UpdateUp, this);

}

void ObjectTile::ChangeStateDown(void)
{
	// 経過時間
	moveTimer_ = 0.0f;
	// 移動時間
	moveTime_ = MOVE_TIME;

	// 初期位置
	startPos_ = transform_.pos;
	// 移動する場所
	movePlacePos_ = VAdd(startPos_, VScale(AsoUtility::DIR_D, MOVE_UP_TILE));

	stateUpdate_ = std::bind(&ObjectTile::UpdateDown, this);
}

void ObjectTile::ChangeStateEnd(void)
{
	stateUpdate_ = std::bind(&ObjectTile::UpdateEnd, this);
}

void ObjectTile::UpdateNone(void)
{
}

void ObjectTile::UpdateStop(void)
{
}

void ObjectTile::UpdateUp(void)
{

	UpdateProcessFloorMove();
	if (moveTimer_ >= moveTime_)
	{
		ChangeState(STATE::DOWN);
	}

}

void ObjectTile::UpdateDown(void)
{
	UpdateProcessFloorMove();
	if (moveTimer_ >= moveTime_)
	{
		ChangeState(STATE::UP);
	}

}

void ObjectTile::UpdateEnd(void)
{
}

void ObjectTile::UpdateProcessFloorMove(void)
{
	// 経過時間取得(デルタタイム)
	moveTimer_ += SceneManager::GetInstance().GetDeltaTime();
	// 線形補間用ステップ計算
	float t = moveTimer_ / moveTime_;

	// 経過時間により移動完了
	if (moveTimer_ > moveTime_)
	{
		transform_.pos = movePlacePos_;
		return;
	}

	// 線形補間で移動(これでオーバーせずにピタッと止まる)
	transform_.pos = AsoUtility::Lerp(startPos_, movePlacePos_, t);
	transform_.Update();

}
