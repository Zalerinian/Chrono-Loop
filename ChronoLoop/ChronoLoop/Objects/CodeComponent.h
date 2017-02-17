#pragma once
#include "Component.h"

struct Action;
class CodeComponent : public Component {
	Action* mAction = nullptr;
public:
	CodeComponent(Action* _action);
	void Update();
	void Destroy();
};