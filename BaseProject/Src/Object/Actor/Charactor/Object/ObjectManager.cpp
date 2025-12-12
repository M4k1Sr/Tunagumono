#include <string>
#include <fstream>
#include "../../../../Application.h"
#include "../../../../Utility/AsoUtility.h"
#include "./ObjectBase.h"
#include "./ObjectBox.h"
#include "./ObjectManager.h"

ObjectManager::ObjectManager(void)
{
}

ObjectManager::~ObjectManager(void)
{
}

void ObjectManager::Init(void)
{
	// オブジェクトデータ読み込み
	LoadCsvData();

}

void ObjectManager::Update(void)
{
	// 更新
	for (auto& object : objects_)
	{
		object->Update();
	}
}

void ObjectManager::Draw(void)
{
	// 描画
	for (auto& object : objects_)
	{
		object->Draw();
	}

}

void ObjectManager::Release(void)
{
	// 解放
	for (auto& object : objects_)
	{
		object->Release();
		delete object;
	}

}

void ObjectManager::AddHitCollider(const ColliderBase* hitCollider)
{
	for (auto& object : objects_)
	{
		object->AddHitCollider(hitCollider);
	}
}

void ObjectManager::LoadCsvData(void)
{
	// ファイルの読込
	std::ifstream ifs = std::ifstream(Application::PATH_CSV + "ObjectData.csv");

	if (!ifs)
	{
		// エラーが発生
		return;
	}

	// ファイルを１行ずつ読み込む
	std::string line;// 1行の文字情報

	std::vector<std::string> strSplit; // 1行を1文字の動的配列に分割

	bool isHeader = true;

	while (getline(ifs, line))
	{
		if (isHeader)
		{
			isHeader = false;
			continue;
		}

		// １行をカンマ区切りで分割
		strSplit = AsoUtility::Split(line, ',');

		ObjectBase* enemy = nullptr;

		// 構造体に合わせて読込データを格納
		ObjectBase::ObjectData data = ObjectBase::ObjectData();

		int idx = 0;

		// ID
		data.id = stoi(strSplit[idx++]);

		// 種別
		data.type = static_cast<ObjectBase::TYPE>(stoi(strSplit[idx++]));

		// 初期座標
		data.defaultPos =
		{
		stof(strSplit[idx++]),
		stof(strSplit[idx++]),
		stof(strSplit[idx++])
		};

		// オブジェクト生成
		Create(data);

	}

	ifs.close();
}

ObjectBase* ObjectManager::Create(const ObjectBase::ObjectData& data)
{
	ObjectBase* object = nullptr;

	switch (data.type)
	{
	case ObjectBase::TYPE::BOX:
		object = new ObjectBox(data);
		break;
		// 増える毎に追加
	}

	if (object != nullptr)
	{
		object->Init();
		objects_.emplace_back(object);
	}

	return object;
}

