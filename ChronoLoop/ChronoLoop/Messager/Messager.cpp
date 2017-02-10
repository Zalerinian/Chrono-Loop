#include "stdafx.h"
#include "Messager.h"

Messager* Messager::mMessenger = nullptr;
bool Messager::death = false;
std::queue<void*> Messager::msgQueue = std::queue<void*>();

template<class... Args>
Messager::Messager()
{
	Initialize();
}

Messager::~Messager()
{
	death = true;
}

Messager& Messager::Instance()
{
	if (!mMessenger)
		mMessenger = new Messager();

	return *mMessenger;
}
template<class... Args>
void Messager::Destroy()
{
	if (mMessenger)
		delete mMessenger;
}

template<class... Args>
void Messager::Initialize()
{
	if (!mMessenger)
		mMessenger = this;
	
	std::thread thrd(&Messager::Process<Args...>, this);
	thrd.detach();
}

void Messager::SendInMessage(void* _msg)
{
	msgQueue.push(_msg);
}
//?Need? just call the function needed?
template<class... Args>
void Messager::SendOutMessage(Message<Args...>* _msg)
{

}

template<class... Args>
void Messager::ProcessSound(Message<Args...>* _msg)
{
	switch (_msg->smType)
	{
	case INITAILIZE_Audio:
	{
		audio.Initialize();
	}
	break;
	case SHUTDOWN_Audio:
	{
		audio.Shutdown();
	}
	break;
	case UPDATE_Audio:
	{
		audio.Update();
	}
	break;
	case ADD_Listener:
	{
		//audio.AddListener(std::get<0>((*_msg).need), std::get<1>((*_msg).need));

		TupleSplitter::apply(&AudioWrapper::AddListener, _msg->need);
	}
	break;
	case REMOVE_Listener:
	{
		//TupleSplitter::apply(audio.RemoveListener, (*_msg).need);
	}
	break;
	case ADD_Emitter:
	{
		//TupleSplitter::apply(audio.AddEmitter, (*_msg).need);
	}
	break;
	case REMOVE_Emitter:
	{
		//TupleSplitter::apply(audio.RemoveEmitter, (*_msg).need);
	}
	break;
	case SET_BasePath:
	{
		//TupleSplitter::apply(audio.SetBasePath, (*_msg).need);
	}
	break;
	case ADD_Soundbank:
	{
		//TupleSplitter::apply(audio.LoadSoundBank, (*_msg).need);
	}
	break;
	case REMOVE_Soundbank:
	{
		//TupleSplitter::apply(audio.UnloadSoundBank, (*_msg).need);
	}
	break;
	}
}
template<class... Args>
void Messager::ProcessRender(Message<Args...>* _msg)
{

}
template<class... Args>
void Messager::ProcessPhysics(Message<Args...>* _msg)
{

}
template<class... Args>
void Messager::ProcessInput(Message<Args...>* _msg)
{

}

template<class... Args>
void Messager::ProcessMessage(Message<Args...>* _msg)
{
	switch (_msg->typ)
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
	while (!death)
	{
		if (msgQueue.empty())
			continue;

		Message<Args...> * getMsg = nullptr;

		void* temp = msgQueue.front();
		getMsg = (Message<Args...>*)(temp);
		ProcessMessage(getMsg);
		msgQueue.pop();
		delete getMsg;
	}
}