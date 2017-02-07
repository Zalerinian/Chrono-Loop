//#include "Code\Header\renderer.h"
#include "stdafx.h"
#include "renderer.h"
#include <fstream>


using namespace DirectX;

Renderer::Node::Node(Mesh * _m, char* vertexShader, char* pixelShader, bool pushback)
{
	m_vertexShader = nullptr;
	m_inputLayout = nullptr;
	m_pixelShader = nullptr;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_constantBuffer = nullptr;
	m_boneBuffer = nullptr;
	m_Texture = nullptr;
	m_TextureSRV = nullptr;
	m_SamplerState = nullptr;
	//Vertex Buffer
	Renderer::Instance().m_GetWorldMatrix((XMMATRIX&)m_matrix);
	auto device = Renderer::Instance().m_device;
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	auto verts = _m->GetVerts();
	vertexBufferData.pSysMem = verts;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc((UINT)_m->VertSize() * sizeof(AnimatedVert), D3D11_BIND_VERTEX_BUFFER);
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);
	//Index Buffer
	m_indexCount = (unsigned int)_m->VertSize();
	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = _m->GetIndicies();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc((UINT)_m->IndicieSize() * sizeof(unsigned short), D3D11_BIND_INDEX_BUFFER);
	result = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer);

	std::vector<byte> fileData;
	//Vertex Shader
	if (ShaderLoader(vertexShader, fileData)) {
		result = device->CreateVertexShader(
			&fileData[0],
			fileData.size(),
			nullptr,
			&m_vertexShader);
		//Input Layout
		const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		result = device->CreateInputLayout(
			vertexDesc,
			ARRAYSIZE(vertexDesc),
			&fileData[0],
			fileData.size(),
			&m_inputLayout);
	}
	std::vector<byte> PixelFileData;
	//Pixel Shader
	if (ShaderLoader(pixelShader, PixelFileData)) {
		result = device->CreatePixelShader(
			&PixelFileData[0],
			PixelFileData.size(),
			nullptr,
			&m_pixelShader);
	}
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	result = device->CreateBuffer(&constantBufferDesc, nullptr, &m_constantBuffer);

	CD3D11_BUFFER_DESC constantBoneBufferDesc(sizeof(BoneConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	result = device->CreateBuffer(&constantBoneBufferDesc, nullptr, &m_boneBuffer);

	CD3D11_BUFFER_DESC constantLightBufferDesc(sizeof(LightSource) * NumofLights, D3D11_BIND_CONSTANT_BUFFER);
	result = device->CreateBuffer(&constantLightBufferDesc, nullptr, &light_constantBuffer);

	AddBuffer(m_constantBuffer);
	AddBuffer(m_boneBuffer);
	AddBuffer(light_constantBuffer);
	if (_m->ImagePath())
		result = CreateDDSTextureFromFile(device, _m->ImagePath(), (ID3D11Resource**)&m_Texture, &m_TextureSRV);
	else
		result = CreateDDSTextureFromFile(device, L"Assets//Default.dds", (ID3D11Resource**)&m_Texture, &m_TextureSRV);

	D3D11_SAMPLER_DESC sDesc;
	memset(&sDesc, 0, sizeof(sDesc));
	sDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
	sDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
	sDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
	sDesc.MaxAnisotropy = 16;
	result = device->CreateSamplerState(&sDesc, &m_SamplerState);
	if (pushback)
		Renderer::Instance().m_nodes.push_back(this);
}

Renderer::Node::~Node()
{
	//delete this;
}

void Renderer::Node::Rotate(float x, float y, float z)
{
	XMMATRIX rotX = XMMatrixRotationX(x);
	XMMATRIX rotY = XMMatrixRotationY(y);
	XMMATRIX rotZ = XMMatrixRotationZ(z);
	XMMATRIX temp = XMLoadFloat4x4(&m_matrix);
	XMMATRIX result = XMMatrixMultiply(temp, rotY);
	//result = XMMatrixMultiply(result, rotZ);
	//result = XMMatrixMultiply(result, temp);
	XMStoreFloat4x4(&m_matrix, result);
}

//XMMATRIX& GetWorld(Bone b, int frame)
//{
//	XMMATRIX world = XMMatrixIdentity();
//	if (b.Parent)
//		world = XMMatrixInverse(nullptr, GetWorld(*b.Parent, frame)) * XMLoadFloat4x4(&b.keyframes[frame].transform);
//	else
//		world = XMLoadFloat4x4(&b.keyframes[frame].transform);
//	return world;
//}


void Renderer::Node::SetKeyframe(Animation * anim, int frame)
{
	if (frame == -1)
		for (int i = 0; i < MAX_BONES; i++)
			XMStoreFloat4x4(&m_boneData.BoneOffest[i], XMMatrixIdentity());
	else
	{
		for (int i = 0; i < anim->bones.size(); i++)
		{
			if (anim->bones[i]->keyframes->size() > frame)
			{
				XMFLOAT4X4 trans = anim->bones[i]->keyframes->at(frame)->transform;
				 XMStoreFloat4x4(&m_boneData.BoneOffest[i], XMMatrixInverse(nullptr, XMLoadFloat4x4(&anim->bones[i]->transform)) * XMLoadFloat4x4(&trans));
			}
			else
				XMStoreFloat4x4(&m_boneData.BoneOffest[i], XMMatrixIdentity());

		}
	}
}
void Renderer::Node::SetKeyframe1(Animation * anim, int frame)
{
	Interpolator interpThing;
	if (frame == -1)
		for (int i = 0; i < MAX_BONES; i++)
			XMStoreFloat4x4(&m_boneData.BoneOffest[i], XMMatrixIdentity());
	else
	{
		for (int i = 0; i < anim->bones.size(); i++)
		{
			if (anim->bones[i]->keyframes->size() > frame)
			{
				interpThing.PreviousFrame = frame;
				interpThing.nextFrame = frame + 1;
				interpThing.bonePtr = anim->bones[i];
				interpThing.Process(1.0f);

				XMFLOAT4X4 trans = interpThing.betweenKeyFrame->transform;
				XMStoreFloat4x4(&m_boneData.BoneOffest[i], XMMatrixInverse(nullptr, XMLoadFloat4x4(&anim->bones[i]->transform)) * XMLoadFloat4x4(&trans));
			}
			else
				XMStoreFloat4x4(&m_boneData.BoneOffest[i], XMMatrixIdentity());

		}
	}
}
Renderer* Renderer::s_instance;

Renderer::Renderer()
{
	m_swapChain = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
#if VR
	m_rightEye = nullptr;
	m_leftEye = nullptr;
#endif // VR
	m_depthStencilState = nullptr;
	m_depthStencilView = nullptr;
	m_rasterState = nullptr;
	m_viewNode = nullptr;
}


Renderer::~Renderer()
{
}

#if VR
bool Renderer::m_Initialize(int height, int width, HWND hwnd, bool vsync, int fps, bool fullscreen, float screenDepth, float screenNear, vr::IVRSystem * vrSys)
{
	this->m_vrSystem = vrSys;
	return this->m_Initialize(height, width, hwnd, vsync, fps, fullscreen, screenDepth, screenNear);
}
#endif // VR

bool Renderer::m_Initialize(int height, int width, HWND hwnd, bool vsync, int fps, bool fullscreen, float screenDepth, float screenNear)
{
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	//D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;



	m_vsync_enabled = vsync;

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
		return false;
	if (FAILED(factory->EnumAdapters(0, &adapter)))
		return false;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
		return false;
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
		return false;
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return false;
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList)))
		return false;

	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)width)
		{
			if (displayModeList[i].Height == (unsigned int)height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	if (FAILED(adapter->GetDesc(&adapterDesc)))
		return false;
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	error = wcstombs_s((size_t *)&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
		return false;

	delete[] displayModeList;
	displayModeList = 0;
	adapterOutput->Release();
	adapterOutput = 0;
	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	ZeroMemory(&swapDesc, sizeof(swapDesc));

	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (m_vsync_enabled)
	{
		swapDesc.BufferDesc.RefreshRate.Numerator = 1;
		swapDesc.BufferDesc.RefreshRate.Denominator = fps;
	}
	else
	{
		swapDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hwnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	if (fullscreen)
		swapDesc.Windowed = false;
	else
		swapDesc.Windowed = true;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = 0;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapDesc, &m_swapChain, &m_device, NULL, &m_deviceContext)))
		return false;

	if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr)))
		return false;

	if (FAILED(m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView)))
		return false;

	backBufferPtr->Release();
	backBufferPtr = 0;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	if (FAILED(m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer)))
		return false;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (FAILED(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState)))
		return false;

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView)))
		return false;

