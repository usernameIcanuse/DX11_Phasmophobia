#ifndef _ZFRUSTUM_H_
#define _ZFRUSTUM_H_

#include "Base.h"

/// 프러스텀에 정확하게 포함되지 않더라도, 약간의 여분을 주어서 프러스텀에 포함시키기 위한 값
#define PLANE_EPSILON	5.0f

/**
 * 프러스텀 컬링을 하기위한 클래스
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