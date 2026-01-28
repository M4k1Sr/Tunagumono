#include <DxLib.h>
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Object/Common/AnimationController.h"
#include "../../../Object/Collider/ColliderLine.h"
#include "../../../Object/Collider/ColliderModel.h"
#include "../../../Object/Collider/ColliderCapsule.h"
#include "../../../Object/Actor/Charactor/Object/ObjectBase.h"
#include "../../../Object/Actor/Charactor/Object/ObjectTile.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Application.h"
#include "CharactorBase.h"

CharactorBase::CharactorBase(void)
	:
	animationController_(nullptr),
	moveDir_(AsoUtility::VECTOR_ZERO),
	moveSpeed_(0.0f),
	movePow_(AsoUtility::VECTOR_ZERO),
	isJump_(false),
	ActorBase()
{
}

CharactorBase::~CharactorBase(void)
{
}

void CharactorBase::Update(void)
{
	// 移動前座標を更新
	prevPos_ = transform_.pos;

	// 各キャラクターごとの更新処理
	UpdateProcess();

	// 移動方向に応じた遅延回転
	DelayRotate();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定前準備
	CollisionReserve();

	// 衝突判定
	Collision();

	// モデル制御更新
	transform_.Update();

	// アニメーション再生
	animationController_->Update();

	// 各キャラクターごとの更新後処理
	UpdateProcessPost();

}

void CharactorBase::Draw(void)
{
	// 基底クラスの描画処理
	ActorBase::Draw();

	// 丸影の描画
	DrawShadow();

}

void CharactorBase::Release(void)
{
	// アニメーション解放
	if (animationController_ != nullptr)
	{
		animationController_->Release();
		delete animationController_;
	}

	// 基底クラス解放
	ActorBase::Release();
}

void CharactorBase::InitLoad(void)
{
	// 丸影画像
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;
}

void CharactorBase::DelayRotate(void)
{
	// 移動方向から回転に変換する
	Quaternion goalRot = Quaternion::LookRotation(moveDir_);

	// 回転の補間
	transform_.quaRot =
		Quaternion::Slerp(transform_.quaRot, goalRot, 0.2f);

}

void CharactorBase::CalcGravityPow(void)
{
	// 重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Application::GetInstance().GetGravityPow() * scnMng_.GetDeltaTime();

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);

	jumpPow_ = VAdd(jumpPow_, gravity);

	// 重力速度の制限
	if (jumpPow_.y < MAX_FALL_SPEED)
	{
		jumpPow_.y = MAX_FALL_SPEED;
	}
}

void CharactorBase::Collision(void)
{
	// 移動処理
	transform_.pos = VAdd(transform_.pos, movePow_);

	// 衝突(カプセル)
	CollisionCapsule();

	// ジャンプ量を加算
	transform_.pos = VAdd(transform_.pos, jumpPow_);

	// 衝突(重力)
	CollisionGravity();

}

void CharactorBase::CollisionGravity(void)
{
	// 落下中しか判定しない
	if (!(VDot(AsoUtility::DIR_D, jumpPow_) > 0.9f))
	{
		return;
	}

	// 線分コライダ
	int lineType = static_cast<int>(COLLIDER_TYPE::LINE);

	// 線分コライダが無ければ処理を抜ける
	if (ownColliders_.count(lineType) == 0) return;

	// 線分コライダ情報
	ColliderLine* colliderLine_ =
		dynamic_cast<ColliderLine*>(ownColliders_.at(lineType));
	if (colliderLine_ == nullptr) return;

	bool isOnTile = false;
	VECTOR tileVelocity = AsoUtility::VECTOR_ZERO;

	// 登録されている衝突物を全てチェック
	for (const auto& hitCol : hitColliders_)
	{

		// ステージ以外は処理を飛ばす
		if (hitCol->GetTag() != ColliderBase::TAG::STAGE 
			&& hitCol->GetTag() != ColliderBase::TAG::BOX
			&& hitCol->GetTag() != ColliderBase::TAG::TILE) continue;

		// 派生クラスへキャスト
		const ColliderModel* colliderModel =
			dynamic_cast<const ColliderModel*>(hitCol);

		if (colliderModel == nullptr) continue;

		// 衝突したポリゴンの上に押し戻す
		bool isHit = colliderLine_->PushBackUp(
			colliderModel,
			transform_,
			2.0f,
			true,
			false);

		// 
		

		// ジャンプ判定
		if (isHit)
		{
			isJump_ = false;

			// 動く床の上に乗っているか
			if (hitCol->GetTag() == ColliderBase::TAG::TILE)
			{
				isOnTile = true;

				// とりあえず固定値で速度設定
				tileVelocity = VGet(0.0f, 0.5f, 0.0f);	// 毎フレーム0.5fフレーム上昇する床

			}
		}
	}

	if (!isJump_)
	{
		// ジャンプリセット
		jumpPow_ = AsoUtility::VECTOR_ZERO;
		// ジャンプの入力受付時間をリセット
		stepJump_ = 0.0f;

		// 動く床に乗っている場合は床の移動量を加算
		if (isOnTile)
		{
			transform_.pos = VAdd(transform_.pos, tileVelocity);
		}

	}

}

