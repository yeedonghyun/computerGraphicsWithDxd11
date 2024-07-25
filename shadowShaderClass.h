#pragma once
class shadowShaderClass {
#pragma once
private:

	struct shadowShaderBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;

		XMMATRIX LVP;
		XMMATRIX NDCToWolrdMatrix;
	};

	struct shadowMatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX MVP;
	};

public:

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown(); 
	bool shadowRender(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);
	bool InitializShadoweShader(ID3D11Device*, HWND, WCHAR*, WCHAR*); 
	bool SetshadowShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);

	void RendershadowShader(ID3D11DeviceContext*, int);

	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

private:
	ID3D11VertexShader* m_shadowShader_VS = nullptr;
	ID3D11PixelShader* m_shadowShader_PS = nullptr;

	ID3D11InputLayout* m_layout = nullptr;
	ID3D11SamplerState* m_sampleState = nullptr;

	ID3D11Buffer* m_matrixBuffer = nullptr;
	ID3D11Buffer* m_lightBuffer = nullptr;

};