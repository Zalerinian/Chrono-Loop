#pragma once
#include "..\Sound\SoundEngine.h"
#include <queue>
#include <vector>
#include <thread>
#include <cstdarg>

//TUPLE voodoo
namespace TupleSplitter {
	template<typename F, typename Tuple, size_t ...S >
	decltype(auto) apply_tuple_impl(F&& fn, Tuple&& t, std::index_sequence<S...>) {
		return std::initializer_list<int>{
			(std::forward<F>(fn)(std::get<S>(std::forward<Tuple>(t))), void(), 0)...
		};
			//std::forward<F>(fn)(std::get<S>(std::forward<Tuple>(t))...);
	}

	template<typename F, typename Tuple>
	decltype(auto) apply(F&& fn, Tuple&& t) {
		std::size_t constexpr tSize
			= std::tuple_size<typename std::remove_reference<Tuple>::type>::value;
		return apply_tuple_impl(std::forward<F>(fn),
			std::forward<Tuple>(t),
			std::make_index_sequence<tSize>());
		// I'm at least 15% sure that the make_index_sequence bit is for making the
		// third template argument valid, as technically the value itself is never
		// used in the function, but it is used as part of the template.

		// std::forward makes a regular lvalue into an rvalue, but an lvalue reference
		// is unchanged.

		// std::get is used to get a value from a tuple. Since it can hold any number
		// of any number of types, you can't index into it like a regular array or
		// anything. std::get expects an index as a template argument for which
		// value to get.
	}
}

enum messageTypes {
	NONE = -1,
	SoundEngine,
	Render,
	Input,
	Science
	
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
private:
public:

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
	Message(Message<Args...>& _msg)
	{
		age = _msg.age;
		isPriority = _msg.isPriority;
		typ = _msg.typ;
		need = _msg.need;
	}

	Message(messageTypes _type, int _secondtype, long _age, bool _priority, Args&&... _args)
	{
		typ = _type;
		num = _secondtype;
		age = 0;
		isPriority = _priority;
		need = std::make_tuple(_args...);
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
	bool operator()(const void* _r, const void* _l)
	{
		return true;
	}
};


class Messager
{
	static Messager* mMessenger;

private:
	AudioWrapper audio;

	static std::queue<void*> msgQueue;
	static bool death;
	static void(*proc)(void);

public:
	template<class... Args>
	Messager();
	~Messager();
	static Messager& Instance();
	template<class... Args>
	static void Destroy();
	Messager* GetMessenger() { return mMessenger; }

	void SendInMessage(void* _msg);
	template<class... Args>
	void SendOutMessage(Message<Args...>* _msg);

	template<class... Args>
	void Initialize();

	template<class... Args>
	void Process();

private:
	template<class... Args>
	void ProcessMessage(Message<Args...>* _msg);

	template<class... Args>
	void ProcessSound(Message<Args...>* _msg);
	template<class... Args>
	void ProcessRender(Message<Args...>* _msg);
	template<class... Args>
	void ProcessPhysics(Message<Args...>* _msg);
	template<class... Args>
	void ProcessInput(Message<Args...>* _msg);
};
