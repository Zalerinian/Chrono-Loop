#pragma once
#include <vector>
#include "ParticleComponents.h"

struct ID3D11GeometryShader;

class ParticleSystem
{
private:
	static ParticleSystem* mSystem;

	std::vector<ParticleEmitter*> mPEmitters;
	ID3D11GeometryShader* mGeometryShader;
	GSMatrix bufferData;

	ParticleSystem();
	~ParticleSystem();

public:
	static ParticleSystem* Instance();
	static void Destroy();

	void SetGeometryShader(const char * _file);

	void Update();
	void Render();
	void AddEmitter(ParticleEmitter* _pemitter);
	void RemoveEmitter(ParticleEmitter* _pemitter);
	bool DoesExist(ParticleEmitter* _pemitter);

};