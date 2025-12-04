#pragma once
#include <DxLib.h>
#include "ActorBase.h"

class SkyDome : public ActorBase
{
public:

	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW,
	};
	// コンストラクタ
	SkyDome(const Transform& followTransform);
	// デストラクタ
	~SkyDome(void) override;

	// 更新
	void Update(void) override;
	// 描画
	void Draw(void) override;
	// 解放
	void Release(void) override;

protected:

	// リソースロード
	void InitLoad(void) override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;

	// 衝突判定の初期化
	void InitCollider(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override;

	// 初期化後の個別処理
	void InitPost(void) override;

private:

	// 追従対象のTranceform
	const Transform& followTransform_;

	STATE state_;

	// スカイドームの座標
	static constexpr VECTOR SKY_DOME_POS = { 0.0f, 0.0f, 0.0f };
	// スカイドームのスケール
	static constexpr float SKY_DOME_SCALE = 100.0f;
	// スカイドームのローカル角度
	static constexpr VECTOR SKY_DOME_LOCAL_ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// 状態遷移
	void ChangeState(STATE state);

	void ChangeStateNone(void);
	void ChangeStateStay(void);
	void ChangeStateFollow(void);

	void UpdateStateNone(void);
	void UpdateStateStay(void);
	void UpdateStateFollow(void);





};

