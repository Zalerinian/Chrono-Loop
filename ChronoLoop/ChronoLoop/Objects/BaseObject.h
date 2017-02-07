#pragma once
#include <string>
#include <list>
#include "Transform.h"

class BaseObject
{
private:
	std::string name;
	//uint32 id;
	Transform transform;
	std::list<BaseObject> children;
public:

};