#pragma once
class EnvironmentShaderClass
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

	bool Initialize(ID3D11Device*, HWND);

	void Shutdown();
	bool EnvironmentRender(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, XMFLOAT3);

private:
	bool InitializEnvironmentShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	bool SetEnvironmentShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, XMFLOAT3);
	void RenderEnvironmentShader(ID3D11DeviceContext*, int);

	void ShutdownShader();
private:
	ID3D11VertexShader* m_EnvironmentShader_VS = nullptr;
	ID3D11PixelShader* m_EnvironmentShader_PS = nullptr;

	ID3D11InputLayout* m_layout = nullptr;
	ID3D11SamplerState* m_sampleState = nullptr;

	ID3D11Buffer* m_matrixBuffer = nullptr;
	ID3D11Buffer* m_lightBuffer = nullptr;
};