#include "stdafx.h"
#include "depthShaderClass.h"


bool depthShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	// ���� �� �ȼ� ���̴��� �ʱ�ȭ�մϴ�.
	return InitializDeptheShader(device, hwnd, L"./depth_VS.hlsl", L"./depth_PS.hlsl");
}

bool depthShaderClass::depthRender(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX LVP, ID3D11ShaderResourceView* texture)
{
	// �������� ����� ���̴� �Ű� ������ �����մϴ�.
	if (!SetDepthShaderParameters(deviceContext, LVP, texture))
	{
		return false;
	}

	// ������ ���۸� ���̴��� �������Ѵ�.
	RenderDepthShader(deviceContext, indexCount);

	return true;
}

void depthShaderClass::Shutdown()
{
	// ���ؽ� �� �ȼ� ���̴��� ���õ� ��ü�� �����մϴ�.
	ShutdownShader();
}

bool depthShaderClass::InitializDeptheShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;

	// ���ؽ� ���̴� �ڵ带 �������Ѵ�.
	ID3D10Blob* vertexShaderBuffer = nullptr;
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "main", "vs_5_0", dwShaderFlags, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// ���̴� ������ ���н� �����޽����� ����մϴ�.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// ������ ������ �ƴ϶�� ���̴� ������ ã�� �� ���� ����Դϴ�.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// �ȼ� ���̴� �ڵ带 �������Ѵ�.
	ID3D10Blob* pixelShaderBuffer = nullptr;
	result = D3DCompileFromFile(psFilename, NULL, NULL, "main", "ps_5_0", dwShaderFlags, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// ���̴� ������ ���н� �����޽����� ����մϴ�.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// ������ ������ �ƴ϶�� ���̴� ������ ã�� �� ���� ����Դϴ�.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// ���۷κ��� ���� ���̴��� �����Ѵ�.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_depthShader_VS);
	if (FAILED(result))
	{
		return false;
	}

	// ���ۿ��� �ȼ� ���̴��� �����մϴ�.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_depthShader_PS);
	if (FAILED(result))
	{
		return false;
	}

	// ���� �Է� ���̾ƿ� ����ü�� �����մϴ�.
	// �� ������ ModelClass�� ���̴��� VertexType ������ ��ġ�ؾ��մϴ�.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	// ���̾ƿ��� ��� ���� �����ɴϴ�.
	UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// ���� �Է� ���̾ƿ��� ����ϴ�.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// �� �̻� ������ �ʴ� ���� ���̴� �۹��� �ȼ� ���̴� ���۸� �����մϴ�.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// �ؽ�ó ���÷� ���� ����ü�� ���� �� �����մϴ�.
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// �ؽ�ó ���÷� ���¸� ����ϴ�.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	// ���� ���̴��� �ִ� ��� ��� ������ ����ü�� �ۼ��մϴ�.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(depthMatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// ��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ��� ���ۿ� ������ �� �ְ� �մϴ�.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �ȼ� ���̴����ִ� ���� ���� ��� ������ ������ �����մϴ�.
	// D3D11_BIND_CONSTANT_BUFFER�� ����ϸ� ByteWidth�� �׻� 16�� ��� �����ϸ� �׷��� ������ CreateBuffer�� �����մϴ�.
	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// �� Ŭ���� ������ ���� ���̴� ��� ���ۿ� �׼��� �� �� �ֵ��� ��� ���� �����͸� ����ϴ�.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


bool depthShaderClass::SetDepthShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX LVP, ID3D11ShaderResourceView* texture)
{
	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	// ��� ������ ������ �� �� �ֵ��� ��޴ϴ�.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	// ��� ������ �����Ϳ� ���� �����͸� �����ɴϴ�.
	depthMatrixBufferType* dataPtr = (depthMatrixBufferType*)mappedResource.pData;

	// ��� ���ۿ� ����� �����մϴ�.

	LVP = XMMatrixTranspose(LVP);

	dataPtr->LVP = LVP;

	// ��� ������ ����� Ǳ�ϴ�.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// ���� ���̴������� ��� ������ ��ġ�� �����մϴ�.
	unsigned int bufferNumber = 0;

	// ���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲߴϴ�.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// light constant buffer�� ��� �� �ֵ��� ����Ѵ�.
	if (FAILED(deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	// ��� ������ ����� �����մϴ�.
	deviceContext->Unmap(m_lightBuffer, 0);

	// �ȼ� ���̴����� ���� ��� ������ ��ġ�� ??�����մϴ�.
	bufferNumber = 0;

	// ���������� ������Ʈ �� ������ �ȼ� ���̴����� ���� ��� ���۸� �����մϴ�.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);
	return true;
}


void depthShaderClass::RenderDepthShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// ���� �Է� ���̾ƿ��� �����մϴ�.
	deviceContext->IASetInputLayout(m_layout);

	// �ﰢ���� �׸� ���� ���̴��� �ȼ� ���̴��� �����մϴ�.
	deviceContext->VSSetShader(m_depthShader_VS, NULL, 0);
	deviceContext->PSSetShader(m_depthShader_PS, NULL, 0);

	// �ȼ� ���̴����� ���÷� ���¸� �����մϴ�.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// �ﰢ���� �׸��ϴ�.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}


void depthShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	// ���� �޽����� ���â�� ǥ���մϴ�.
	OutputDebugStringA(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));

	// ���� �޼����� ��ȯ�մϴ�.
	errorMessage->Release();
	errorMessage = 0;

	// ������ ������ ������ �˾� �޼����� �˷��ݴϴ�.
	MessageBox(hwnd, L"Error compiling shader.", shaderFilename, MB_OK);
}

void depthShaderClass::ShutdownShader()
{
	// ���� ��� ���۸� �����մϴ�.
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// ��� ��� ���۸� �����մϴ�.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// ���÷� ���¸� �����Ѵ�.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// ���̾ƿ��� �����մϴ�.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_depthShader_VS)
	{
		m_depthShader_VS->Release();
		m_depthShader_VS = 0;
	}

	if (m_depthShader_PS)
	{
		m_depthShader_PS->Release();
		m_depthShader_PS = 0;
	}
}