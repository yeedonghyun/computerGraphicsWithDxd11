#pragma once

class LightShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct shadowMatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX MVP;
	};

	struct EnvironmentBufferType
	{
		XMFLOAT3 cameraPosition;
		XMMATRIX world;
		XMMATRIX MVP;
	};

	struct shadowShaderBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;

		XMMATRIX LVP;
		XMMATRIX NDCToWolrdMatrix;
	};

	struct LightBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};
public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(ID3D11Device*, HWND);

	void Shutdown();

	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);
	
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);

	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);

	void RenderShader(ID3D11DeviceContext*, int);
	
private:
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;

	ID3D11InputLayout* m_layout = nullptr;
	ID3D11SamplerState* m_sampleState = nullptr;

	ID3D11Buffer* m_matrixBuffer = nullptr;
	ID3D11Buffer* m_lightBuffer = nullptr;
};