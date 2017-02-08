#pragma once
#include <string>
#include <list>
#include "Transform.h"

class BaseObject
{
private:
	std::string name;
	short id;
	Transform transform;
	std::list<BaseObject> children;
public:
	short GetUniqueId();
	Transform& GetTransform();

};