#include "CodeComponent.h"
#include "Action.h"

CodeComponent::CodeComponent(Action* _action) : mAction(_action) {
	mType = ComponentType::eCOMPONENT_CODE;
	if (mAction) {
		mAction->Start();
	} else {

		// TODO: print an error and probably destroy this component.
	}
}

void CodeComponent::Update() {
	if (mAction) {
		mAction->Update();
	} else {
		// TODO print an error to the error console that a code component has no action. This component should probably be destroyed.
	}
}

void CodeComponent::Destroy() {
	if (mAction) {
		mAction->OnDestroy();
	} else {
		// TODO: Print an error that the action does not exist.
	}
}
