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
	bool isEnabled;
	ComponentType type;
public:
	bool IsEnabled() { return isEnabled; };
	void Disable() { isEnabled = false; };
	void Enable() { isEnabled = true; };
	void Update();
	void Destroy() { delete this; };
};
