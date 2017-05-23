#pragma once
#include "..\Objects\BaseObject.h"
#include "CodeComponent.hpp"
#include "..\Input\KeyboardInput.h"
#include "..\Common\Logger.h"
#include "..\Core\LevelManager.h"
#include "..\Common\Interpolator.h"
#include "..\Actions\CCLevel3BoxMovementButton.h"


namespace Epoch
{
	struct MoveableBox
	{
		BaseObject* mBox;
		Interpolator<matrix4>* mInterp = new Interpolator<matrix4>();
		int mId = 0;
		int mRow = 0;
		int mCol = 0;

		void SetUp(int _id,int _h, int _v)
		{
			mId = _id;
			mRow = _h;
			mCol = _v;
			mInterp->SetActive(false);
		}
		bool operator==(MoveableBox& _other)
		{
			if (mId == _other.mId &&
				mRow == _other.mRow &&
				mCol == _other.mCol &&
				mBox == _other.mBox)
				return true;
			return false;
		}
		void operator=(MoveableBox& _other)
		{
			mId = _other.mId;
			mRow = _other.mRow;
			mCol = _other.mCol;
			mBox = _other.mBox;
		}
	};
	struct CCMazeHelper : public CodeComponent
	{
		MoveableBox mazeBoxes[3];
		Level* cLevel;
		BaseObject* mLButton, *mRButton, *mUButton, *mDButton, *mResetButton;
		CCLevel3BoxMovementButton* mLBCC, *mRBCC, *mUBCC, *mDBCC, *mResetBCC;
		bool mBox1Done, mBox2Done, mBox3Done, mOnce = false;
		int mGrid[4][4] = {
			{ 0, 0,-1,-1 },
			{ 0,-1, 0, 0 },
			{ 1, 0,-1, 2 },
			{-1, 0, 0, 3 }
		};
		virtual void Start()
		{
			mBox1Done = mBox2Done = mBox3Done = true;
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			mazeBoxes[0].SetUp(1, 2, 0);
			mazeBoxes[1].SetUp(2, 2, 3);
			mazeBoxes[2].SetUp(3, 3, 3);
			mazeBoxes[0].mBox = cLevel->FindObjectWithName("EnvBox1");
			mazeBoxes[1].mBox = cLevel->FindObjectWithName("EnvBox2");
			mazeBoxes[2].mBox = cLevel->FindObjectWithName("EnvBox3");
			mLButton = cLevel->FindObjectWithName("Button1");
			mRButton = cLevel->FindObjectWithName("Button3");
			mUButton = cLevel->FindObjectWithName("Button2");
			mDButton = cLevel->FindObjectWithName("Button4");
			mResetButton = cLevel->FindObjectWithName("ResetButton");
			mLBCC = new CCLevel3BoxMovementButton();
			mRBCC = new CCLevel3BoxMovementButton();
			mUBCC = new CCLevel3BoxMovementButton(); 
			mDBCC = new CCLevel3BoxMovementButton();
			mResetBCC = new CCLevel3BoxMovementButton();

			mLButton->AddComponent(mLBCC);
			mRButton->AddComponent(mRBCC);
			mUButton->AddComponent(mUBCC);
			mDButton->AddComponent(mDBCC);
			mResetButton->AddComponent(mResetBCC);

			//PrintGrid();
		}
		virtual void Update()
		{
			if ((GetAsyncKeyState(VK_DOWN) & 0x1 || mDBCC->GetisColliding()) && (mBox1Done && mBox2Done && mBox3Done)) {
				MoveDown();
				mDBCC->SetisColliding(false);
				//PrintGrid();
			}
			else if (((GetAsyncKeyState(VK_UP) & 0x1) || mUBCC->GetisColliding()) && (mBox1Done && mBox2Done && mBox3Done)) {
				MoveUp();
				mUBCC->SetisColliding(false);
				//PrintGrid();
			}
			else if ((GetAsyncKeyState(VK_LEFT) & 0x1 || mLBCC->GetisColliding()) && (mBox1Done && mBox2Done && mBox3Done)) {
				MoveLeft();
				mLBCC->SetisColliding(false);
				//PrintGrid();
			}
			else if ((GetAsyncKeyState(VK_RIGHT) & 0x1 || mRBCC->GetisColliding()) && (mBox1Done && mBox2Done && mBox3Done)) {
				MoveRight();
				mRBCC->SetisColliding(false);
				//PrintGrid();
			}
			else if(GetAsyncKeyState(Epoch::Keys::R) & 0x1 || mResetBCC->GetisColliding())
			{

				if(!mOnce)
				{ 
				ResetBoxes();
				mOnce = true;
				mResetBCC->SetisColliding(false);
				//PrintGrid();
				}

			}
			else
			{
				mOnce = false;
			}
			//SystemLogger::GetLog() << curButtonPress << std::endl;
			//SystemLogger::GetLog() << lastButtonPress << std::endl;

			if (!Settings::GetInstance().GetBool("IsTimePaused")) {
				if (!mBox1Done) {
					mBox1Done = mazeBoxes[0].mInterp->Update(TimeManager::Instance()->GetDeltaTime());
					if (mBox1Done) {
						mazeBoxes[0].mInterp->SetActive(false);
						((SFXEmitter*)cLevel->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 5))->CallEvent(Emitter::ePlay);

					}
				}
				if (!mBox2Done) {
					mBox2Done = mazeBoxes[1].mInterp->Update(TimeManager::Instance()->GetDeltaTime());
					if (mBox2Done) {
						mazeBoxes[1].mInterp->SetActive(false);
						((SFXEmitter*)cLevel->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 5))->CallEvent(Emitter::ePlay);

					}
				}
				if (!mBox3Done) {
					mBox3Done = mazeBoxes[2].mInterp->Update(TimeManager::Instance()->GetDeltaTime());
					if (mBox3Done) {
						mazeBoxes[2].mInterp->SetActive(false);
						((SFXEmitter*)cLevel->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 5))->CallEvent(Emitter::ePlay);

					}
				}


			}
		}
		virtual void OnDestroy()
		{
			delete mazeBoxes[0].mInterp;
			delete mazeBoxes[1].mInterp;
			delete mazeBoxes[2].mInterp;
		}
		void PrintGrid()
		{
			for (int x = 0; x < 4; ++x) {
				for (int y = 0; y < 4; ++y) {
					SystemLogger::GetLog() << mGrid[x][y] << ",";
				}
				SystemLogger::GetLog() << std::endl;
			}
			SystemLogger::GetLog() << std::endl;
		}
		void MoveLeft() 
		{
			MoveableBox orderedBoxes[3];
			orderedBoxes[0] = mazeBoxes[0];
			orderedBoxes[1] = mazeBoxes[1];
			orderedBoxes[2] = mazeBoxes[2];
			int i, j, flag = 1;
			MoveableBox tempBox;
			for (i = 1; i <= 3 && flag; ++i){//bubble sort for array of 3
				flag = 0;
				for (j = 0; j < 2; ++j){ 
					if (orderedBoxes[j + 1].mCol < orderedBoxes[j].mCol)
					{
						tempBox = orderedBoxes[j];
						orderedBoxes[j] = orderedBoxes[j + 1];
						orderedBoxes[j + 1] = tempBox;
						flag = 1;
					}
				}
			}
			//Should be sorted now
			int currCoord = 0;
			for (i = 0; i < 3; ++i) {
				for (int k = orderedBoxes[i].mCol-1; k > -1; --k) {
					currCoord = mGrid[orderedBoxes[i].mRow][k];
					if (currCoord == 0) {
						mGrid[orderedBoxes[i].mRow][orderedBoxes[i].mCol] = 0;
						orderedBoxes[i].mCol = k;
						mGrid[orderedBoxes[i].mRow][orderedBoxes[i].mCol] = orderedBoxes[i].mId;
					}
					else
						break;
				}
			}

			matrix4* tempMatrix0, *tempMatrix1, *tempMatrix2;
			tempMatrix0 = &mazeBoxes[0].mBox->GetTransform().GetMatrix();
			tempMatrix1 = &mazeBoxes[1].mBox->GetTransform().GetMatrix();
			tempMatrix2 = &mazeBoxes[2].mBox->GetTransform().GetMatrix();
			//Boxes have been set to their respective positions
			for (i = 0; i < 3; ++i)
			{
				switch (orderedBoxes[i].mId)
				{
				case 1:
					mazeBoxes[0] = orderedBoxes[i];
					break;
				case 2:
					mazeBoxes[1] = orderedBoxes[i];
					break;
				case 3:
					mazeBoxes[2] = orderedBoxes[i];
					break;
				}
			}
			SetBoxesPosition(tempMatrix0, tempMatrix1, tempMatrix2);
		}
		void MoveRight()
		{
			MoveableBox orderedBoxes[3];
			orderedBoxes[0] = mazeBoxes[0];
			orderedBoxes[1] = mazeBoxes[1];
			orderedBoxes[2] = mazeBoxes[2];
			int i, j, flag = 1;
			MoveableBox tempBox;
			for (i = 1; i <= 3 && flag; ++i) {//bubble sort for array of 3
				flag = 0;
				for (j = 0; j < 2; ++j) {
					if (orderedBoxes[j + 1].mCol > orderedBoxes[j].mCol)
					{
						tempBox = orderedBoxes[j];
						orderedBoxes[j] = orderedBoxes[j + 1];
						orderedBoxes[j + 1] = tempBox;
						flag = 1;
					}
				}
			}
			//Should be sorted now
			int currCoord = 0;
			for (i = 0; i < 3; ++i) {
				for (int k = orderedBoxes[i].mCol + 1; k < 4; ++k) {
					currCoord = mGrid[orderedBoxes[i].mRow][k];
					if (currCoord == 0) {
						mGrid[orderedBoxes[i].mRow][orderedBoxes[i].mCol] = 0;
						orderedBoxes[i].mCol = k;
						mGrid[orderedBoxes[i].mRow][orderedBoxes[i].mCol] = orderedBoxes[i].mId;
					}
					else
						break;
				}
			}
			matrix4* tempMatrix0, *tempMatrix1, *tempMatrix2;
			tempMatrix0 = &mazeBoxes[0].mBox->GetTransform().GetMatrix();
			tempMatrix1 = &mazeBoxes[1].mBox->GetTransform().GetMatrix();
			tempMatrix2 = &mazeBoxes[2].mBox->GetTransform().GetMatrix();
			//Boxes have been set to their respective positions
			for (i = 0; i < 3; ++i)
			{
				switch (orderedBoxes[i].mId)
				{
				case 1:
					mazeBoxes[0] = orderedBoxes[i];
					break;
				case 2:
					mazeBoxes[1] = orderedBoxes[i];
					break;
				case 3:
					mazeBoxes[2] = orderedBoxes[i];
					break;
				}
			}
			SetBoxesPosition(tempMatrix0, tempMatrix1, tempMatrix2);
		}
		void MoveUp()
		{
			MoveableBox orderedBoxes[3];
			orderedBoxes[0] = mazeBoxes[0];
			orderedBoxes[1] = mazeBoxes[1];
			orderedBoxes[2] = mazeBoxes[2];
			int i, j, flag = 1;
			MoveableBox tempBox;
			for (i = 1; i <= 3 && flag; ++i) {//bubble sort for array of 3
				flag = 0;
				for (j = 0; j < 2; ++j) {
					if (orderedBoxes[j + 1].mRow < orderedBoxes[j].mRow)
					{
						tempBox = orderedBoxes[j];
						orderedBoxes[j] = orderedBoxes[j + 1];
						orderedBoxes[j + 1] = tempBox;
						flag = 1;
					}
				}
			}
			//Should be sorted now
			int currCoord = 0;
			for (i = 0; i < 3; ++i) {
				for (int k = orderedBoxes[i].mRow - 1; k > -1;--k) {
					currCoord = mGrid[k][orderedBoxes[i].mCol];
					if (currCoord == 0) {
						mGrid[orderedBoxes[i].mRow][orderedBoxes[i].mCol] = 0;
						orderedBoxes[i].mRow = k;
						mGrid[orderedBoxes[i].mRow][orderedBoxes[i].mCol] = orderedBoxes[i].mId;
					}
					else
						break;
				}
			}
			matrix4* tempMatrix0, *tempMatrix1, *tempMatrix2;
			tempMatrix0 = &mazeBoxes[0].mBox->GetTransform().GetMatrix();
			tempMatrix1 = &mazeBoxes[1].mBox->GetTransform().GetMatrix();
			tempMatrix2 = &mazeBoxes[2].mBox->GetTransform().GetMatrix();
			//Boxes have been set to their respective positions
			for (i = 0; i < 3; ++i)
			{
				switch (orderedBoxes[i].mId)
				{
				case 1:
					mazeBoxes[0] = orderedBoxes[i];
					break;
				case 2:
					mazeBoxes[1] = orderedBoxes[i];
					break;
				case 3:
					mazeBoxes[2] = orderedBoxes[i];
					break;
				}
			}
			SetBoxesPosition(tempMatrix0, tempMatrix1, tempMatrix2);

		}
		void MoveDown()
		{
			MoveableBox orderedBoxes[3];
			orderedBoxes[0] = mazeBoxes[0];
			orderedBoxes[1] = mazeBoxes[1];
			orderedBoxes[2] = mazeBoxes[2];
			int i, j, flag = 1;
			MoveableBox tempBox;
			for (i = 1; i <= 3 && flag; ++i) {//bubble sort for array of 3
				flag = 0;
				for (j = 0; j < 2; ++j) {
					if (orderedBoxes[j + 1].mRow > orderedBoxes[j].mRow)
					{
						tempBox = orderedBoxes[j];
						orderedBoxes[j] = orderedBoxes[j + 1];
						orderedBoxes[j + 1] = tempBox;
						flag = 1;
					}
				}
			}
			//Should be sorted now
			int currCoord = 0;
			for (i = 0; i < 3; ++i) {
				for (int k = orderedBoxes[i].mRow + 1; k < 4; ++k) {
					currCoord = mGrid[k][orderedBoxes[i].mCol];
					if (currCoord == 0) {
						mGrid[orderedBoxes[i].mRow][orderedBoxes[i].mCol] = 0;
						orderedBoxes[i].mRow = k;
						mGrid[orderedBoxes[i].mRow][orderedBoxes[i].mCol] = orderedBoxes[i].mId;
					}
					else
						break;
				}
			}
			matrix4* tempMatrix0, *tempMatrix1, *tempMatrix2;
			tempMatrix0 = &mazeBoxes[0].mBox->GetTransform().GetMatrix();
			tempMatrix1 = &mazeBoxes[1].mBox->GetTransform().GetMatrix();
			tempMatrix2 = &mazeBoxes[2].mBox->GetTransform().GetMatrix();
			//Boxes have been set to their respective positions
			for (i = 0; i < 3; ++i)
			{
				switch(orderedBoxes[i].mId)
				{
				case 1:
					mazeBoxes[0] = orderedBoxes[i];
					break;
				case 2:
					mazeBoxes[1] = orderedBoxes[i];
					break;
				case 3:
					mazeBoxes[2] = orderedBoxes[i];
					break;
				}
			}
			SetBoxesPosition(tempMatrix0,tempMatrix1,tempMatrix2);
			
		}
		void ResetBoxes()
		{
			matrix4 temp0, temp1, temp2;
			temp0 = mazeBoxes[0].mBox->GetTransform().GetMatrix();
			temp1 = mazeBoxes[1].mBox->GetTransform().GetMatrix();
			temp2 = mazeBoxes[2].mBox->GetTransform().GetMatrix();
			mGrid[mazeBoxes[0].mRow][mazeBoxes[0].mCol] = 0;
			mGrid[mazeBoxes[1].mRow][mazeBoxes[1].mCol] = 0;
			mGrid[mazeBoxes[2].mRow][mazeBoxes[2].mCol] = 0;
			mazeBoxes[0].SetUp(1, 2, 0);
			mazeBoxes[1].SetUp(2, 2, 3);
			mazeBoxes[2].SetUp(3, 3, 3);
			mGrid[mazeBoxes[0].mRow][mazeBoxes[0].mCol] = 1;
			mGrid[mazeBoxes[1].mRow][mazeBoxes[1].mCol] = 2;
			mGrid[mazeBoxes[2].mRow][mazeBoxes[2].mCol] = 3;

			SetBoxesPosition(&temp0, &temp1, &temp2);

			//Shoot Particles
			/*Particle* start = &Particle::Init();
			start->SetPos(vec3f(0, -3.0f, 0));
			start->SetColors(vec3f(.6f, .6f, .8f), vec3f(.3f, 0, .3f));
			start->SetLife(200);
			start->SetSize(.1f, .1f);
			ParticleEmitter* startEmit = new ParticleEmitter(600, 500, 2, vec4f(0, -3,0, 1));
			startEmit->SetParticle(start);
			startEmit->SetTexture("../Resources/BasicCircleP.png");
			startEmit->SetPosBounds(vec3f(-7, 0, -7), vec3f(7, 4, 7));
			startEmit->SetVelBounds(vec3f(1, 5, .5f), vec3f(2, 10, 1));
			ParticleSystem::Instance()->AddEmitter(startEmit);
			startEmit->FIRE();
			((SFXEmitter*)mObject->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent(Emitter::ePlay);*/

			Particle * p = &Particle::Init();
			p->SetColors(vec4f(.8f, 0, .8f, 1), vec4f());
			p->SetLife(600);
			p->SetSize(.25f, .15f);
			vec3f EPos = vec4f(0, -3, 0, 1);
			ParticleEmitter *emit = new ParticleEmitter(400, 200, 20, EPos);
			emit->SetPosBounds(vec3f(-7, 0, -7), vec3f(7, 1.5f, 7));
			emit->SetVelBounds(vec3f(1, 1, .5f), vec3f(2, 3, 1));
			emit->SetParticle(p);
			emit->SetTexture("../Resources/BasicCircleP.png");
			ParticleSystem::Instance()->AddEmitter(emit);
			emit->FIRE();

		}
		void SetBoxesPosition(matrix4* matrix0, matrix4* matrix1, matrix4* matrix2)
		{
			matrix4* curMatrix = nullptr;
			matrix4 finalDest;
			for (int i = 0; i < 3; ++i)
			{
				switch (i)
				{
				case 0:
					curMatrix = matrix0;
					break;
				case 1:
					curMatrix = matrix1;
					break;
				case 2:
					curMatrix = matrix2;
					break;
				}
				int X = mazeBoxes[i].mRow;
				int Y = mazeBoxes[i].mCol;
				//Row 0
				if (X == 0 && Y == 0) {
					finalDest = matrix4::CreateScale(2.1f, 1.5f, 2) * matrix4::CreateTranslation(6, -1.41f, 6.2f);
				}
				else if (X == 0 && Y == 1) {
					finalDest = matrix4::CreateScale(2.1f, 1.5f, 2) *  matrix4::CreateTranslation(2, -1.41f, 6.2f);
				}
				//Row 1
				else if (X == 1 && Y == 0) {
					finalDest = matrix4::CreateScale(2.1f, 1.5f, 2) * matrix4::CreateTranslation(6, -1.41f, 2.2f);
				}
				else if (X == 1 && Y == 2) {
					finalDest = matrix4::CreateScale(2.1f, 1.5f, 2) *  matrix4::CreateTranslation(-2, -1.41f, 2.2f);
				}
				else if (X == 1 && Y == 3) {
					finalDest = matrix4::CreateScale(2.1f, 1.5f, 2) * matrix4::CreateTranslation(-6, -1.41f, 2.2f);
				}
				//Row 2
				else if (X == 2 && Y == 0) {
					finalDest = matrix4::CreateScale(2.1f, 1.5f, 2) * matrix4::CreateTranslation(6, -1.41f, -1.80f);
				}
				else if (X == 2 && Y == 1) {
					finalDest = matrix4::CreateScale(2.1f, 1.5f, 2) * matrix4::CreateTranslation(2, -1.41f, -1.80f);
				}
				else if (X == 2 && Y == 3) {
					finalDest = matrix4::CreateScale(2.1f, 1.5f, 2) *  matrix4::CreateTranslation(-6, -1.41f, -1.80f);
				}
				//Row 3
				else if (X == 3 && Y == 0) {
					finalDest = matrix4::CreateScale(2.1f, 1.5f, 2) * matrix4::CreateTranslation(6, -1.41f, -5.80f);
				}
				else if (X == 3 && Y == 1) {
					finalDest = matrix4::CreateScale(2.1f, 1.5f, 2) * matrix4::CreateTranslation(2, -1.41f, -5.80f);
				}
				else if (X == 3 && Y == 2) {
					finalDest = matrix4::CreateScale(2.1f, 1.5f, 2) * matrix4::CreateTranslation(-2, -1.41f, -5.80f);
				}
				else if (X == 3 && Y == 3) {
					finalDest = matrix4::CreateScale(2.1f, 1.5f, 2) * matrix4::CreateTranslation(-6, -1.41f, -5.80f);
				}
				if (*curMatrix != finalDest)
				{
					mazeBoxes[i].mInterp->Prepare(0.2f, *curMatrix, finalDest, mazeBoxes[i].mBox->GetTransform().GetMatrix());
					mazeBoxes[i].mInterp->SetActive(true);
					switch (i)
					{
					case 0:
						mBox1Done = false;
						break;
					case 1:
						mBox2Done = false;
						break;
					case 2:
						mBox3Done = false;
						break;
					}
				}
			}
			//mBox1Done = mBox2Done = mBox3Done = false;
		}
		void SetBoxesAsGrid()
		{
			for (unsigned int i = 0; i <3; i++) {
				for (unsigned int x = 0; x < 4; x++) {
					for (unsigned int y = 0; y < 4; y++) {
						if(mGrid[x][y] == mazeBoxes[i].mId)
						{
							mazeBoxes[i].mRow = x;
							mazeBoxes[i].mCol = y;
						}
					}
				}
			}
		}
	};
}