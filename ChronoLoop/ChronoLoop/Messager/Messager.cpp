#include "Messager.h"

Messager* Messager::mMessager = nullptr;
//std::priority_queue<Message*, std::vector<Message*>, cmp> Messager::msgQueue = std::priority_queue<Message*, std::vector<Message*>, cmp>();
std::queue<Message*> Messager::msgQueue = std::queue<Message*>();
bool Messager::death = false;

Messager::Messager()
{
	Initialize();
}
Messager::~Messager()
{
	death = true;
}

void Messager::Initialize()
{
	if (!mMessager)
		mMessager = this;

	std::thread thrd(&Messager::Process, this);
	thrd.detach();
}
Messager& Messager::Instance()
{
	if (!mMessager)
		mMessager = new Messager();

	return *mMessager;
}
void Messager::Destroy()
{
	if (mMessager)
		delete mMessager;
}


void Messager::SendInMessage(Message* _msg)
{
	msgQueue.push(_msg);
}
void Messager::Process()
{
	while (!death)
	{
		if (msgQueue.empty())
			continue;

		Message* tempMsg = msgQueue.front();
		ProcessMessage(tempMsg);
		msgQueue.pop();

		delete tempMsg;
	}
}

void Messager::ProcessMessage(Message* _msg)
{
	switch (_msg->mType)
	{
	case msgTypes::mSound:
	{
		ProcessSound(_msg);
	}
	break;
	case msgTypes::mRender:
	{
		ProcessRender(_msg);
	}
	break;
	case msgTypes::mInput:
	{
		ProcessInput(_msg);
	}
	break;
	case msgTypes::mPhysics:
	{
		ProcessPhysics(_msg);
	}
	break;
	}
}
void Messager::ProcessSound(Message* _msg)
{
	switch (_msg->smType)
	{
	case INITIALIZE_Audio:
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

	}
	break;
	case REMOVE_Listener:
	{

	}
	break;
	case ADD_Emitter:
	{

	}
	break;
	case REMOVE_Emitter:
	{

	}
	break;
	case MAKEEVENT_Event:
	{

	}
	break;
	case MAKEEVENT_Loc:
	{
		m_LocEvent* lEvent = (m_LocEvent*)(_msg->mNeed);
		audio.MakeEventAtLocation(lEvent->mID, lEvent->mPos);
	}
	break;
	case MAKEEVENT_Listener:
	{

	}
	break;
	case SET_BasePath:
	{
		m_Path* pth = (m_Path*)(_msg->mNeed);
		audio.SetBasePath(pth->mPath);
	}
	break;
	case ADD_Soundbank:
	{
		m_Path* pth = (m_Path*)(_msg->mNeed);
		audio.LoadSoundBank(pth->mPath);
	}
	break;
	case REMOVE_Soundbank:
	{
		m_Path* pth = (m_Path*)(_msg->mNeed);
		audio.UnloadSoundBank(pth->mPath);
	}
	break;
	}
}
void Messager::ProcessRender(Message* _msg)
{

}
void Messager::ProcessPhysics(Message* _msg)
{

}
void Messager::ProcessInput(Message* _msg)
{

}
