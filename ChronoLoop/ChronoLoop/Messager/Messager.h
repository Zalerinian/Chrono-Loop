#pragma once
#include "..\..\SoundEngine\SoundEngine.h"

template<class... Args>
struct Message;

class Messager
{
	static Messager* mMessenger;

private:

	//AudioWrapper audio;




public:
	

	Messager();
	~Messager();

	Messager* GetMessenger() { return mMessenger; }

	void SendInMessage();
	void SendOutMessage();

	void Process();

private:
	template<class... Args>
	void ProcessMessage(Message<Args...> _msg);
};