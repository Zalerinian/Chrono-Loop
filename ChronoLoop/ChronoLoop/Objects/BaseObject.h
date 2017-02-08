#pragma once
#include <string>
#include <list>
#include "Transform.h"

class Componet;

class BaseObject
{
private:
	std::string name;
	short id;
	Transform transform;
	std::list<BaseObject> children;
	std::list<Componet> mComponets;
public:
	short GetUniqueId();
	Transform& GetTransform();

};