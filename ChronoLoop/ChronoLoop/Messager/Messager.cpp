#include "Messager.h"

Messager* Messager::mMessager = nullptr;
//std::priority_queue<Message*, std::vector<Message*>> Messager::msgQueue = std::priority_queue<Message*, std::vector<Message*>>();
//std::queue<Message*> Messager::msgQueue = std::queue<Message*>();
//bool Messager::death = false;
bool SendInProgress = false; //Send in progress
bool ProcessInProgress = false; //Process in progress

Messager::Messager()
{
	Initialize();
}
Messager::~Messager()
{
	death = true;
	while (!mLock.try_lock());
	mLock.unlock();
	thrd.join();
	
	AudioWrapper::Destroy();
}

void Messager::Initialize()
{
	if (!mMessager)
		mMessager = this;

	death = false;
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
	mLock.lock();
	msgQueue.push(_msg);
	mLock.unlock();
}

void Messager::Process()
{
	while (!death)
	{
		Message* tempMsg = nullptr;

		if (!mLock.try_lock())
			continue;
		if (msgQueue.empty())
		{
			mLock.unlock();
			continue;
		}

		tempMsg = msgQueue.front();
		msgQueue.pop();
		mLock.unlock();
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
	}
}

void Messager::ProcessSound(Message* _msg)
{
	switch (_msg->smType)
	{
	case INITIALIZE_Audio:
	{
		AudioWrapper::GetInstance().Initialize();
	}
	break;
	case SHUTDOWN_Audio:
	{
		AudioWrapper::GetInstance().Shutdown();
	}
	break;
	case UPDATE_Audio:
	{
		if (AudioWrapper::GetInstance().IsInitialized())
			AudioWrapper::GetInstance().Update();
	}
	break;
	case ADD_Listener:
	{
		m_Listener* lisn = (m_Listener*)(_msg->mNeed);
		AudioWrapper::GetInstance().AddListener(lisn->mListener, lisn->mName);
	}
	break;
	case REMOVE_Listener:
	{
		Listener* mLis = (Listener*)(_msg->mNeed);
		AudioWrapper::GetInstance().RemoveListener(mLis);
	}
	break;
	case ADD_Emitter:
	{
		m_Emitter* emit = (m_Emitter*)(_msg->mNeed);
		AudioWrapper::GetInstance().AddEmitter(emit->mEmitter, emit->mName);
	}
	break;
	case REMOVE_Emitter:
	{
		Emitter* emit = (Emitter*)(_msg->mNeed);
		AudioWrapper::GetInstance().RemoveEmitter(emit);
	}
	break;
	case MAKEEVENT_Event:
	{
		m_Event* evnt = (m_Event*)(_msg->mNeed);
		AudioWrapper::GetInstance().MakeEvent(evnt->mID, evnt->mEmitter);
	}
	break;
	case MAKEEVENT_Loc:
	{
		m_LocEvent* lEvent = (m_LocEvent*)(_msg->mNeed);
		AudioWrapper::GetInstance().MakeEventAtLocation(lEvent->mID, lEvent->mPos);
	}
	break;
	case MAKEEVENT_Listener:
	{
		m_EventListener * evnt = (m_EventListener*)(_msg->mNeed);
		AudioWrapper::GetInstance().MakeEventAtListener(evnt->mID, evnt->mLID);
	}
	break;
	case SET_BasePath:
	{
		m_Path* pth = (m_Path*)(_msg->mNeed);
		AudioWrapper::GetInstance().SetBasePath(pth->mPath);
	}
	break;
	case ADD_Soundbank:
	{
		m_Path* pth = (m_Path*)(_msg->mNeed);
		AudioWrapper::GetInstance().LoadSoundBank(pth->mPath);
	}
	break;
	case REMOVE_Soundbank:
	{
		m_Path* pth = (m_Path*)(_msg->mNeed);
		AudioWrapper::GetInstance().UnloadSoundBank(pth->mPath);
	}
	break;
	}
}
