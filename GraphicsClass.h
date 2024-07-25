#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.5f;

class D3DClass;
class CameraClass;
class ModelClass;
class LightShaderClass;
class depthShaderClass;
class shadowShaderClass;
class EnvironmentShaderClass;
class LightClass;
class RenderTextureClass;
class DebugWindowClass;
class TextureShaderClass;
class TextureClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Render();
	bool RTT();
	bool shadowMapping();
	bool depthMapRender();
	bool shadowRender();

	bool EnvironmentMappingRender();
	bool EnvironmentRender();

	void CameraMove(float dx, float dy, float dz);
	void CameraRotate(float dx, float dy, float dz);

private:
	bool RenderToTexture();
	bool RenderScene();

	bool TextureToRender(); 
	bool debugWindowRender(ID3D11ShaderResourceView*, int , int);

public:
	XMMATRIX LVP, NDCToWolrdMatrix;

private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;

	ModelClass* m_Model = nullptr;
	ModelClass* m_Plane = nullptr;
	ModelClass* m_sphere = nullptr;
	ModelClass* m_underPlane = nullptr;
	ModelClass* m_sphereMap = nullptr;

	TextureClass* m_modelTexture = nullptr;

	LightShaderClass* m_LightShader = nullptr;
	depthShaderClass* m_depthShader = nullptr;
	shadowShaderClass* m_shadowShader = nullptr;
	EnvironmentShaderClass* m_EnvironmentShader = nullptr;

	LightClass* m_Light = nullptr;
	RenderTextureClass* m_RenderTexture = nullptr;
	DebugWindowClass* m_DebugWindow = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
};