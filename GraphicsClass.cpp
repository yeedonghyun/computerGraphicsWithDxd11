#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "depthShaderClass.h"
#include "shadowShaderClass.h"
#include "EnvironmentShaderClass.h"
#include "lightclass.h"
#include "rendertextureclass.h"
#include "debugwindowclass.h"
#include "textureshaderclass.h"
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	LVP = XMMatrixIdentity();
	NDCToWolrdMatrix = XMMatrixIdentity();
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
	LVP = XMMatrixIdentity();
	NDCToWolrdMatrix = XMMatrixIdentity();
}

GraphicsClass::~GraphicsClass()
{
}

void GraphicsClass::CameraMove(float dx, float dy, float dz)
{
	XMFLOAT3 pos = m_Camera->GetPosition();
	m_Camera->SetPosition(dx + pos.x, dy + pos.y, dz + pos.z);
}

void GraphicsClass::CameraRotate(float dx, float dy, float dz)
{
	XMFLOAT3 pos = m_Camera->GetRotation();
	m_Camera->SetRotation(dx + pos.x, dy + pos.y, dz + pos.z);
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Direct3D 객체 생성
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// m_Camera 객체 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 모델 객체 생성
	m_Model = new ModelClass;
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "./data/stone03.tga", "./data/Ai.obj"))
	{
		MessageBox(hwnd, L"Could not initialize the m_Model model object.", L"Error", MB_OK);
		return false;
	}

	m_Plane = new ModelClass;
	if (!m_Plane->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "./data/desk.tga", "./data/Plane.obj"))
	{
		MessageBox(hwnd, L"Could not initialize the m_Plane model object.", L"Error", MB_OK);
		return false;
	}

	m_sphere = new ModelClass;
	if (!m_sphere->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "./data/stone03.tga", "./data/bigTeapot.obj"))
	{
		MessageBox(hwnd, L"Could not initialize the m_sphere model object.", L"Error", MB_OK);
		return false;
	}

	m_underPlane = new ModelClass;
	if (!m_underPlane->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "./data/stone03.tga", "./data/plane1.obj"))
	{
		MessageBox(hwnd, L"Could not initialize the m_underPlane model object.", L"Error", MB_OK);
		return false;
	}

	m_sphereMap = new ModelClass;
	if (!m_sphereMap->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "./data/spheremap2.tga", "./data/smallsphere.obj"))
	{
		MessageBox(hwnd, L"Could not initialize the m_underPlane model object.", L"Error", MB_OK);
		return false;
	}

	// 라이트 쉐이더 객체를 만듭니다.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// 라이트 쉐이더 객체를 초기화합니다.
	if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	m_depthShader = new depthShaderClass;
	if (!m_depthShader)
	{
		return false;
	}
	if (!m_depthShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the depth shader object.", L"Error", MB_OK);
		return false;

	}

	m_shadowShader = new shadowShaderClass;
	if (!m_shadowShader)
	{
		return false;
	}
	if (!m_shadowShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the shadow shader object.", L"Error", MB_OK);
		return false;
	}

	m_EnvironmentShader = new EnvironmentShaderClass;
	if (!m_EnvironmentShader)
	{
		return false;
	}

	if (!m_EnvironmentShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the shadow shader object.", L"Error", MB_OK);
		return false;
	}

	// 조명 객체를 만듭니다.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Camera->SetPosition(0.f, 1.f, -5.f);
	m_Light->SetAmbientColor(1.f, 1.f, 1.f, 1.f); //주변광
	m_Light->SetDiffuseColor(1.f, 1.f, 1.f, 1.f); //색상
	m_Light->SetPosition(0.f, 3.f, -5.f); //빛의 방향

	// 렌더링 텍스처 객체를 생성한다.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// 렌더링 텍스처 객체를 초기화한다.
	if (!m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight))
	{
		return false;
	}

	// 디버그 창 객체를 만듭니다.
	m_DebugWindow = new DebugWindowClass;
	if (!m_DebugWindow)
	{
		return false;
	}

	// 디버그 창 객체를 초기화 합니다.
	if (!m_DebugWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, 128 * 2, 128 * 2))
	{
		MessageBox(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
		return false;
	}

	// 텍스처 쉐이더 객체를 생성한다.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// 텍스처 쉐이더 객체를 초기화한다.
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// 텍스처 쉐이더 객체를 해제한다.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// 디버그 창 객체를 해제합니다.
	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}

	// 렌더를 텍스쳐 객체로 릴리즈한다.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// 조명 객체를 해제한다.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// 라이트 쉐이더 객체를 해제합니다.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_depthShader)
	{
		m_depthShader->Shutdown();
		delete m_depthShader;
		m_depthShader = 0;
	}

	if (m_shadowShader)
	{
		m_shadowShader->Shutdown();
		delete m_shadowShader;
		m_shadowShader = 0;
	}

	if (m_EnvironmentShader)
	{
		m_EnvironmentShader->Shutdown();
		delete m_EnvironmentShader;
		m_EnvironmentShader = 0;
	}

	// 모델 객체 반환
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Plane)
	{
		m_Plane->Shutdown();
		delete m_Plane;
		m_Plane = 0;
	}

	if (m_sphere)
	{
		m_sphere->Shutdown();
		delete m_sphere;
		m_sphere = 0;
	}

	if (m_underPlane)
	{
		m_underPlane->Shutdown();
		delete m_underPlane;
		m_underPlane = 0;
	}

	if (m_sphereMap)
	{
		m_sphereMap->Shutdown();
		delete m_sphereMap;
		m_sphereMap = 0;
	}

	// m_Camera 객체 반환
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D 객체 반환
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}


