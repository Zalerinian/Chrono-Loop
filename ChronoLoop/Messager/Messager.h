#pragma once
//#include "..\SoundEngine\SoundEngine.h"

struct Message;

class Messager
{
	static Messager* mMessenger;

private:

	//AudioWrapper audio;




public:
	enum messageTypes{	NONE = -1,
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
						REMOVE_Soundbank,
						
						
						
						
						
						
						
						
							
								};

	Messager();
	~Messager();

	Messager* GetMessenger() { return mMessenger; }

	void SendInMessage();
	void SendOutMessage();

	void Process();

private:
	void ProcessMessage(Message _msg);
};