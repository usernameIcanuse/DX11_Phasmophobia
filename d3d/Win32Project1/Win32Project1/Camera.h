#pragma once
class CCamera
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT};

	CCamera();
	CCamera(CameraType cameraType);
	~CCamera();

public:
	void strafe(float units);//��/�� �԰���
	void fly(float units);//���Ʒ� ����?
	void walk(float units);//�յ� �ȱ�

	void pitch(float angle);//���� ���ͷ� ȸ��( �� ���Ʒ� ȸ��_
	void yaw(float angle);//���� ���ͷ� ȸ��(�� �¿� ȸ��)
	void roll(float angle); // ���� ���ͷ� ȸ��(�� �����

	void getViewMatrix(D3DXMATRIX* V);
	void setCameraType(CameraType cameraType);
	void getPosition(D3DXVECTOR3* pos);
	void setPositino(D3DXVECTOR3* pos);
	void getRight(D3DXVECTOR3* right);
	void getUp(D3DXVECTOR3* up);
	void getLook(D3DXVECTOR3* look);

private:
	CameraType	m_eCameraType;
	D3DXVECTOR3 m_vRight;
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vPos;


};

