#pragma once
#define NumofLights 1
#pragma comment (lib, "d3D11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include <vector>

#define VR 1
#define VRDEBUG 0
#if VR
#include <openvr.h>
#endif // VR

class Mesh;
//struct Interpolator
//{
//	unsigned int PreviousFrame;
//	unsigned int nextFrame;
//	float frameTime = 0.0f;
//	Bone* bonePtr;
//	KeyFrame* betweenKeyFrame;
//	KeyFrame* Interpolate(KeyFrame* _prev, KeyFrame* _next, float _deltaTime)
//	{
//
//		XMVECTOR cScal, cRot, cPos;
//		XMVECTOR nScal, nRot, nPos;
//		XMMatrixDecompose(&cScal, &cRot, &cPos, XMLoadFloat4x4(&_prev->transform));
//		XMMatrixDecompose(&nScal, &nRot, &nPos, XMLoadFloat4x4(&_next->transform));
//		//DO THIS IN THE INITIALZE 
//		//**DO NOT LOOP THIS EVERYTIME**//
//
//		XMVECTOR rotNow, scalNow, posNow;
//		XMMATRIX MatrixNow;
//		rotNow = XMQuaternionSlerp(cRot, nRot, _deltaTime);
//		posNow = XMVectorLerp(cPos, nPos, _deltaTime);
//		scalNow.m128_f32[0] = 1.0f;
//		scalNow.m128_f32[1] = 1.0f;
//		scalNow.m128_f32[2] = 1.0f;
//		scalNow.m128_f32[3] = 1.0f;
//		MatrixNow = XMMatrixAffineTransformation(scalNow, XMVectorZero(), rotNow, posNow);
//		KeyFrame* newFrame = new KeyFrame();
//		XMStoreFloat4x4(&newFrame->transform, MatrixNow);
//		return newFrame;
//	}
//	void Process(float _timeToAdd)
//	{
//
//		frameTime += _timeToAdd;
//		while (frameTime > bonePtr->keyframes->at(PreviousFrame)->keyTime) {
//			frameTime -= bonePtr->keyframes->at(PreviousFrame)->keyTime;
//			if (nextFrame == bonePtr->keyframes->size())
//				break;
//			PreviousFrame = nextFrame + 1;
//		}
//		float delta = frameTime / bonePtr->keyframes->at(PreviousFrame)->keyTime;
//		if (nextFrame == bonePtr->keyframes->size())
//			nextFrame = 0;
//		betweenKeyFrame = Interpolate(bonePtr->keyframes->at(PreviousFrame), bonePtr->keyframes->at(nextFrame), delta);
//	}
//};
class Renderer
{
public:
	class Node
	{
		friend class Renderer;
	private:
		ID3D11VertexShader*			m_vertexShader;
		ID3D11InputLayout*			m_inputLayout;
		ID3D11PixelShader*			m_pixelShader;
		ID3D11Buffer*				m_vertexBuffer;
		ID3D11Buffer*				m_indexBuffer;
		ID3D11Buffer*				m_constantBuffer;
		ID3D11Buffer*				light_constantBuffer;
		ID3D11Buffer*				m_boneBuffer;
		unsigned int				m_indexCount;
		ID3D11Texture2D*			m_Texture = nullptr;
		ID3D11ShaderResourceView*	m_TextureSRV = nullptr;
		ID3D11SamplerState*			m_SamplerState;
		ModelViewProjectionConstantBuffer* m_constantBufferData;
		BoneConstantBuffer			m_boneData;
		DirectX::XMFLOAT4X4			m_matrix;
		std::vector<ID3D11Buffer*>	m_buffers;
		bool isWireFrame = false;
		bool m_render = true;
		XMFLOAT4 m_scale;
	public:
		Node(Mesh* _m, char* vertexShader = "SampleVertexShader.cso", char* pixelShader = "LightPixelShader.cso", bool pushBack = true);
		~Node();
		void Wireframe(bool toggle) { isWireFrame = toggle; };
		DirectX::XMFLOAT4X4& GetMatrix() { return m_matrix; }
		void AddBuffer(ID3D11Buffer* buffer) { m_buffers.push_back(buffer); };
		void SetConstantBuffer(ModelViewProjectionConstantBuffer* cb) { m_constantBufferData = cb; };
		void Translate(float x, float y, float z) { m_matrix._14 += x; m_matrix._24 += y; m_matrix._34 += z; };
		void Position(float x, float y, float z) { m_matrix._14 = x; m_matrix._24 = y; m_matrix._34 = z; };
		void Scale(float x, float y, float z) { m_matrix._11 *= x; m_matrix._22 *= y; m_matrix._33 *= z; m_scale = XMFLOAT4(x, y, z,1.0f); };
		void Rotate(float x, float y, float z);
		void ToggleRender() { m_render = !m_render; };
		void SetKeyframe(Animation *anim, int frame);
		void SetKeyframe1(Animation *anim, int frame);

	};
private:
	static Renderer* s_instance;
	float cnt = 0.0f;
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	D3D11_VIEWPORT m_viewport;
	LightSource Lights[NumofLights];
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
#if VR
	ID3D11Texture2D* m_leftEye;
	ID3D11RenderTargetView* m_leftTargetViewMap;
	ID3D11ShaderResourceView* m_leftShaderRecource;
	ID3D11Texture2D* m_rightEye;
	ID3D11RenderTargetView* m_rightTargetViewMap;
	ID3D11ShaderResourceView* m_rightShaderRecource;
	vr::IVRSystem* m_vrSystem;
#endif // VR
	ID3D11DepthStencilState* m_depthStencilState;
	Node* m_viewNode;
	Mesh* view;
	std::vector<Node*> m_nodes;

#if VR
	bool m_Initialize(int height, int width, HWND hwnd, bool vsync, int fps, bool fullscreen, float screenDepth, float screenNear, vr::IVRSystem* vrSys);
#endif // VR

	bool m_Initialize(int height, int width, HWND hwnd, bool vsync, int fps, bool fullscreen, float screenDepth, float screenNear);
	bool m_Render();
	void m_Shutdown();
	void m_BeginScene(XMFLOAT4 color) { m_BeginScene(color.x, color.y, color.z, color.w); };
	void m_BeginScene(float red, float green, float blue, float alpha);
	void m_EndScene();
	void m_Wireframe(bool toggle);
	void m_GetProjectionMatrix(XMMATRIX& projMatrix) { projMatrix = m_projectionMatrix; };
	void m_GetWorldMatrix(XMMATRIX& worldMatrix) { worldMatrix = m_worldMatrix; };
	void m_GetOrientationMatrix(XMMATRIX& oriMatrix) { oriMatrix = m_orthoMatrix; };
	bool m_CreateLight(int _index,XMFLOAT4 _pos,XMFLOAT4 _direction, XMFLOAT4 _color);

public:
	Renderer();
	~Renderer();

	//STATIC FUNCTIONS
	static Renderer& Instance();
	static bool ShaderLoader(const char * filename, std::vector<byte>& out);
	static bool Render();

	static XMMATRIX GetProjectionMatrix();
	static bool Initialize(int height, int width, HWND hwnd, bool vsync, int fps, bool fullscreen, float screenDepth, float screenNear);
#if VR
	static bool Initialize(int height, int width, HWND hwnd, bool vsync, int fps, bool fullscreen, float screenDepth, float screenNear, vr::IVRSystem* vrSys);
#endif // VR
	static void WireframeMode(bool toggle);
};