#if VR
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the texture
	if (FAILED(m_device->CreateTexture2D(&textureDesc, NULL, &m_rightEye)))
		return false;
	if (FAILED(m_device->CreateTexture2D(&textureDesc, NULL, &m_leftEye)))
		return false;

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(m_device->CreateRenderTargetView(m_rightEye, &renderTargetViewDesc, &m_rightTargetViewMap)))
		return false;
	if (FAILED(m_device->CreateRenderTargetView(m_leftEye, &renderTargetViewDesc, &m_leftTargetViewMap)))
		return false;

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	if (FAILED(m_device->CreateShaderResourceView(m_rightEye, &shaderResourceViewDesc, &m_rightShaderRecource)))
		return false;
	if (FAILED(m_device->CreateShaderResourceView(m_leftEye, &shaderResourceViewDesc, &m_leftShaderRecource)))
		return false;
#endif // VR


	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState)))
		return false;

	m_deviceContext->RSSetState(m_rasterState);

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	m_viewport = viewport;

	m_deviceContext->RSSetViewports(1, &m_viewport);

	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)width / (float)height;

	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	m_worldMatrix = XMMatrixIdentity();

	m_orthoMatrix = XMMatrixOrthographicLH((float)width, (float)height, screenNear, screenDepth);

	bool lightChecker = false;
	lightChecker = m_CreateLight(0, XMFLOAT4(1.0f, 3.0f, 2.0f, 0), XMFLOAT4(0, 1, 0.3f, 0), XMFLOAT4(1, 1, 0, 1));

	return true;
}

