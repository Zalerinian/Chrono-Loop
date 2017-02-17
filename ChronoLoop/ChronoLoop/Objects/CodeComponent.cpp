#include "CodeComponent.h"
#include "../Actions/Action.hpp"
#include "../Common/Logger.h"
#include "BaseObject.h"

CodeComponent::CodeComponent(Action* _action) : mAction(_action) {
	mType = ComponentType::eCOMPONENT_CODE;
	if (mAction) {
		mAction->Start();
	} else {
		SystemLogger::GetError() << "[Error] A Code Component has been added with an invalid action parameter. The component will be removed from the object." << std::endl;
		mIsValid = false;
		mIsEnabled = false;
	}
}

void CodeComponent::Update() {
	if (mAction && mIsValid) {
		mAction->Update();
	} else {
		SystemLogger::GetError() << "[Error] To be honest, I don't know how, but a CodeComponent called Update with an invalid Action object." << std::endl;
	}
}

void CodeComponent::Destroy() {
	if (mAction) {
		mAction->OnDestroy();
		delete mAction;
	} else {
		SystemLogger::GetError() << "[Error] A Code Component has been destroyed, but it contains an invalid action. The component will be removed from its object, and no OnDestroy function will be called." << std::endl;
	}
	delete this;
}
