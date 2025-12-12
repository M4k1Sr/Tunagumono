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

	// 主に壁や木などの衝突で仕様するカプセルコライダ
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

	// 状態遷移初期処理登録
	stateChanges_.emplace(static_cast<int>(STATE::NONE),
		std::bind(&ObjectBox::ChangeStateNone, this));

	stateChanges_.emplace(static_cast<int>(STATE::END),
		std::bind(&ObjectBox::ChangeStateEnd, this));

	// 初期状態設定
	ChangeState(STATE::NONE);
}

void ObjectBox::UpdateProcess(void)
{

	// 状態別更新
	stateUpdate_();



}

void ObjectBox::UpdateProcessPost(void)
{
}

void ObjectBox::ChangeState(STATE state)
{
	state_ = state;
	// 各状態の初期状態設定
	ObjectBase::ChangeState(static_cast<int>(state_));


	//state_ = state;
	//switch (state)
	//{
	//case EnemyRat::STATE::NONE:
	//	ChangeStateNone();
	//	break;
	//case EnemyRat::STATE::THINK:
	//	ChangeStateThink();
	//	break;
	//case EnemyRat::STATE::IDLE:
	//	ChangeStateIdle();
	//	break;
	//case EnemyRat::STATE::WANDER:
	//	ChangeStateWander();
	//	break;
	//case EnemyRat::STATE::END:
	//	ChangeStateEnd();
	//	break;
	//}

}

void ObjectBox::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&ObjectBox::UpdateNone, this);
}

void ObjectBox::ChangeStateThink(void)
{
	//stateUpdate_ = std::bind(&ObjectBox::UpdateThink, this);

	//// 思考
	//// ランダムに次の行動を決定
	//// 30%で待機、70%で徘徊
	//int rand = GetRand(100);

	//if (rand < 30)
	//{
	//	ChangeState(STATE::IDLE);
	//}
	//else
	//{
	//	ChangeState(STATE::WANDER);
	//}
}

void ObjectBox::ChangeStateIdle(void)
{
	stateUpdate_ = std::bind(&ObjectBox::UpdateIdle, this);

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));

	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;

}

void ObjectBox::ChangeStateWander(void)
{
	stateUpdate_ = std::bind(&ObjectBox::UpdateWander, this);

	// ランダムな角度
	float angle = static_cast<float>(GetRand(360)) * DX_PI_F / 180.0f;

	// 移動方向
	moveDir_ = VGet(cosf(angle), 0.0f, sinf(angle));

	// ランダムな移動時間
	step_ = 2.0f + static_cast<float>(GetRand(5));

	// 移動スピード
	moveSpeed_ = 3.0f;


}

void ObjectBox::ChangeStateEnd(void)
{
	stateUpdate_ = std::bind(&ObjectBox::UpdateEnd, this);
}

void ObjectBox::UpdateNone(void)
{
}

void ObjectBox::UpdateThink(void)
{
}

void ObjectBox::UpdateIdle(void)
{

	//step_ -= SceneManager::GetInstance().GetDeltaTime(); // または -= 1.0f;

	//if (step_ <= 0.0f)
	//{
	//	ChangeState(STATE::THINK);
	//}
}

void ObjectBox::UpdateWander(void)
{
	//// 毎フレーム移動
	//movePow_ = VScale(moveDir_, moveSpeed_);
	//transform_.pos = VAdd(transform_.pos, movePow_);

	//// 時間経過チェック
	//step_ -= SceneManager::GetInstance().GetDeltaTime(); // または -= 1.0f;
	//if (step_ <= 0.0f)
	//{
	//	ChangeState(STATE::THINK);
	//}
}

void ObjectBox::UpdateEnd(void)
{
}