bool Renderer::m_Render()
{
	auto context = m_deviceContext;
	m_BeginScene(XMFLOAT4(0.7f, 0.7f, 1, 1));
#if VR
#if !DEBUG
	vr::VRCompositor()->CompositorBringToFront();
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
#endif // DEBUG
	for (int e = 0; e < 2; e++)
	{
		float color[4];
		color[0] = 0.7f;
		color[1] = 0.7f;
		color[2] = 1;
		color[3] = 1;
		context->OMSetRenderTargets(1, e == 0 ? &m_leftTargetViewMap : &m_rightTargetViewMap, m_depthStencilView);
		context->ClearRenderTargetView(e == 0 ? m_leftTargetViewMap : m_rightTargetViewMap, color);
#endif // VR
		for (int i = 0; i < m_nodes.size(); i++)
		{
			auto node = m_nodes[i];
			if (!node->m_render)
			{
				continue;
			}
			ModelViewProjectionConstantBuffer cb;
#if VR
			cb.model = node->GetMatrix();
			cb.view = node->m_constantBufferData->view;
#if !DEBUG
			vr::HmdMatrix34_t view = m_vrSystem->GetEyeToHeadTransform((vr::EVREye)e);
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					cb.view.m[j][k] = view.m[j][k];
				}
			}
			cb.projection = node->m_constantBufferData->projection;
#endif // DEBUG
#if DEBUG
#endif // !DEBUG
#endif // VR
#if !VR
			cb.model = node->GetMatrix();
			cb.view = node->m_constantBufferData->view;
			cb.projection = node->m_constantBufferData->projection;
#endif // !VR
			context->UpdateSubresource(node->m_constantBuffer, 0, NULL, &cb, 0, 0);
			context->UpdateSubresource(node->m_boneBuffer, 0, NULL, &node->m_boneData, 0, 0);
			context->UpdateSubresource(node->light_constantBuffer, 0, NULL, &Lights, 0, 0);
			UINT stride = sizeof(AnimatedVert);
			UINT offset = 0;
			//Input Assembler
			context->IASetVertexBuffers(0, 1, &node->m_vertexBuffer, &stride, &offset);
			context->IASetIndexBuffer(node->m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->IASetInputLayout(node->m_inputLayout);
			//Vertex Shader
			context->VSSetShader(node->m_vertexShader, nullptr, 0);
			context->VSSetConstantBuffers(0, 2, node->m_buffers.data());
			//Pixel Shader
			context->PSSetSamplers(0, 1, &node->m_SamplerState);
			context->PSSetShader(node->m_pixelShader, nullptr, 0);
			context->PSSetShaderResources(0, 1, &node->m_TextureSRV);

			context->PSSetConstantBuffers(0, 2, node->m_buffers.data());
			context->PSSetSamplers(0, 1, &node->m_SamplerState);
			//ViewPort
			context->RSSetViewports(1, &m_viewport);
			context->DrawIndexed(node->m_indexCount, 0, 0);
			if (i == 0)
			{
				context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
			}
		}
#if VR
	}
	ID3D11ShaderResourceView *thingamajig = nullptr;
	context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->PSSetShaderResources(0, 1, &thingamajig);
	context->PSSetShaderResources(1, 1, &thingamajig);
	context->PSSetShaderResources(2, 1, &thingamajig);
	context->PSSetShaderResources(3, 1, &thingamajig);
	context->PSSetShaderResources(4, 1, &thingamajig);
	ID3D11RenderTargetView *emptyrtv = nullptr;
	//context->OMSetRenderTargets(1, &emptyrtv, nullptr);
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	if (m_viewNode == nullptr)
	{
		view = new Mesh();
		view->MakeViewPlane();
		m_viewNode = new Node(view, "SampleVertexShader.cso", "SamplePixelShader.cso", false);
		view->SetFrame(-1, -1);
	}
	m_viewNode->m_TextureSRV = m_rightShaderRecource;
	ModelViewProjectionConstantBuffer cb;
	cb.model = m_viewNode->m_matrix;
	XMStoreFloat4x4(&cb.projection, XMMatrixIdentity());
	XMStoreFloat4x4(&cb.view, XMMatrixIdentity());

	context->UpdateSubresource(m_viewNode->m_constantBuffer, 0, NULL, &cb, 0, 0);
	context->UpdateSubresource(m_viewNode->m_boneBuffer, 0, NULL, &m_viewNode->m_boneData, 0, 0);
	context->UpdateSubresource(m_viewNode->light_constantBuffer, 0, NULL, &Lights, 0, 0);
	UINT stride = sizeof(AnimatedVert);
	UINT offset = 0;
	//Input Assembler
	context->IASetVertexBuffers(0, 1, &m_viewNode->m_vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_viewNode->m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(m_viewNode->m_inputLayout);
	//Vertex Shader
	context->VSSetShader(m_viewNode->m_vertexShader, nullptr, 0);
	context->VSSetConstantBuffers(0, 2, m_viewNode->m_buffers.data());
	//Pixel Shader
	context->PSSetSamplers(0, 1, &m_viewNode->m_SamplerState);
	context->PSSetShader(m_viewNode->m_pixelShader, nullptr, 0);
	context->PSSetShaderResources(0, 1, &m_viewNode->m_TextureSRV);

	context->PSSetConstantBuffers(0, 2, m_viewNode->m_buffers.data());
	context->PSSetSamplers(0, 1, &m_viewNode->m_SamplerState);
	//ViewPort
	context->RSSetViewports(1, &m_viewport);
	context->DrawIndexed(m_viewNode->m_indexCount, 0, 0);
#endif // VR

	//cnt += 0.01f;
	//if (cnt >= 360.1f)
	//	cnt = 0.0f;

	//XMStoreFloat4x4(&m_nodes[1]->GetMatrix(), XMMatrixTranspose(XMMatrixRotationY(cnt)));
	m_EndScene();

#if VR
	vr::Texture_t texL = { (void*)m_leftEye, vr::TextureType_DirectX , vr::ColorSpace_Gamma };
	vr::Texture_t texR = { (void*)m_rightEye, vr::TextureType_DirectX , vr::ColorSpace_Gamma };
#if !DEBUG
	vr::EVRCompositorError error = vr::VRCompositor()->Submit(vr::Eye_Left, &texL);
	error = vr::VRCompositor()->Submit(vr::Eye_Right, &texR);

#endif // DEBUG
#endif
	return true;
}