bool GraphicsClass::RenderToTexture()
{
	// 렌더링 대상을 렌더링에 맞게 설정합니다.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	// 렌더링을 텍스처에 지웁니다.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 1.0f, 1.0f, 1.0f, 1.0f);

	// 이제 장면을 렌더링하면 백 버퍼 대신 텍스처로 렌더링됩니다.
	if (!RenderScene())
	{
		return false;
	}

	// 렌더링 대상을 원래의 백 버퍼로 다시 설정하고 렌더링에 대한 렌더링을 더 이상 다시 설정하지 않습니다.
	m_Direct3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderScene()
{

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// 각 프레임의 rotation 변수를 업데이트합니다.
	static float rotation = 0.0f;
	rotation += (float)XM_PI * 0.0025f;

	// 회전 값으로 월드 행렬을 회전합니다.
	worldMatrix = XMMatrixRotationY(rotation);

	// 모델 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 렌더링 합니다.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// 라이트 쉐이더를 사용하여 모델을 렌더링합니다.
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetPosition(), m_Light->GetDiffuseColor());

	return true;
}

bool GraphicsClass::TextureToRender()
{
	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// 각 프레임의 rotation 변수를 업데이트합니다.
	static float rotation = 0.0f;
	rotation += (float)XM_PI * 0.0025f;

	// 회전 값으로 월드 행렬을 회전합니다.
	worldMatrix = XMMatrixRotationY(rotation);

	// 모델 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 렌더링 합니다.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// 라이트 쉐이더를 사용하여 모델을 렌더링합니다.
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetPosition(), m_Light->GetDiffuseColor());

	static float rotation1 = 110;
	static float rotation2 = 3.141592653f;
	worldMatrix = XMMatrixRotationY(rotation1) * XMMatrixRotationZ(rotation2);
	// 모델 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 렌더링 합니다.
	m_Plane->Render(m_Direct3D->GetDeviceContext());

	// 라이트 쉐이더를 사용하여 모델을 렌더링합니다.
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Plane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_RenderTexture->GetShaderResourceView(), m_Light->GetPosition(), m_Light->GetDiffuseColor());

	return true;
}

bool GraphicsClass::debugWindowRender(ID3D11ShaderResourceView* texture, int x, int y) {

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	m_Direct3D->TurnZBufferOff();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	worldMatrix = XMMatrixIdentity();
	// 디버그 윈도우 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	if (!m_DebugWindow->Render(m_Direct3D->GetDeviceContext(), x, y))
	{
		return false;
	}

	//텍스처 셰이더를 사용해 디버그 윈도우를 렌더링한다.
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, texture))
	{
		return false;
	}

	// 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	m_Direct3D->TurnZBufferOn();

	return true;
}

