#pragma once

#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "..\Physics\Physics.h"
#include "..\Common\Logger.h"
#include "../Core/Level.h"
#include "../Core/TimeManager.h"
#include "../Core/LevelManager.h"

namespace Epoch
{

	struct CCButtonHold : public CodeComponent
	{
		bool colliding = false, mhitting = false, mCanDoorInterp = false, mDoorDoneInterpolating = false, mFlip = false, mSoundOnce = false, mPrepMiddleDoor = false;
		bool tempDoor = false;
		BaseObject *Block = nullptr, *Exit = nullptr;
		std::vector<BaseObject*> mD1Wires;
		std::vector<BaseObject*> mD2Wires;
		CubeCollider* blockCube, *exitCube;
		matrix4 blockend, exitend , blockstart, exitstart;
		Interpolator<matrix4>* blockInterp;
		Interpolator<matrix4>* exitInterp;
		//vec3f blockend, exitend;


		Level* cLevel;
		void SetDoorInterpBool(bool _set) { mCanDoorInterp = _set; };
		bool GetDoorInterpBool() { return mCanDoorInterp; };
		virtual void Start()
		{

			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			Block = cLevel->FindObjectWithName("EnvTransparentDoor1");
			Exit = cLevel->FindObjectWithName("EnvTransparentDoor2");
			blockInterp = TimeManager::Instance()->GetObjectInterpolator(Block->GetUniqueID());
			exitInterp = TimeManager::Instance()->GetObjectInterpolator(Exit->GetUniqueID());

			blockCube = (CubeCollider*)Block->mComponents[eCOMPONENT_COLLIDER][0];
			exitCube = (CubeCollider*)Exit->mComponents[eCOMPONENT_COLLIDER][0];
			//blockend = blockCube->GetPos() - vec3f(0, 2.6f, 0);
			//exitend = exitCube->GetPos() + vec3f(0, 2.6f, 0);
			blockstart = blockCube->GetTransform().GetMatrix();
			exitstart = exitCube->GetTransform().GetMatrix();
			blockend = blockCube->GetTransform().GetMatrix().CreateTranslation(vec4f(0, 2.6f, 0, 1));
			exitend = exitCube->GetTransform().GetMatrix().CreateTranslation(vec4f(0, -2.6f, 0, 1));

			mD1Wires = cLevel->FindAllObjectsByPattern("D1Wire");
			mD2Wires = cLevel->FindAllObjectsByPattern("D2Wire");

			//Turn on wires that need to be turned on
			//D1 wires meshes are grey then green
			for (unsigned int i = 0; i < mD1Wires.size(); i++) {
				MeshComponent* temp = (MeshComponent*)mD1Wires[i]->GetComponentIndexed(eCOMPONENT_MESH, 0);
				if (temp) {
					temp->SetVisible(true);
				}
				temp = (MeshComponent*)mD1Wires[i]->GetComponentIndexed(eCOMPONENT_MESH, 1);
				if (temp) {
					temp->SetVisible(false);
				}
			}
			//D2 wires meshes are green then grey
			for (unsigned int i = 0; i < mD2Wires.size(); i++) {
				MeshComponent* temp = (MeshComponent*)mD2Wires[i]->GetComponentIndexed(eCOMPONENT_MESH, 0);
				if (temp) {
					temp->SetVisible(true);
				}
				temp = (MeshComponent*)mD2Wires[i]->GetComponentIndexed(eCOMPONENT_MESH, 1);
				if (temp) {
					temp->SetVisible(false);
				}
			}
		}

		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane && ((Component*)&_other)->GetBaseObject()->GetName() != "EnvButtonstand")
			{

				colliding = true;
				//SystemLogger::GetLog() << "Colliding" << std::endl;
				//Interp stuff
				

				if (!tempDoor) {
					blockInterp->SetActive(true);
					blockInterp->Prepare(0.69f, blockCube->GetTransform().GetMatrix(), blockend, blockCube->GetTransform().GetMatrix());

					//exitCube->SetPos(exitend);
					exitInterp->SetActive(true);
					exitInterp->Prepare(0.69f, exitCube->GetTransform().GetMatrix(), exitend, exitCube->GetTransform().GetMatrix());

					((Collider*)blockCube->GetBaseObject()->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(blockend.Position);
					((Collider*)exitCube->GetBaseObject()->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(exitend.Position);

					mCanDoorInterp = true;
					mDoorDoneInterpolating = false;
					tempDoor = true;

					//Loop to change texture on wires
					//D1 wires meshes are green then grey
					for (unsigned int i = 0; i < mD1Wires.size(); i++) {
						MeshComponent* temp = (MeshComponent*)mD1Wires[i]->GetComponentIndexed(eCOMPONENT_MESH, 0);
						if (temp)
							temp->SetVisible(false);
						temp = (MeshComponent*)mD1Wires[i]->GetComponentIndexed(eCOMPONENT_MESH, 1);
						if (temp)
							temp->SetVisible(true);
					}
					//D2 wires meshes are grey then green
					for (unsigned int i = 0; i < mD2Wires.size(); i++) {
						MeshComponent* temp = (MeshComponent*)mD2Wires[i]->GetComponentIndexed(eCOMPONENT_MESH, 0);
						if (temp)
							temp->SetVisible(false);
						temp = (MeshComponent*)mD2Wires[i]->GetComponentIndexed(eCOMPONENT_MESH, 1);
						if (temp)
							temp->SetVisible(true);
					}
				}

				// Sound 
				if (!mSoundOnce) {
					if (_col.GetBaseObject()->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 0) {
						if (dynamic_cast<AudioEmitter*>(_col.GetBaseObject()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0)))
							((AudioEmitter*)_col.GetBaseObject()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent(Emitter::ePlay);
					}

					if (Block->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 0) {
						if (dynamic_cast<AudioEmitter*>(Block->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0)))
							((AudioEmitter*)Block->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent(Emitter::ePlay);
					}
					if (Exit->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 0) {
						if (dynamic_cast<AudioEmitter*>(Exit->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0)))
							((AudioEmitter*)Exit->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent(Emitter::ePlay);
					}
				mSoundOnce = true;
				}

				ButtonCollider* butCol = (ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);

				if (fabsf(_other.mVelocity * butCol->mPushNormal) < .1f)
					butCol->mVelocity = vec3f(0, 0, 0);
				else
				{
					vec3f norm = ((ButtonCollider*)&_col)->mPushNormal;
					vec3f tForce = norm * (norm * _other.mTotalForce);
					vec3f vel = norm * (norm * _other.mVelocity);
					vec3f accel = norm * (norm * _other.mAcceleration);
					if (tForce * norm < 0 && vel * norm < 0 && accel * norm < 0) {
						_col.mTotalForce = tForce;
						_col.mVelocity = vel;
						_col.mAcceleration = vel / _time;
						//blockCube->SetPos(blockend);
					}
				}
			}
		}
		virtual void Update()
		{
			if (!LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused()) {
				if (colliding)
				{
					//SystemLogger::GetLog() << "Colliding" << std::endl;
					mFlip = true;
					if (mCanDoorInterp && !mDoorDoneInterpolating)
					{
						mDoorDoneInterpolating = (blockInterp->Update(TimeManager::Instance()->GetDeltaTime()) || exitInterp->Update(TimeManager::Instance()->GetDeltaTime()));
					}
					else
					{
						mCanDoorInterp = false;
						blockInterp->SetActive(false);
						exitInterp->SetActive(false);
					}
				}
				else
				{
					tempDoor = false;
					//SystemLogger::GetLog() << "Not Colliding" << std::endl;
					if(mFlip)
					{
						mFlip = false;
						blockInterp->SetActive(true);
						blockInterp->Prepare(0.69f, blockCube->GetTransform().GetMatrix(), blockstart, blockCube->GetTransform().GetMatrix());

						exitInterp->SetActive(true);
						exitInterp->Prepare(0.69f, exitCube->GetTransform().GetMatrix(), exitstart, exitCube->GetTransform().GetMatrix());
						((Collider*)blockCube->GetBaseObject()->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(blockstart.Position);
						((Collider*)exitCube->GetBaseObject()->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(exitstart.Position);

						mCanDoorInterp = true;
						mDoorDoneInterpolating = false;

						//Loop to change texture on wires
						for (unsigned int i = 0; i < mD1Wires.size(); i++) {
							MeshComponent* temp = (MeshComponent*)mD1Wires[i]->GetComponentIndexed(eCOMPONENT_MESH, 0);
							if (temp)
								temp->SetVisible(true);
							temp = (MeshComponent*)mD1Wires[i]->GetComponentIndexed(eCOMPONENT_MESH, 1);
							if (temp)
								temp->SetVisible(false);
						}
						for (unsigned int i = 0; i < mD2Wires.size(); i++)
						{
							MeshComponent* temp = (MeshComponent*)mD2Wires[i]->GetComponentIndexed(eCOMPONENT_MESH, 0);
							if (temp)
								temp->SetVisible(true);
							temp = (MeshComponent*)mD2Wires[i]->GetComponentIndexed(eCOMPONENT_MESH, 1);
							if (temp)
								temp->SetVisible(false);
						}

							if (Block->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 1)
							{
								if (dynamic_cast<AudioEmitter*>(Block->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1)))
									((AudioEmitter*)Block->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::ePlay);
							}
							if (Exit->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 1)
							{
								if (dynamic_cast<AudioEmitter*>(Exit->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1)))
									((AudioEmitter*)Exit->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::ePlay);
							}
							mSoundOnce = false;
						
					}
					if (blockCube->GetTransform().GetMatrix() != blockstart && exitCube->GetTransform().GetMatrix() != exitstart)
					{
						mCanDoorInterp = true;
						mDoorDoneInterpolating = false;
						blockInterp->SetActive(true);
						blockInterp->Prepare(0.3f, blockCube->GetTransform().GetMatrix(), blockstart, blockCube->GetTransform().GetMatrix());

						exitInterp->SetActive(true);
						exitInterp->Prepare(0.3f, exitCube->GetTransform().GetMatrix(), exitstart, exitCube->GetTransform().GetMatrix());
					}
					if (mCanDoorInterp && !mDoorDoneInterpolating)
					{
						mDoorDoneInterpolating = (blockInterp->Update(TimeManager::Instance()->GetDeltaTime()) || exitInterp->Update(TimeManager::Instance()->GetDeltaTime()));
					}
					else
					{
						mCanDoorInterp = false;
						blockInterp->SetActive(false);
						exitInterp->SetActive(false);
					}
					mSoundOnce = false;
				}
			}
			colliding = false;
		}
	};
}
