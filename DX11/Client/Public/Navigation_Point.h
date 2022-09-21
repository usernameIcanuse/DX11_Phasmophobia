#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)

class CNavigation_Point final : public CGameObject
{
protected:
	CNavigation_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation_Point(const CNavigation_Point& rhs);
	virtual ~CNavigation_Point() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


protected:
	CShader*    m_pShaderCom = nullptr;
	CRenderer*  m_pRendererCom = nullptr;

	CVIBuffer_Point_Instance* m_pVIBuffer = nullptr;

protected:
	virtual	HRESULT	Setup_Component();
	virtual HRESULT SetUp_ShaderResource()PURE;

public:
	static  CNavigation_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END