void Renderer::m_Shutdown()
{
	if (m_swapChain)
		m_swapChain->SetFullscreenState(false, NULL);
	if (m_rasterState)
		m_rasterState->Release();
	if (m_depthStencilView)
		m_depthStencilView->Release();
	if (m_depthStencilState)
		m_depthStencilState->Release();
	if (m_depthStencilBuffer)
		m_depthStencilBuffer->Release();
#if VR
	if (m_rightEye)
		m_rightEye->Release();
	if (m_leftEye)
		m_leftEye->Release();
	if (m_leftTargetViewMap)
		m_leftTargetViewMap->Release();
	if (m_leftShaderRecource)
		m_leftShaderRecource->Release();
#endif // VR
	if (m_renderTargetView)
		m_renderTargetView->Release();
	if (m_deviceContext)
		m_deviceContext->Release();
	if (m_device)
		m_device->Release();
	if (m_swapChain)
		m_swapChain->Release();
}

void Renderer::m_BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::m_EndScene()
{
	if (m_vsync_enabled)
		m_swapChain->Present(1, 0);
	else
		m_swapChain->Present(0, 0);
}

void Renderer::m_Wireframe(bool toggle)
{
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = toggle ? D3D11_CULL_NONE : D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = toggle ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState)))
		return;

	m_deviceContext->RSSetState(m_rasterState);
}

