#pragma once

class CameraClass : public AlignedAllocationPolicy<16>
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetXMFLOAT3Position(XMFLOAT3);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void viewMatrix();
	void GetViewMatrix(XMMATRIX&);


private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMMATRIX m_viewMatrix;
};