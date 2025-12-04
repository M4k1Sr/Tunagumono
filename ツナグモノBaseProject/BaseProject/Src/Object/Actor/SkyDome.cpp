#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "../../Object/Common/Transform.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Application.h"
#include "SkyDome.h"

SkyDome::SkyDome(const Transform& followTransform)
	:
	ActorBase(),
	followTransform_(followTransform),
	state_(STATE::NONE)
{
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Update(void)
{

	switch (state_)
	{
	case SkyDome::STATE::NONE:
		UpdateStateNone();
		break;
	case SkyDome::STATE::STAY:
		UpdateStateStay();
		break;
	case SkyDome::STATE::FOLLOW:
		UpdateStateFollow();
		break;
	}

}

void SkyDome::Draw(void)
{
	SetUseLighting(FALSE);
	MV1DrawModel(transform_.modelId);
	SetUseLighting(TRUE);
}

void SkyDome::Release(void)
{
}

void SkyDome::InitLoad(void)
{
	// ÉÇÉfÉãì«Ç›çûÇ›
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::SKY_DOME));

}

void SkyDome::InitTransform(void)
{
	transform_.scl = VGet(SKY_DOME_SCALE, SKY_DOME_SCALE, SKY_DOME_SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(SKY_DOME_LOCAL_ROT);
	transform_.pos = SKY_DOME_POS;
	transform_.Update();

}

void SkyDome::InitCollider(void)
{
}

void SkyDome::InitAnimation(void)
{
}

void SkyDome::InitPost(void)
{
	MV1SetUseZBuffer(transform_.modelId, false);
	MV1SetWriteZBuffer(transform_.modelId, false);

	SceneManager::SCENE_ID sceneId = scnMng_.GetSceneID();
	if (sceneId == SceneManager::SCENE_ID::GAME)
	{
		ChangeState(STATE::FOLLOW);
	}
	else
	{
		ChangeState(STATE::STAY);
	}
}

void SkyDome::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case SkyDome::STATE::NONE:
		ChangeStateNone();
		break;
	case SkyDome::STATE::STAY:
		ChangeStateStay();
		break;
	case SkyDome::STATE::FOLLOW:
		ChangeStateFollow();
		break;
	}
}

void SkyDome::ChangeStateNone(void)
{
}

void SkyDome::ChangeStateStay(void)
{
}

void SkyDome::ChangeStateFollow(void)
{
	// í«è]äJén
	transform_.pos = followTransform_.pos;
	transform_.Update();
}

void SkyDome::UpdateStateNone(void)
{
}

void SkyDome::UpdateStateStay(void)
{
	Quaternion rot = Quaternion::AngleAxis(
		AsoUtility::Deg2RadF(0.1f), AsoUtility::AXIS_Y);
	transform_.quaRot = transform_.quaRot.Mult(rot);
	transform_.Update();
}

void SkyDome::UpdateStateFollow(void)
{
	// Yé≤âÒì]
	Quaternion rot = Quaternion::AngleAxis(
		AsoUtility::Deg2RadF(0.1f), AsoUtility::AXIS_Y);
	transform_.quaRot = transform_.quaRot.Mult(rot);

	// í«è]
	transform_.pos = followTransform_.pos;

	transform_.Update();
}
