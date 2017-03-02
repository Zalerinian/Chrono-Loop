#include "ParticleSystem.h"
#include "Rendering\renderer.h"
#include "Core\TimeManager.h"
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

void ParticleSystem::SetGeometryShader()
{
	char* buffer;
	int bytes = 0;
	FileIO::LoadBytes("GeometryShader.cso", &buffer, bytes);

	(*RenderEngine::Renderer::Instance()->iGetDevice())->CreateGeometryShader(buffer, bytes, nullptr, &mGeometryShader);
}
void ParticleSystem::SetVertexPixelShader()
{
	char* buffer;
	int bytes = 0;

	FileIO::LoadBytes("ParticleVertexShader.cso", &buffer, bytes);
	(*RenderEngine::Renderer::Instance()->iGetDevice())->CreateVertexShader(buffer, bytes, nullptr, &mVShader);

	FileIO::LoadBytes("ParticlePixelShader.cso", &buffer, bytes);
	(*RenderEngine::Renderer::Instance()->iGetDevice())->CreatePixelShader(buffer, bytes, nullptr, &mPShader);

	CD3D11_BUFFER_DESC desc(sizeof(GSMatrix), D3D11_BIND_CONSTANT_BUFFER);
	(*RenderEngine::Renderer::Instance()->iGetDevice())->CreateBuffer(&desc, NULL, &mVBuff);
}

void ParticleSystem::SetMatrices(matrix4 _lmodel, matrix4 _lview, matrix4 _lproj, matrix4 _rmodel, matrix4 _rview, matrix4 _rproj)
{
	mLeft.model = _lmodel;
	mLeft.view = _lview;
	mLeft.proj = _lproj;

	mRight.model = _rmodel;
	mRight.view = _rview;
	mRight.proj = _rproj;
}

void ParticleSystem::Update()
{
	//TODO: Update emitters
	//float dt = TimeManager::Instance()->GetDeltaTime();
	for (ParticleEmitter* emit : mPEmitters)
		emit->Update(0);
	Render();
}

void ParticleSystem::Render()
{
	//TODO: Set up and render
	auto cntxt = (*RenderEngine::Renderer::Instance()->iGetContext());
	//Do for both eyes?

	cntxt->VSSetShader(mVShader, NULL, 0);
	ID3D11Buffer* buff = mVBuff;
	cntxt->VSSetConstantBuffers(0, 1, &buff);
	cntxt->GSSetShader(mGeometryShader, NULL, 0);
	cntxt->PSSetShader(mPShader, NULL, 0);
	for (int eye = 0; eye < 2; eye++)
	{
		if (eye == 0)
			cntxt->UpdateSubresource(mVBuff, 0, NULL, &mLeft, 0, 0);
		else
			cntxt->UpdateSubresource(mVBuff, 0, NULL, &mRight, 0, 0);

		for (int i = 0; i < mPEmitters.size(); i++)
		{
			//Set vbuffer and gbuffer
			UINT stride = sizeof(GSParticle);
			UINT offset = 0;
			ID3D11Buffer* buffer = mPEmitters[i]->GetVertexBuffer();
			cntxt->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
			ID3D11ShaderResourceView* srv = mPEmitters[i]->GetTexture();
			cntxt->PSSetShaderResources(0, 1, &srv);
			//draw call
			cntxt->Draw(mPEmitters.size(), 0);
			//Unset gbuffer
		}
	}
	cntxt->GSSetShader(NULL, NULL, 0);
	cntxt->VSSetShader(NULL, NULL, 0);
	cntxt->PSSetShader(NULL, NULL, 0);
	cntxt->VSSetConstantBuffers(0, 0, NULL);

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