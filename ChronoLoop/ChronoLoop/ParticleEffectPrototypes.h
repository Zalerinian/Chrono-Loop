#pragma once
#include "Particles\ParticleSystem.h"

namespace Epoch
{
	//Don't use these atm
	void CreateTeleportEffect(vec4f _pos)
	{
		Particle * p = &Particle::Init();
		p->SetColors(vec4f(.2, .2, 1, 0), vec4f(0, 1, .2, 0));
		p->SetLife(200);
		p->SetSize(1.25 / 2.0, .15 / 2.0);
		p->SetPos(vec4f());
		TeleportEffect* emit = new TeleportEffect(-1, 250, 2, _pos);
		emit->SetTexture("../Resources/BasicRectP.png");
		emit->SetParticle(p);
		emit->y1 = 8;
		emit->y2 = 12;
		ParticleSystem::Instance()->AddEmitter(emit);

		p = &Particle::Init();
		p->SetColors(vec4f(.5, 0, .25, 0), vec4f(.2, .8, .5, 0));
		p->SetLife(1000);
		p->SetSize(.25 / 2.0, .05 / 2.0);
		p->SetPos(vec4f());
		emit = new TeleportEffect(-1, 150, 1, _pos);
		emit->SetTexture("../Resources/BasicCircleP.png");
		emit->SetParticle(p);
		emit->y1 = 1;
		emit->y2 = 5;
		ParticleSystem::Instance()->AddEmitter(emit);

	}

	void CreateCloneDeathEffect(vec4f _pos)
	{
		Particle * p = &Particle::Init();
		p->SetColors(vec4f(1, 1, 1, 1), vec4f());
		p->SetLife(550);
		p->SetSize(.25f, .15f);
		vec3f EPos = vec3f(_pos.x, _pos.y, _pos.z);
		//vec3f EPos = vec3f(0,0,0);
		ParticleEmitter *emit = new ParticleEmitter(600, 200, 20, EPos);
		emit->SetParticle(p);
		emit->SetTexture("../Resources/BasicCircleP.png");
		ParticleSystem::Instance()->AddEmitter(emit);
		emit->FIRE();
	}

}