void CharactorBase::CollisionCapsule(void)
{
	// カプセルコライダ
	int capsuleType = static_cast<int>(COLLIDER_TYPE::CAPSULE);

	// カプセルコライダが無ければ処理を抜ける
	if (ownColliders_.count(capsuleType) == 0) return;

	// カプセルコライダ情報
	ColliderCapsule* colliderCapsule =
		dynamic_cast<ColliderCapsule*>(ownColliders_.at(capsuleType));
	if (colliderCapsule == nullptr) return;

	// 登録されている衝突物を全てチェック
	for (const auto& hitCol : hitColliders_)
	{
		
		// ステージは除外
		if (hitCol->GetTag() == ColliderBase::TAG::STAGE) continue;
		
		// モデル以外は処理を飛ばす
		if (hitCol->GetShape() != ColliderBase::SHAPE::MODEL) continue;

		// 派生クラスへキャスト
		const ColliderModel* colliderModel =
			dynamic_cast<const ColliderModel*>(hitCol);

		if (colliderModel == nullptr) continue;

		// 指定された回数と距離で三角形の法線方向に押し戻す
		colliderCapsule->PushBackAlongNormal(
			colliderModel,
			transform_,
			CNT_TRY_COLLISION,
			COLLISION_BACK_DIS,
			true,
			false
			);
	}
}

void CharactorBase::DrawShadow(void)
{
	float PLAYER_SHADOW_HEIGHT = 800.0f;
	float PLAYER_SHADOW_SIZE = 30.0f;

	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Zバッファは有効にする
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	// 影を落とすモデルを取得（ステージなど）
	for (const auto& hitCol : hitColliders_)
	{
		if (hitCol->GetTag() != ColliderBase::TAG::STAGE) continue;

		const ColliderModel* colliderModel =
			dynamic_cast<const ColliderModel*>(hitCol);

		if (colliderModel == nullptr) continue;

		int modelHandle = colliderModel->GetFollow()->modelId;

		// キャラクターの直下の地面ポリゴンを取得
		MV1_COLL_RESULT_POLY_DIM hitResDim = MV1CollCheck_Capsule(
			modelHandle, -1,
			transform_.pos,
			VAdd(transform_.pos, VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)),
			PLAYER_SHADOW_SIZE
		);

		// 頂点データの初期化
		VERTEX3D vertex[3];
		vertex[0].dif = GetColorU8(255, 255, 255, 255);
		vertex[0].spc = GetColorU8(0, 0, 0, 0);
		vertex[0].su = 0.0f;
		vertex[0].sv = 0.0f;
		vertex[1] = vertex[0];
		vertex[2] = vertex[0];

		// 検出したポリゴンに影を描画
		MV1_COLL_RESULT_POLY* hitRes = hitResDim.Dim;
		for (int i = 0; i < hitResDim.HitNum; i++, hitRes++)
		{
			// ポリゴンの座標設定
			vertex[0].pos = hitRes->Position[0];
			vertex[1].pos = hitRes->Position[1];
			vertex[2].pos = hitRes->Position[2];

			// 地面から少し浮かせる（Z-Fighting防止）
			VECTOR slideVec = VScale(hitRes->Normal, 0.5f);
			vertex[0].pos = VAdd(vertex[0].pos, slideVec);
			vertex[1].pos = VAdd(vertex[1].pos, slideVec);
			vertex[2].pos = VAdd(vertex[2].pos, slideVec);

			// ポリゴンの不透明度を設定する
			vertex[0].dif.a = 0;
			vertex[1].dif.a = 0;
			vertex[2].dif.a = 0;

			if (hitRes->Position[0].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				vertex[0].dif.a = 200 * (1.0f - fabs(hitRes->Position[0].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (hitRes->Position[1].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				vertex[1].dif.a = 200 * (1.0f - fabs(hitRes->Position[1].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (hitRes->Position[2].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				vertex[2].dif.a = 200 * (1.0f - fabs(hitRes->Position[2].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
			for (int j = 0; j < 3; j++)
			{
				vertex[j].u = (hitRes->Position[j].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
				vertex[j].v = (hitRes->Position[j].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			}

			// 影ポリゴンを描画
			DrawPolygon3D(vertex, 1, imgShadow_, TRUE);
		}

		// メモリ解放
		MV1CollResultPolyDimTerminate(hitResDim);
	}

	SetUseLighting(TRUE);
}