#include "Messager.h"

Messager* Messager::mMessager = nullptr;
//std::priority_queue<Message*, std::vector<Message*>> Messager::msgQueue = std::priority_queue<Message*, std::vector<Message*>>();
std::queue<Message*> Messager::msgQueue = std::queue<Message*>();
bool Messager::death = false;
bool SendInProgress = false; //Send in progress
bool ProcessInProgress = false; //Process in progress

Messager::Messager()
{
	Initialize();
}
Messager::Messager(Messager& _newMsgr)
{
	audio = _newMsgr.audio;
	death = _newMsgr.death;
	/*if (mLocker.try_lock())
	{
		msgQueue = _newMsgr.msgQueue;
		mLocker.unlock();
	}*/
}
Messager::~Messager()
{
	death = true;
	thrd.join();
	audio.Shutdown();
	/*while (!msgQueue.empty())
	{
		msgQueue.pop();
	}*/
}

void Messager::Initialize()
{
	if (!mMessager)
		mMessager = this;

	thrd = std::thread(&Messager::Process, this);
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
	while (ProcessInProgress);
	SendInProgress = true;
	msgQueue.push(_msg);
	SendInProgress = false;
}

void Messager::Process()
{
	while (!death)
	{
		Message* tempMsg = nullptr;

		while (SendInProgress);
		ProcessInProgress = true;
		if (msgQueue.empty())
		{
			ProcessInProgress = false;
			continue;
		}
		tempMsg = msgQueue.front();
		msgQueue.pop();
		ProcessInProgress = false;
		ProcessMessage(tempMsg);
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
		m_Listener* lisn = (m_Listener*)(_msg->mNeed);
		audio.AddListener(lisn->mListener, lisn->mName);
	}
	break;
	case REMOVE_Listener:
	{
		Listener* mLis = (Listener*)(_msg->mNeed);
		audio.RemoveListener(mLis);
	}
	break;
	case ADD_Emitter:
	{
		m_Emitter* emit = (m_Emitter*)(_msg->mNeed);
		audio.AddEmitter(emit->mEmitter, emit->mName);
	}
	break;
	case REMOVE_Emitter:
	{
		Emitter* emit = (Emitter*)(_msg->mNeed);
		audio.RemoveEmitter(emit);
	}
	break;
	case MAKEEVENT_Event:
	{
		m_Event* evnt = (m_Event*)(_msg->mNeed);
		audio.MakeEvent(evnt->mID, evnt->mEmitter);
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
		m_EventListener * evnt = (m_EventListener*)(_msg->mNeed);
		audio.MakeEventAtListener(evnt->mID, evnt->mLID);
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
