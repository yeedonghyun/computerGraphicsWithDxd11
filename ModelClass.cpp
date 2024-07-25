#include "stdafx.h"
#include "TextureClass.h"
#include "modelclass.h"

#include <fstream>
using namespace std;


ModelClass::ModelClass()
{
	m_indexCount = 0, m_vertexCount = 0;
	bHasNormal = false, bHasUV = false;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, char* modelFilename)
{
	// 정점 및 인덱스 버퍼를 초기화합니다.
	if (!InitializeBuffers(device, modelFilename))
	{
		return false;
	}

	// 이 모델의 텍스처를 로드합니다.
	return LoadTexture(device, deviceContext, textureFilename);
}


void ModelClass::Shutdown()
{
	// 모델 텍스쳐를 반환합니다.
	ReleaseTexture();

	// 버텍스 및 인덱스 버퍼를 종료합니다.
	ShutdownBuffers();

	// 모델 데이터 반환
	ReleaseModel();
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// 그리기를 준비하기 위해 그래픽 파이프 라인에 꼭지점과 인덱스 버퍼를 놓습니다.
	RenderBuffers(deviceContext);
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool ModelClass::InitializeBuffers(ID3D11Device* device, char* filename)
{
	roadObjFile(filename);

	// 정점 배열을 만듭니다.
	VertexType* vertices = new VertexType[m_indexCount];
	if (!vertices)
	{
		return false;
	}

	// 인덱스 배열을 만듭니다.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// 정점 배열과 인덱스 배열을 데이터로 읽어옵니다.
	for (int i = 0; i < nNumFace; i++) {
		vertices[i * 3].position = XMFLOAT3(vertex[face[i].m_vertex[0]]);
		vertices[i * 3].texture = XMFLOAT2(uv[face[i].m_uv[0]]);
		vertices[i * 3].normal = XMFLOAT3(normal[face[i].m_normal[0]]);

		vertices[i * 3 + 1].position = XMFLOAT3(vertex[face[i].m_vertex[1]]);
		vertices[i * 3 + 1].texture = XMFLOAT2(uv[face[i].m_uv[1]]);
		vertices[i * 3 + 1].normal = XMFLOAT3(normal[face[i].m_normal[1]]);

		vertices[i * 3 + 2].position = XMFLOAT3(vertex[face[i].m_vertex[2]]);
		vertices[i * 3 + 2].texture = XMFLOAT2(uv[face[i].m_uv[2]]);
		vertices[i * 3 + 2].normal = XMFLOAT3(normal[face[i].m_normal[2]]);
	}

	for (int i = 0; i < m_indexCount; i++) {
		indices[i] = i;
	}

	// 정적 정점 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_indexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 이제 정점 버퍼를 만듭니다.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// 정적 인덱스 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성합니다.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성합니다.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제합니다.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// 인덱스 버퍼를 해제합니다.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 정점 버퍼를 해제합니다.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// 정점 버퍼의 단위와 오프셋을 설정합니다.
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형으로 설정합니다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	// 텍스처 오브젝트를 생성한다.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// 텍스처 오브젝트를 초기화한다.
	return m_Texture->Initialize(device, deviceContext, filename);
}


void ModelClass::ReleaseTexture()
{
	// 텍스처 오브젝트를 릴리즈한다.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}


bool ModelClass::roadObjFile(char* pFileName)
{
	char text[256];

	int faceVertex[3], UV[3], nomal[3];

	vector<string>line;

	ifstream pFile(pFileName);

	if (!pFile)
		return false;

	while (!pFile.eof())
	{
		pFile.getline(text, 256);
		line.push_back(text);
	}

	for (unsigned int i = 0; i < line.size(); i++)
	{
		if (line[i][0] == 'v' && line[i][1] == ' ') {
			if (line[i][2] == ' ') {
				sscanf_s(line[i].c_str(), "v  %f  %f  %f", &vertex[m_vertexCount][0], &vertex[m_vertexCount][1], &vertex[m_vertexCount][2]);
			}
			else {
				sscanf_s(line[i].c_str(), "v %f %f %f", &vertex[m_vertexCount][0], &vertex[m_vertexCount][1], &vertex[m_vertexCount][2]);
			}
			m_vertexCount++;
		}

		else if (line[i][0] == 'v' && line[i][1] == 't') {
			if (line[i][2] == ' ') {
				sscanf_s(line[i].c_str(), "vt  %f  %f", &uv[nNumUV][0], &uv[nNumUV][1]);
			}
			else {
				sscanf_s(line[i].c_str(), "vt %f %f", &uv[nNumUV][0], &uv[nNumUV][1]);
			}
			nNumUV++;
			bHasUV = true;
		}

		else if (line[i][0] == 'v' && line[i][1] == 'n') {

			if (line[i][2] == ' ') {
				sscanf_s(line[i].c_str(), "vn  %f  %f  %f", &normal[nNumNormal][0], &normal[nNumNormal][1], &normal[nNumNormal][2]);
			}
			else {
				sscanf_s(line[i].c_str(), "vn %f %f %f", &normal[nNumNormal][0], &normal[nNumNormal][1], &normal[nNumNormal][2]);
			}
			nNumNormal++;
			bHasNormal = true;
		}

		else if (i < line.size() && line[i][0] == 'f') {

			unsigned int j = 0, numVertex = 0;

			for (; j < line[i].size(); j++) {
				if (line[i][j] == ' ') {
					numVertex++;
				}
			}

			if (line[i][j - 1] == ' ') {
				numVertex--;
			}

			face[nNumFace].m_nNumVertex = numVertex;
			int faceVerNum = 0;

			if (line[i][1] == ' ' && bHasUV && bHasNormal) {

				for (unsigned int j = 1; j < line[i].size(); j++) {
					if (line[i][j] == ' ') {
						sscanf_s(&line[i][j + 1], "%d/%d/%d", &faceVertex[faceVerNum], &UV[faceVerNum], &nomal[faceVerNum]);
						faceVerNum++;
					}
				}
				for (int j = 0; j < face[nNumFace].m_nNumVertex; j++) {
					face[nNumFace].m_vertex[j] = faceVertex[j] - 1;
					face[nNumFace].m_uv[j] = UV[j] - 1;
					face[nNumFace].m_normal[j] = nomal[j] - 1;
				}
			}

			else if (line[i][1] == ' ' && !bHasUV && bHasNormal) {
				for (unsigned int j = 1; j < line[i].size(); j++) {
					if (line[i][j] == ' ') {
						sscanf_s(&line[i][j + 1], "%d//%d", &faceVertex[faceVerNum], &nomal[faceVerNum]);
						faceVerNum++;
					}
				}
				for (int j = 0; j < face[nNumFace].m_nNumVertex; j++) {
					face[nNumFace].m_vertex[j] = faceVertex[j] - 1;
					face[nNumFace].m_normal[j] = nomal[j] - 1;
				}
			}

			else {
				for (unsigned int j = 1; j < line[i].size(); j++) {
					if (line[i][j] == ' ') {
						sscanf_s(&line[i][j + 1], "%d", &faceVertex[faceVerNum]);
						faceVerNum++;
					}
				}
				for (int j = 0; j < face[nNumFace].m_nNumVertex; j++) {
					face[nNumFace].m_vertex[j] = faceVertex[j] - 1;
				}
			}
			nNumFace++;
		}
	}
	pFile.close();

	m_indexCount = nNumFace * 3;

	return true;
}

void ModelClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}
}