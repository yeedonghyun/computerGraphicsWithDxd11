#pragma once

class depthShaderClass {

private:

	struct depthMatrixBufferType
	{
		XMMATRIX LVP;
	};

	struct LightBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};
public:
	bool Initialize(ID3D11Device*, HWND);
	bool depthRender(ID3D11DeviceContext*, int, XMMATRIX, ID3D11ShaderResourceView*);

	void Shutdown();

private:
	bool InitializDeptheShader(ID3D11Device*, HWND, WCHAR*, WCHAR*); bool SetDepthShaderParameters(ID3D11DeviceContext*, XMMATRIX, ID3D11ShaderResourceView*);
	void RenderDepthShader(ID3D11DeviceContext*, int);

	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

private:
	ID3D11VertexShader* m_depthShader_VS = nullptr;
	ID3D11PixelShader* m_depthShader_PS = nullptr;

	ID3D11InputLayout* m_layout = nullptr;
	ID3D11SamplerState* m_sampleState = nullptr;

	ID3D11Buffer* m_matrixBuffer = nullptr;
	ID3D11Buffer* m_lightBuffer = nullptr;
};