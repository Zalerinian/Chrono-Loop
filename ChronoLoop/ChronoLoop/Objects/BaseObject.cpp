#include "stdafx.h"
#include "BaseObject.h"

short BaseObject::GetUniqueId()
{
	return id;
}

Transform & BaseObject::GetTransform()
{
	return transform;
}
