#include "../../../../Application.h"
#include "./EnemyBase.h"
#include "./EnemyRat.h"
#include "./EnemyManager.h"

EnemyManager::EnemyManager(void)
{
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Init(void)
{
	// エネミー生成
	EnemyRat* enemyRat = new EnemyRat();
	enemies_.emplace_back(enemyRat);
}

void EnemyManager::Update(void)
{
	//↑より効率がいい書き方
	for (const auto enemy : enemies_)
	{
		enemy->Update();
	}
}

void EnemyManager::Draw(void)
{
	for (const auto enemy : enemies_)
	{
		enemy->Draw();
	}

}

void EnemyManager::Release(void)
{
	for (const auto enemy : enemies_)
	{
		enemy->Release();
	}

}

void EnemyManager::AddHitCollider(const ColliderBase* hitCollider)
{
	for (auto& enemy : enemies_)
	{
		enemy->AddHitCollider(hitCollider);
	}
}

