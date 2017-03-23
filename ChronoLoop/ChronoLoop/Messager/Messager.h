#pragma once
#include "..\Sound\SoundEngine.h"
#include "..\Sound\Sound\Soundbanks\Wwise_IDs.h"
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include "..\Common\Logger.h"
#include <cstdarg>

namespace Epoch
{
	enum msgTypes { NONE = -1, mSound };

	enum soundMsg {
		INITIALIZE_Audio, SHUTDOWN_Audio, UPDATE_Audio, ADD_Listener, REMOVE_Listener,
		ADD_Emitter, REMOVE_Emitter, MAKEEVENT_Loc, MAKEEVENT_Listener, MAKEEVENT_Event,
		SET_BasePath, ADD_Soundbank, REMOVE_Soundbank
	};

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

		~Message() {
			delete mNeed;
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

	class Messager
	{
		static Messager* mMessager;

	private:
		//static std::priority_queue<Message*,std::vector<Message*>> msgQueue;
		std::thread thrd;
		std::mutex mLock;

		std::queue<Message*> msgQueue;
		bool death;

		void ProcessMessage(Message* _msg);

		void ProcessSound(Message* _msg);
		Messager();
		Messager(const Messager& _newMsger) {}
		Messager& operator=(const Messager& _m) { return *this; }
	public:
		~Messager();
		static Messager& Instance();
		static void Destroy();

		void SendInMessage(Message* _msg);
		void Initialize();
		void Process();
	};

	//Sound Func Structs--------------------
	struct m_Path
	{
		const wchar_t* mPath;

		m_Path() { mPath = nullptr; }
		m_Path(const wchar_t* _path) { mPath = _path; }
	};

	struct m_LocEvent
	{
		AudioEvent mID;
		const vec4f* mPos;

		m_LocEvent() { }
		m_LocEvent(AudioEvent _id, const vec4f* _pos) { mID = _id; mPos = _pos; }
	};

	struct m_Event
	{
		AudioEvent mID;
		Emitter* mEmitter;

		m_Event() { }
		m_Event(AudioEvent _id, Emitter* _emitter) { mID = _id; mEmitter = _emitter; }
	};

	struct m_EventListener
	{
		AudioEvent mID;
		unsigned int mLID;

		m_EventListener() { mLID = 0; }
		m_EventListener(AudioEvent _id, unsigned int _lID = 0) { mID = _id; mLID = _lID; }
	};

	struct m_Listener
	{
		Listener* mListener;
		char * mName;

		m_Listener() {}
		m_Listener(Listener* _listener, char* _name) { mListener = _listener; mName = _name; }
	};

	struct m_Emitter
	{
		Emitter* mEmitter;
		const char* mName;

		m_Emitter() {}
		m_Emitter(Emitter* _emitter, const char* _name) { mEmitter = _emitter; mName = _name; }
	};

	struct m_FLT
	{
		float mScale;

		m_FLT() { mScale = 1.0f; }
		m_FLT(float _scale = 1.0f) { mScale = _scale; }
	};
	//-------------------------------------
}
