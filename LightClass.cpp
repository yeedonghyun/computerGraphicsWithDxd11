#include "stdafx.h"
#include "LightClass.h"


LightClass::LightClass()
{
	at.x = 0.0f;
	at.y = 0.0f;
	at.z = 1.0f;
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetPosition(float x, float y, float z)
{
	m_position = XMFLOAT3(x, y, z);
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
}

XMFLOAT4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT3 LightClass::GetPosition()
{
	return m_position;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}


XMMATRIX LightClass::lightViewMatrix(XMFLOAT3 pos, XMFLOAT3 at)
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;

	positionVector = XMLoadFloat3(&pos);

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	
	upVector = XMLoadFloat3(&up);

	lookAtVector = XMLoadFloat3(&at);

	return XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}