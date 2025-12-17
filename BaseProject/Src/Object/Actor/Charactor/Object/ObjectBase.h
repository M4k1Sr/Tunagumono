#pragma once
#include <functional>
#include "../CharactorBase.h"

class ObjectBase : public CharactorBase
{
public:

	// 種別
	enum class TYPE
	{
		BOX,
		TILE,
	};

	// オブジェクトデータ
	struct ObjectData
	{
		int id;
		ObjectBase::TYPE type;
		VECTOR defaultPos;
	};

	// コンストラクタ
	ObjectBase(const ObjectBase::ObjectData& data);

	// デストラクタ
	virtual ~ObjectBase(void) override;

protected:

	// 状態管理
	int stateBase_;

	// 状態管理(状態遷移時初期処理)
	std::map<int, std::function<void(void)>> stateChanges_;

	// 状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;

	// リソースロード
	void InitLoad(void) override {}

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override {}

	// 衝突判定の初期化
	void InitCollider(void) override {}

	// アニメーションの初期化
	void InitAnimation(void) override {}

	// 初期化後の個別処理
	void InitPost(void) override {}

	// 状態遷移
	void ChangeState(int state);

	// 種別
	TYPE type_;

};
