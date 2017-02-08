#pragma once
#include "..\Sound\SoundEngine.h"
#include <queue>
#include <vector>
#include <cstdarg>

enum messageTypes {
	NONE = -1,
	SoundEngine,
	RenderEngine,
	Input,
	Physics
	
};

enum soundMessages
{
	//SoundEngine
	INITAILIZE_Audio,
	SHUTDOWN_Audio,
	UPDATE_Audio,
	ADD_Listener,
	REMOVE_Listener,
	ADD_Emitter,
	REMOVE_Emitter,
	SET_BasePath,
	ADD_Soundbank,
	REMOVE_Soundbank
};
enum renderMessages
{

};
enum inputMessages
{

};
enum physicsMessages
{

};

template<class... Args>
struct Message
{
	long age;
	bool isPriority;

	messageTypes typ;
	union
	{
		int num;
		soundMessages smType;
		renderMessages rmTypes;
		inputMessages imTypes;
		physicsMessages pmTypes;
	};
	std::tuple<Args...> need;



	Message()
	{
		age = 0;
		isPriority = false;
	}
	Message(Message& _msg)
	{
		age = _msg.age;
		isPriority = _msg.isPriority;
		typ = _msg.typ;
		need = _msg.need;
	}

	template<class...Args>
	Message(messageTypes _type, Args&&... _args)
	{
		typ = _type;
		need = make_tuple(_args);
	}

#pragma region Operator Overrides

	bool operator==(const Message& _msg)
	{
		return (typ == _msg.typ) && (num == _msg.num) && (need = _msg.need) && (age == _msg.age) && (isPriority == _msg.isPriority);
	}
	bool operator==(Message& _msg)
	{
		return (typ == _msg.typ) && (num == _msg.num) && (need = _msg.need) && (age == _msg.age) && (isPriority == _msg.isPriority);
	}

	//Compares if age is less, determines if its older
	bool operator<(const Message& _msg)
	{
		return (age < _msg.age)
	}
	bool operator<(Message& _msg)
	{
		return (age < _msg.age)
	}

	//Compares if age is greater, determines if its younger
	bool operator>(const Message& _msg)
	{
		return (age > _msg.age)
	}
	bool operator>(Message& _msg)
	{
		return (age > _msg.age)
	}

#pragma endregion

};

struct CMP {

	template<class... Args>
	bool compare(const Message<Args...>& _r, const Message<Args...>& _l)
	{
		return true;
	}
};


class Messager
{
	static Messager* mMessenger;

private:
	AudioWrapper audio;

	template<class... Args>
	static std::priority_queue<Message<Args...>, std::vector<Message<Args...>>, CMP> msgQueue;


public:
	template<class... Args>
	Messager();
	~Messager();

	Messager* GetMessenger() { return mMessenger; }

	template<class... Args>
	void SendInMessage(Message<Args...> _msg);
	template<class... Args>
	void SendOutMessage(Message<Args...> _msg);

	template<class... Args>
	void Process();

private:
	template<class... Args>
	void ProcessMessage(Message<Args...> _msg);

	template<class... Args>
	void ProcessSound(Message<Args...> _msg);
	template<class... Args>
	void ProcessRender(Message<Args...> _msg);
	template<class... Args>
	void ProcessPhysics(Message<Args...> _msg);
	template<class... Args>
	void ProcessInput(Message<Args...> _msg);
};
