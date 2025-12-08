#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class AnimationController;
class SkyDome;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

private:

	// アニメーション処理
	AnimationController* animationController_;

	// 惑星
	Transform bigPlanet_;

	// 回転惑星
	Transform rollPlanet_;

	// ニンゲン
	Transform player_;

	// スカイドーム
	SkyDome* skyDome_;

	// スカイドーム用の空Transform
	Transform empty_;

	// タイトル画像のY座標
	static constexpr int IMG_TITLE_POS_Y = 250;
	// PushSpace画像のY座標
	static constexpr int IMG_PUSH_POS_Y = 500;

	// 回転惑星のスケール
	static constexpr float ROLL_PLANET_SCALE = 0.7f;
	// 回転惑星の座標
	static constexpr VECTOR ROLL_PLANET_POS = { -250.0f, -100.0f, -100.0f };
	// 回転惑星の角度
	static constexpr VECTOR ROLL_PLANET_ANGLE = { 90.0f * DX_PI_F / 180.0f, 0.0f, 0.0f};


	// ニンゲンのスケール
	static constexpr float PLAYER_SCALE = 0.4f;
	// ニンゲン座標
	static constexpr VECTOR PLAYER_POS = { -250.0f, -32.0f, -105.0f };
	// ニンゲンの角度
	static constexpr VECTOR PLAYER_ANGLE = {0.0f,  -90.0f * DX_PI_F / 180.0f, 0.0f };
	static constexpr VECTOR PLAYER_LOCAL_ANGLE = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// タイトル画像
	int imgTitle_;

	// プッシュ画像
	int imgPush_;
};
