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
	// ���� �� �ε��� ���۸� �ʱ�ȭ�մϴ�.
	if (!InitializeBuffers(device, modelFilename))
	{
		return false;
	}

	// �� ���� �ؽ�ó�� �ε��մϴ�.
	return LoadTexture(device, deviceContext, textureFilename);
}


void ModelClass::Shutdown()
{
	// �� �ؽ��ĸ� ��ȯ�մϴ�.
	ReleaseTexture();

	// ���ؽ� �� �ε��� ���۸� �����մϴ�.
	ShutdownBuffers();

	// �� ������ ��ȯ
	ReleaseModel();
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// �׸��⸦ �غ��ϱ� ���� �׷��� ������ ���ο� �������� �ε��� ���۸� �����ϴ�.
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

	// ���� �迭�� ����ϴ�.
	VertexType* vertices = new VertexType[m_indexCount];
	if (!vertices)
	{
		return false;
	}

	// �ε��� �迭�� ����ϴ�.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// ���� �迭�� �ε��� �迭�� �����ͷ� �о�ɴϴ�.
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

	// ���� ���� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_indexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���� ���۸� ����ϴ�.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// ���� �ε��� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� �����մϴ�.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� �����մϴ�.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// �ε��� ���۸� �����մϴ�.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// ���� ���۸� �����մϴ�.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// ���� ������ ������ �������� �����մϴ�.
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ �����մϴ�.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	// �ؽ�ó ������Ʈ�� �����Ѵ�.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�.
	return m_Texture->Initialize(device, deviceContext, filename);
}


void ModelClass::ReleaseTexture()
{
	// �ؽ�ó ������Ʈ�� �������Ѵ�.
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