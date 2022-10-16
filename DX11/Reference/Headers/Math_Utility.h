#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CMath_Utility final : public CBase
{
public:
	CMath_Utility();
	virtual ~CMath_Utility() = default;

public:
	static	bool		Picking(class CVIBuffer* pVIBuffer, class CTransform* pTransform, _float4* pOut);
	static  RAY			Get_MouseRayInWorldSpace();
	static  RAY			Get_MouseRayInWorldSpace(_float _fX, _float _fY);
};

END