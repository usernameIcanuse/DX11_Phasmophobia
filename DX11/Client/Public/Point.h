#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)

class CPoint final : public CGameObject
{
private:
	CPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPoint(const CPoint& rhs);
	virtual ~CPoint() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;		
	CVIBuffer_Point_Instance*			m_pVIBufferCom = nullptr;


private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END