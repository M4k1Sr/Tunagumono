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
	ObjectBase(data)
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
		ColliderBase::TAG::BOX, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::BOX,
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
}

void ObjectTile::UpdateProcess(void)
{
}

void ObjectTile::UpdateProcessPost(void)
{
}

