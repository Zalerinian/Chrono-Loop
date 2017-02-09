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
	BaseObject();
	~BaseObject();
	BaseObject(BaseObject const & _copy);
	BaseObject& operator=(const BaseObject);
	short& GetUniqueId();
	Transform& GetTransform();

};