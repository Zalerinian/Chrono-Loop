#pragma once

#include <cstdarg>

enum messageTypes {
	NONE = -1,
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
	REMOVE_Soundbank};



template<class... Args>
struct Message
{
	long age;
	bool isPriority;

	messageTypes typ;
	std::tuple<Args...> need;

	template<class...Args>
	Message(Args&&... _args)
	{
		need = make_tuple(_args);
	}
};