bool GraphicsClass::depthMapRender() {

	XMMATRIX worldMatrix, lightViewMatrix, projectionMatrix, MVP;

	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 1.0f, 1.0f, 1.0f, 1.0f);

	m_Direct3D->GetWorldMatrix(worldMatrix);

	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	lightViewMatrix = m_Light->lightViewMatrix(m_Light->GetPosition(), m_Light->at);
	MVP = XMMatrixMultiply(worldMatrix, lightViewMatrix);
	MVP = XMMatrixMultiply(MVP, projectionMatrix);

	m_underPlane->Render(m_Direct3D->GetDeviceContext());
	if (!m_depthShader->depthRender(m_Direct3D->GetDeviceContext(), m_underPlane->GetIndexCount(), MVP, m_underPlane->GetTexture())) {
		return false;
	}

	static float rotation = 0.0f;
	rotation += (float)XM_PI * 0.0025f;

	worldMatrix = XMMatrixRotationY(rotation);
	MVP = XMMatrixMultiply(worldMatrix, lightViewMatrix);
	MVP = XMMatrixMultiply(MVP, projectionMatrix);

	m_sphere->Render(m_Direct3D->GetDeviceContext());
	if (!m_depthShader->depthRender(m_Direct3D->GetDeviceContext(), m_sphere->GetIndexCount(), MVP, m_sphere->GetTexture())) {
		return false;
	}

	m_Direct3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::shadowRender() {

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, MVP;
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(100.f, 100.f, 100.f));
	MVP = XMMatrixMultiply(worldMatrix, viewMatrix);
	MVP = XMMatrixMultiply(MVP, projectionMatrix);

	NDCToWolrdMatrix = XMMatrixMultiply(viewMatrix, projectionMatrix);
	NDCToWolrdMatrix = XMMatrixInverse(nullptr, NDCToWolrdMatrix);

	lightViewMatrix = m_Light->lightViewMatrix(m_Light->GetPosition(), m_Light->at);
	LVP = XMMatrixMultiply(lightViewMatrix, projectionMatrix);

	m_underPlane->Render(m_Direct3D->GetDeviceContext());
	if (!m_shadowShader->shadowRender(m_Direct3D->GetDeviceContext(), m_underPlane->GetIndexCount(), MVP, worldMatrix, LVP, NDCToWolrdMatrix,
		m_underPlane->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_Light->GetPosition(), m_Light->GetDiffuseColor())) {
		return false;
	}

	static float rotation = 0.0f;
	rotation += (float)XM_PI * 0.0025f;
	worldMatrix = XMMatrixRotationY(rotation);

	MVP = XMMatrixMultiply(worldMatrix, viewMatrix);
	MVP = XMMatrixMultiply(MVP, projectionMatrix);

	m_sphere->Render(m_Direct3D->GetDeviceContext());
	if (!m_shadowShader->shadowRender(m_Direct3D->GetDeviceContext(), m_sphere->GetIndexCount(), MVP, worldMatrix, LVP, NDCToWolrdMatrix,
		m_sphere->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_Light->GetPosition(), m_Light->GetDiffuseColor())) {
		return false;
	}

	return true;
}

bool GraphicsClass::EnvironmentRender() {

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, MVP;
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	MVP = XMMatrixMultiply(worldMatrix, viewMatrix);
	MVP = XMMatrixMultiply(MVP, projectionMatrix);

	NDCToWolrdMatrix = XMMatrixMultiply(viewMatrix, projectionMatrix);
	NDCToWolrdMatrix = XMMatrixInverse(nullptr, NDCToWolrdMatrix);

	lightViewMatrix = m_Light->lightViewMatrix(m_Light->GetPosition(), m_Light->at);
	LVP = XMMatrixMultiply(lightViewMatrix, projectionMatrix);

	m_underPlane->Render(m_Direct3D->GetDeviceContext());
	if (!m_shadowShader->shadowRender(m_Direct3D->GetDeviceContext(), m_underPlane->GetIndexCount(), MVP, worldMatrix, LVP, NDCToWolrdMatrix,
		m_underPlane->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_Light->GetPosition(), m_Light->GetDiffuseColor())) {
		return false;
	}

	static float rotation = 0.0f;
	rotation += (float)XM_PI * 0.0025f;
	worldMatrix = XMMatrixRotationY(rotation);

	MVP = XMMatrixMultiply(worldMatrix, viewMatrix);
	MVP = XMMatrixMultiply(MVP, projectionMatrix);

	m_sphere->Render(m_Direct3D->GetDeviceContext());
	if (!m_EnvironmentShader->EnvironmentRender(m_Direct3D->GetDeviceContext(), m_sphere->GetIndexCount(), MVP, worldMatrix, LVP, NDCToWolrdMatrix,
		m_sphere->GetTexture(), m_sphereMap->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_Light->GetPosition(), m_Light->GetDiffuseColor(), m_Camera->GetPosition())) {
		return false;
	}

	return true;
}

bool GraphicsClass::RTT() {

	m_Camera->GetPosition();
	m_Camera->viewMatrix();

	if (!RenderToTexture())
	{
		return false;
	}

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	if (!TextureToRender())
	{
		return false;
	}

	//debugWindowRender(m_RenderTexture->GetShaderResourceView(), 50, 50);

	m_Direct3D->EndScene();

	return true;
}

bool GraphicsClass::shadowMapping() {

	m_Camera->GetPosition();
	m_Camera->viewMatrix();

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	if (!depthMapRender()) {
		return false;
	}

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	if (!shadowRender()) {
		return false;
	}

	//debugWindowRender(m_RenderTexture->GetShaderResourceView(), 50, 50);

	m_Direct3D->EndScene();
	return true;
}

bool GraphicsClass::EnvironmentMappingRender() {

	m_Camera->GetPosition();
	m_Camera->viewMatrix();

	if (!depthMapRender()) {
		return false;
	}

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	if (!EnvironmentRender()) {
		return false;
	}

	debugWindowRender(m_sphereMap->GetTexture(), 50, 50);

	m_Direct3D->EndScene();
	return true;
}

bool GraphicsClass::Render()
{
	if (!EnvironmentMappingRender()) {
		return false;
	}

	/*if (!shadowMapping()) {
		return false;
	}*/

	/*if (!RTT()) {
		return false;
	}*/

	return true;
}