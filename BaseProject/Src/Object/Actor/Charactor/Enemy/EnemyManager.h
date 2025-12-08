#pragma once
#include <vector>
class EnemyBase;
class ColliderBase;

class EnemyManager
{
public:

	// コンストラクタ
	EnemyManager(void);

	// デストラクタ
	~EnemyManager(void);

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 解放
	void Release(void);

	// エネミー
	const std::vector<EnemyBase*>& GetEemies(void) const { return enemies_; }

	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);

private:

	// エネミー
	std::vector<EnemyBase*> enemies_;

};