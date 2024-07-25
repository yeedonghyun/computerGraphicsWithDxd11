#pragma once

#include <stdio.h>
#include <vector>
#include <fstream>
#include <string.h>
using namespace std;

#define NUM_MAX_VERTEX_PER_FACE 3
#define NUM_MAX_VERTEX 10000
#define NUM_MAX_FACE 10000

class TextureClass;

class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	struct Face_t {
		int m_nNumVertex;
		int m_vertex[NUM_MAX_VERTEX_PER_FACE];
		int m_uv[NUM_MAX_VERTEX_PER_FACE];
		int m_normal[NUM_MAX_VERTEX_PER_FACE];
	};
public:
	bool bHasNormal, bHasUV;

	int m_vertexCount, m_indexCount;

	int	 nNumFace, nNumUV, nNumNormal, verNum;
	float vertex[NUM_MAX_VERTEX][3];
	float uv[NUM_MAX_VERTEX][2];
	float sortVertex[NUM_MAX_VERTEX][3];
	float normal[NUM_MAX_VERTEX][3];
	Face_t face[NUM_MAX_FACE];
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool ModelClass::Initialize(ID3D11Device* , ID3D11DeviceContext* , char* , char* );
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*, char*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device* , ID3D11DeviceContext* , char* );
	void ReleaseTexture();

	bool roadObjFile(char*);
	void ReleaseModel();

private:
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	TextureClass* m_Texture = nullptr;
	ModelType* m_model = nullptr;
};