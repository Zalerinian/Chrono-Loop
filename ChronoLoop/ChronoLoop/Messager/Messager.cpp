#include "stdafx.h"
#include "Messager.h"
#include <thread>

template<class... Args>
Messager::Messager()
{
	std::thread thrd(&Messager::Process, this);
	thrd.detach();
}

Messager::~Messager()
{

}

template<class... Args>
void Messager::SendInMessage(Message<Args...> _msg)
{
	msgQueue.push_back(_msg);
}
//?Need? just call the function needed?
template<class... Args>
void Messager::SendOutMessage(Message<Args...> _msg)
{

}

template<class... Args>
void Messager::ProcessSound(Message<Args...> _msg)
{
	switch (_msg.smType)
	{

	}
}
template<class... Args>
void Messager::ProcessRender(Message<Args...> _msg)
{

}
template<class... Args>
void Messager::ProcessPhysics(Message<Args...> _msg)
{

}
template<class... Args>
void Messager::ProcessInput(Message<Args...> _msg)
{

}

template<class... Args>
void Messager::ProcessMessage(Message<Args...> _msg)
{
	switch (_msg.typ)
	{
	case messageTypes::SoundEngine:
	{
		ProcessSound(_msg);
	}
	break;
	case messageTypes::RenderEngine:
	{
		ProcessRender(_msg);
	}
	break;
	case messageTypes::Input:
	{
		ProcessInput(_msg);
	}
	break;
	case messageTypes::Physics:
	{
		ProcessPhysics(_msg);
	}
	break;
	}
}

template<class... Args>
void Messager::Process()
{
	//While alive, process messages in the queue
	while (true)
	{
		if (msgQueue.empty())
			continue;

		Message<Args...> * getMsg = nullptr;

		getMsg = msgQueue.top();
		ProcessMessage(getMsg);
		msgQueue.pop();
		delete getMsg;

	};
}