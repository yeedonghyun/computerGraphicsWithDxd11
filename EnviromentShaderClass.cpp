#include "stdafx.h"
#include "EnvironmentShaderClass.h"

bool EnvironmentShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	// ���� �� �ȼ� ���̴��� �ʱ�ȭ�մϴ�.
	return InitializEnvironmentShader(device, hwnd, L"./shadowEnvironment_VS.hlsl", L"./shadowEnvironment_PS.hlsl");
}

void EnvironmentShaderClass::Shutdown()
{
	// ���ؽ� �� �ȼ� ���̴��� ���õ� ��ü�� �����մϴ�.
	ShutdownShader();
}

bool EnvironmentShaderClass::EnvironmentRender(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX MVP, XMMATRIX worldMatrix, XMMATRIX LVP, XMMATRIX NDCToWolrdMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* shpereMap, ID3D11ShaderResourceView* depthMap, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT3 CameraPosition)
{
	if (!SetEnvironmentShaderParameters(deviceContext, MVP, worldMatrix, LVP, NDCToWolrdMatrix, texture, shpereMap, depthMap, lightDirection, diffuseColor, CameraPosition))
	{
		return false;
	}

	RenderEnvironmentShader(deviceContext, indexCount);

	return true;
}

bool EnvironmentShaderClass::InitializEnvironmentShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

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
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_EnvironmentShader_VS);
	if (FAILED(result))
	{
		return false;
	}

	// ���ۿ��� �ȼ� ���̴��� �����մϴ�.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_EnvironmentShader_PS);
	if (FAILED(result))
	{
		return false;
	}

	// ���� �Է� ���̾ƿ� ����ü�� �����մϴ�.
	// �� ������ ModelClass�� ���̴��� VertexType ������ ��ġ�ؾ��մϴ�.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

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
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;

	//samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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
	D3D11_BUFFER_DESC EnvironmentBufferDesc;
	EnvironmentBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	EnvironmentBufferDesc.ByteWidth = sizeof(EnvironmentBufferType);
	EnvironmentBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	EnvironmentBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	EnvironmentBufferDesc.MiscFlags = 0;
	EnvironmentBufferDesc.StructureByteStride = 0;

	// ��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ��� ���ۿ� ������ �� �ְ� �մϴ�.
	result = device->CreateBuffer(&EnvironmentBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �ȼ� ���̴����ִ� ���� ���� ��� ������ ������ �����մϴ�.
	// D3D11_BIND_CONSTANT_BUFFER�� ����ϸ� ByteWidth�� �׻� 16�� ��� �����ϸ� �׷��� ������ CreateBuffer�� �����մϴ�.
	D3D11_BUFFER_DESC shadowShaderBufferDesc;
	shadowShaderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	shadowShaderBufferDesc.ByteWidth = sizeof(shadowShaderBufferType);
	shadowShaderBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shadowShaderBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	shadowShaderBufferDesc.MiscFlags = 0;
	shadowShaderBufferDesc.StructureByteStride = 0;

	// �� Ŭ���� ������ ���� ���̴� ��� ���ۿ� �׼��� �� �� �ֵ��� ��� ���� �����͸� ����ϴ�.
	result = device->CreateBuffer(&shadowShaderBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void EnvironmentShaderClass::ShutdownShader()
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

	if (m_EnvironmentShader_VS)
	{
		m_EnvironmentShader_VS->Release();
		m_EnvironmentShader_VS = 0;
	}

	if (m_EnvironmentShader_PS)
	{
		m_EnvironmentShader_PS->Release();
		m_EnvironmentShader_PS = 0;
	}
}

void EnvironmentShaderClass::RenderEnvironmentShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// ���� �Է� ���̾ƿ��� �����մϴ�.
	deviceContext->IASetInputLayout(m_layout);

	// �ﰢ���� �׸� ���� ���̴��� �ȼ� ���̴��� �����մϴ�.
	deviceContext->VSSetShader(m_EnvironmentShader_VS, NULL, 0);
	deviceContext->PSSetShader(m_EnvironmentShader_PS, NULL, 0);

	// �ȼ� ���̴����� ���÷� ���¸� �����մϴ�.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// �ﰢ���� �׸��ϴ�.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

void EnvironmentShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	// ���� �޽����� ���â�� ǥ���մϴ�.
	OutputDebugStringA(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));

	// ���� �޼����� ��ȯ�մϴ�.
	errorMessage->Release();
	errorMessage = 0;

	// ������ ������ ������ �˾� �޼����� �˷��ݴϴ�.
	MessageBox(hwnd, L"Error compiling shader.", shaderFilename, MB_OK);
}

bool EnvironmentShaderClass::SetEnvironmentShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX MVP, XMMATRIX worldMatrix, XMMATRIX LVP, XMMATRIX NDCToWolrdMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* sphereMap, ID3D11ShaderResourceView* depthMap, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT3 cameraPosition)
{
	deviceContext->PSSetShaderResources(0, 2, &texture);
	deviceContext->PSSetShaderResources(1, 2, &sphereMap);
	deviceContext->PSSetShaderResources(2, 2, &depthMap);


	// ��� ������ ������ �� �� �ֵ��� ��޴ϴ�.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	// ��� ������ �����Ϳ� ���� �����͸� �����ɴϴ�.
	EnvironmentBufferType* dataPtr = (EnvironmentBufferType*)mappedResource.pData;

	MVP = XMMatrixTranspose(MVP);
	worldMatrix = XMMatrixTranspose(worldMatrix);
	LVP = XMMatrixTranspose(LVP);
	NDCToWolrdMatrix = XMMatrixTranspose(NDCToWolrdMatrix);

	dataPtr->cameraPosition = cameraPosition;
	dataPtr->world = worldMatrix;
	dataPtr->MVP = MVP;

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

	// ��� ������ �����Ϳ� ���� �����͸� �����ɴϴ�.
	shadowShaderBufferType* dataPtr2 = (shadowShaderBufferType*)mappedResource.pData;

	// ���� ������ ��� ���ۿ� �����մϴ�.
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->LVP = LVP;
	dataPtr2->NDCToWolrdMatrix = NDCToWolrdMatrix;

	// ��� ������ ����� �����մϴ�.
	deviceContext->Unmap(m_lightBuffer, 0);

	// �ȼ� ���̴����� ���� ��� ������ ��ġ�� ??�����մϴ�.
	bufferNumber = 0;

	// ���������� ������Ʈ �� ������ �ȼ� ���̴����� ���� ��� ���۸� �����մϴ�.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);
	return true;
}