bool Renderer::m_CreateLight(int _index, XMFLOAT4 _pos, XMFLOAT4 _direction, XMFLOAT4 _color)
{
	if (_index < NumofLights) {
		Lights[_index].pos = _pos;
		Lights[_index].direction = _direction;
		Lights[_index].color = _color;
		return true;
	}
	return false;
}
Renderer & Renderer::Instance()
{
	if (!s_instance)
		s_instance = new Renderer();
	return *s_instance;
}

bool Renderer::ShaderLoader(const char * filename, std::vector<byte>& out)
{
	std::ifstream file;
	file.open(filename, std::ios_base::in | std::ios_base::binary);
	if (!file.is_open()) {
		char buffer[1024];
		strerror_s(buffer, errno);
		return false;
	}
	unsigned int length = 0;
	byte *bite = new byte;
	file.seekg(0, std::ios_base::end);
	length = (unsigned int)file.tellg();
	file.seekg(0, std::ios_base::beg);
	out.reserve(length);
	for (unsigned int i = 0; i < length; i++) {
		file.read((char*)bite, 1);
		out.push_back(*bite);
	}
	delete bite;
	return true;
}

bool Renderer::Render()
{
	if (!s_instance)
		return false;
	return s_instance->m_Render();
}

XMMATRIX Renderer::GetProjectionMatrix()
{
	if (!s_instance)
		return XMMatrixIdentity();
	return s_instance->m_projectionMatrix;
}

bool Renderer::Initialize(int height, int width, HWND hwnd, bool vsync, int fps, bool fullscreen, float screenDepth, float screenNear)
{
	if (!s_instance)
		return false;
	return s_instance->m_Initialize(height, width, hwnd, vsync, fps, fullscreen, screenDepth, screenNear);
}

#if VR
bool Renderer::Initialize(int height, int width, HWND hwnd, bool vsync, int fps, bool fullscreen, float screenDepth, float screenNear, vr::IVRSystem * vrSys)
{
	if (!s_instance)
		return false;
	return s_instance->m_Initialize(height, width, hwnd, vsync, fps, fullscreen, screenDepth, screenNear, vrSys);
}
#endif // VR

void Renderer::WireframeMode(bool toggle)
{
	if (!s_instance)
		return;
	s_instance->m_Wireframe(toggle);
}

