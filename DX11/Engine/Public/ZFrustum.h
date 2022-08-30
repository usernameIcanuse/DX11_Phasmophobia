#ifndef _ZFRUSTUM_H_
#define _ZFRUSTUM_H_

#include "Base.h"

/// �������ҿ� ��Ȯ�ϰ� ���Ե��� �ʴ���, �ణ�� ������ �־ �������ҿ� ���Խ�Ű�� ���� ��
#define PLANE_EPSILON	5.0f

/**
 * �������� �ø��� �ϱ����� Ŭ����
 *
 */
class CZFrustum final : public CBase
{
	DECLARE_SINGLETON(CZFrustum)
public:
	CZFrustum();
	virtual ~CZFrustum() = default;

public:
	void	Update_Frustum();
	void	Make(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix);
	BOOL	CheckPoint(float x, float y, float z);
	BOOL	CheckCube(float, float, float, float);
	BOOL	CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	BOOL	CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);
	
private:
	_vector	m_planes[6];

public:
	virtual void Free() override;
};


#endif // _ZFRUSTUM_H_