#pragma once
#include <vector>
#include "ObjectBase.h"
class ObjectBase;
class ColliderBase;

class ObjectManager
{
public:

	// コンストラクタ
	ObjectManager(void);

	// デストラクタ
	~ObjectManager(void);

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 解放
	void Release(void);

	// オブジェクト
	const std::vector<ObjectBase*>& GetObjects(void) const { return objects_; }

	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);

	// CSVからオブジェクト情報の読取を行う
	void LoadCsvData(void);

	// オブジェクト生成
	ObjectBase* Create(const ObjectBase::ObjectData& data);

private:

	// オブジェクト
	std::vector<ObjectBase*> objects_;

};