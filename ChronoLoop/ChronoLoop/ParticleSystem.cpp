#include "ParticleSystem.h"

ParticleSystem* ParticleSystem::mSystem = nullptr;

ParticleSystem::ParticleSystem()
{

}
ParticleSystem::~ParticleSystem()
{
	for (ParticleEmitter* emit : mPEmitters)
		delete emit;

	mPEmitters.clear();
}

ParticleSystem* ParticleSystem::Instance()
{
	if (!mSystem)
		mSystem = new ParticleSystem();

	return mSystem;
}

void ParticleSystem::Destroy()
{
	delete mSystem;
}

void ParticleSystem::Update()
{
	//TODO: Update emitters
}

void ParticleSystem::Render()
{
	//TODO: Set up and render
}

void ParticleSystem::AddEmitter(ParticleEmitter* _pemitter)
{
	mPEmitters.push_back(_pemitter);
}

void ParticleSystem::RemoveEmitter(ParticleEmitter* _pemitter)
{
	auto iter = std::find(mPEmitters.begin(), mPEmitters.end(), _pemitter);
	if (iter != mPEmitters.end())
		mPEmitters.erase(iter);
}

bool ParticleSystem::DoesExist(ParticleEmitter* _pemitter)
{
	if (std::find(mPEmitters.begin(), mPEmitters.end(), _pemitter) != mPEmitters.end())
		return true;

	return false;
}