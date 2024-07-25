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
	// Direct3D ��ü ����
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Direct3D ��ü �ʱ�ȭ
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// m_Camera ��ü ����
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// �� ��ü ����
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

	// ����Ʈ ���̴� ��ü�� ����ϴ�.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// ����Ʈ ���̴� ��ü�� �ʱ�ȭ�մϴ�.
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

	// ���� ��ü�� ����ϴ�.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Camera->SetPosition(0.f, 1.f, -5.f);
	m_Light->SetAmbientColor(1.f, 1.f, 1.f, 1.f); //�ֺ���
	m_Light->SetDiffuseColor(1.f, 1.f, 1.f, 1.f); //����
	m_Light->SetPosition(0.f, 3.f, -5.f); //���� ����

	// ������ �ؽ�ó ��ü�� �����Ѵ�.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// ������ �ؽ�ó ��ü�� �ʱ�ȭ�Ѵ�.
	if (!m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight))
	{
		return false;
	}

	// ����� â ��ü�� ����ϴ�.
	m_DebugWindow = new DebugWindowClass;
	if (!m_DebugWindow)
	{
		return false;
	}

	// ����� â ��ü�� �ʱ�ȭ �մϴ�.
	if (!m_DebugWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, 128 * 2, 128 * 2))
	{
		MessageBox(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
		return false;
	}

	// �ؽ�ó ���̴� ��ü�� �����Ѵ�.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// �ؽ�ó ���̴� ��ü�� �ʱ�ȭ�Ѵ�.
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// �ؽ�ó ���̴� ��ü�� �����Ѵ�.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// ����� â ��ü�� �����մϴ�.
	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}

	// ������ �ؽ��� ��ü�� �������Ѵ�.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// ���� ��ü�� �����Ѵ�.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// ����Ʈ ���̴� ��ü�� �����մϴ�.
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

	// �� ��ü ��ȯ
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

	// m_Camera ��ü ��ȯ
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D ��ü ��ȯ
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}


bool GraphicsClass::RenderToTexture()
{
	// ������ ����� �������� �°� �����մϴ�.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	// �������� �ؽ�ó�� ����ϴ�.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 1.0f, 1.0f, 1.0f, 1.0f);

	// ���� ����� �������ϸ� �� ���� ��� �ؽ�ó�� �������˴ϴ�.
	if (!RenderScene())
	{
		return false;
	}

	// ������ ����� ������ �� ���۷� �ٽ� �����ϰ� �������� ���� �������� �� �̻� �ٽ� �������� �ʽ��ϴ�.
	m_Direct3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderScene()
{

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// �� �������� rotation ������ ������Ʈ�մϴ�.
	static float rotation = 0.0f;
	rotation += (float)XM_PI * 0.0025f;

	// ȸ�� ������ ���� ����� ȸ���մϴ�.
	worldMatrix = XMMatrixRotationY(rotation);

	// �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ������ �մϴ�.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// ����Ʈ ���̴��� ����Ͽ� ���� �������մϴ�.
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetPosition(), m_Light->GetDiffuseColor());

	return true;
}

bool GraphicsClass::TextureToRender()
{
	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// �� �������� rotation ������ ������Ʈ�մϴ�.
	static float rotation = 0.0f;
	rotation += (float)XM_PI * 0.0025f;

	// ȸ�� ������ ���� ����� ȸ���մϴ�.
	worldMatrix = XMMatrixRotationY(rotation);

	// �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ������ �մϴ�.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// ����Ʈ ���̴��� ����Ͽ� ���� �������մϴ�.
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetPosition(), m_Light->GetDiffuseColor());

	static float rotation1 = 110;
	static float rotation2 = 3.141592653f;
	worldMatrix = XMMatrixRotationY(rotation1) * XMMatrixRotationZ(rotation2);
	// �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ������ �մϴ�.
	m_Plane->Render(m_Direct3D->GetDeviceContext());

	// ����Ʈ ���̴��� ����Ͽ� ���� �������մϴ�.
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Plane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_RenderTexture->GetShaderResourceView(), m_Light->GetPosition(), m_Light->GetDiffuseColor());

	return true;
}

bool GraphicsClass::debugWindowRender(ID3D11ShaderResourceView* texture, int x, int y) {

	// ��� 2D �������� �����Ϸ��� Z ���۸� ���ϴ�.
	m_Direct3D->TurnZBufferOff();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	worldMatrix = XMMatrixIdentity();
	// ����� ������ ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� �׸��⸦ �غ��մϴ�.
	if (!m_DebugWindow->Render(m_Direct3D->GetDeviceContext(), x, y))
	{
		return false;
	}

	//�ؽ�ó ���̴��� ����� ����� �����츦 �������Ѵ�.
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, texture))
	{
		return false;
	}

	// ��� 2D �������� �Ϸ�Ǿ����Ƿ� Z ���۸� �ٽ� �ѽʽÿ�.
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