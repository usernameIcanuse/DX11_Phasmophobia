#pragma once

#include "Base.h"
#include "Light.h"
BEGIN(Engine)

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	LIGHTDESC* Get_LightDesc(_uint iIndex);

public:
	HRESULT Add_Light(CLight* pLight);
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

	
	HRESULT Clear_Light();
private:
	list<CLight*>				m_Lights;
	typedef list<CLight*>		LIGHTS;

public:
	virtual void Free() override;
};

END