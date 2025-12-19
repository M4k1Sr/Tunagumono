#include "../../../../Utility/AsoUtility.h"
#include "ObjectBase.h"

ObjectBase::ObjectBase(const ObjectBase::ObjectData& data)
	:
	CharactorBase(),
	type_(data.type),
	stateBase_(0)
{
	// ‰ŠúÀ•W‚Ìİ’è
	transform_.pos = data.defaultPos;
}

ObjectBase::~ObjectBase(void)
{
}

void ObjectBase::ChangeState(int state)
{

	stateBase_ = state;

	// Šeó‘Ô‘JˆÚ‚Ì‰Šúˆ—
	stateChanges_[stateBase_]();

}

