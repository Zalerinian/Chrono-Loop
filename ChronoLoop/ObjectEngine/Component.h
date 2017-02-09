#pragma once

enum ComponentType
{
	Unknown,
	Code,
	Collider,
	AudioEmitter,
	AudioListener,
	UI
};
class Component
{
private:
	bool isEnable;
	ComponentType type;
public:
	bool isEnabled() { return isEnable; };
	void Disable() { isEnable = false; };
	void Enable() { isEnable = true; };
	void Update();
	void Destroy() { delete this; };
};
