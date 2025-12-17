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
#include "ObjectBox.h"

ObjectBox::ObjectBox(const ObjectBase::ObjectData& data)
	:
	ObjectBase(data)
{
}

ObjectBox::~ObjectBox(void)
{
}

void ObjectBox::InitLoad(void)
{
	// 基底クラスのリソースロード
	ObjectBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::OBJECT_BOX));

}

void ObjectBox::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE, SCALE, SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void ObjectBox::InitCollider(void)
{

	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::BOX, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// 主に壁や木などの衝突で使用するカプセルコライダ
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::BOX, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);

}

void ObjectBox::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(transform_.modelId);

}

void ObjectBox::InitPost(void)
{
}

void ObjectBox::UpdateProcess(void)
{
}

void ObjectBox::UpdateProcessPost(void)
{
}

