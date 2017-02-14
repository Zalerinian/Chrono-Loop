#pragma once
#include "..\Sound\SoundEngine.h"
#include "..\Sound\Sound\Soundbanks\Wwise_IDs.h"
#include <queue>
#include <vector>
#include <thread>
#include <cstdarg>

enum msgTypes {NONE = -1, mSound, mRender, mInput, mPhysics};

enum soundMsg {INITIALIZE_Audio, SHUTDOWN_Audio, UPDATE_Audio, ADD_Listener, REMOVE_Listener,
				ADD_Emitter, REMOVE_Emitter, MAKEEVENT_Loc, MAKEEVENT_Listener, MAKEEVENT_Event,
				SET_BasePath, ADD_Soundbank, REMOVE_Soundbank};
enum renderMsg {};
enum inputMsg {};
enum physicsMsg {};

struct Message
{
public:
	long mAge;
	bool mIsPriority;

	msgTypes mType;
	union
	{
		int num;
		soundMsg smType;
		renderMsg rmTypes;
		inputMsg imTypes;
		physicsMsg pmTypes;
	};

	//Pointer to the struct *ugh*
	void * mNeed;

	Message()
	{
		mNeed = nullptr;
	}
	Message(Message& _msg)
	{

	}
	Message(msgTypes _type, int _secondtype, long _age, bool _priority, void* _shit = nullptr)
	{
		mType = _type;
		num = _secondtype;
		mAge = _age;
		mIsPriority = _priority;
		mNeed = _shit;
	}

#pragma region Operator Overrides

	//Is older
	bool operator<(Message& _other)
	{
		return mAge < _other.mAge;
	}
	bool operator<(const Message& _other)
	{
		return mAge < _other.mAge;
	}
	bool operator<=(Message& _other)
	{
		return mAge <= _other.mAge;
	}
	bool operator<=(const Message& _other)
	{
		return mAge <= _other.mAge;
	}
	//Is younger
	bool operator>(Message& _other)
	{
		return mAge > _other.mAge;
	}
	bool operator>(const Message& _other)
	{
		return mAge > _other.mAge;
	}
	bool operator>=(Message& _other)
	{
		return mAge >= _other.mAge;
	}
	bool operator>=(const Message& _other)
	{
		return mAge >= _other.mAge;
	}

	bool operator==(Message& _other)
	{
		return true;
	}
	bool operator==(const Message& _other)
	{
		return true;
	}

#pragma endregion

};
struct cmp
{
	bool operator()(Message* _l, Message* _r)
	{
		return true;
	}
};

class Messager
{
	static Messager* mMessager;

private:
	AudioWrapper audio;

	//static std::priority_queue<Message*, std::vector<Message*>, cmp> msgQueue;
	static std::queue<Message*> Messager::msgQueue;
	static bool death;

	void ProcessMessage(Message* _msg);

	void ProcessSound(Message* _msg);
	void ProcessRender(Message* _msg);
	void ProcessPhysics(Message* _msg);
	void ProcessInput(Message* _msg);
public:
	Messager();
	~Messager();
	static Messager& Instance();
	static void Destroy();

	void SendInMessage(Message* _msg);
	void Initialize();
	void Process();
};

//Bullshit Structs

struct m_Path
{
	const wchar_t* mPath;

	m_Path() { mPath = nullptr; }
	m_Path(const wchar_t* _path) { mPath = _path; }
};

struct m_LocEvent
{
	AudioEvent mID;
	vec4f* mPos;

	m_LocEvent() { }
	m_LocEvent(AudioEvent _id, vec4f* _pos) { mID = _id; mPos = _pos; }
};

