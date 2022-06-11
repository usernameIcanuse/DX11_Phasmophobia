#pragma once
class CCamera
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT};

	CCamera();
	CCamera(CameraType cameraType);
	~CCamera();

public:
	void strafe(float units);//왼/오 게걸음
	void fly(float units);//위아래 날기?
	void walk(float units);//앞뒤 걷기

	void pitch(float angle);//우향 벡터로 회전( 고개 위아래 회전_
	void yaw(float angle);//상향 벡터로 회전(고개 좌우 회전)
	void roll(float angle); // 전방 벡터로 회전(고개 갸우뚱

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

