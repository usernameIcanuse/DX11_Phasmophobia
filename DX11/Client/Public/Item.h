#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CModel;
class CCollider;
END

BEGIN(Client)

class CItem : public CGameObject
{
protected:
	CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem(const CItem& rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	CShader*    m_pShaderCom = nullptr;
	CTexture*   m_pTextureCom = nullptr;
	CRenderer*  m_pRendererCom = nullptr;
	CModel*     m_pModelCom = nullptr;
	CCollider*  m_pAABBCom = nullptr;
protected:
	virtual	HRESULT	Setup_Component();
	virtual HRESULT SetUp_ShaderResource()PURE;

	//_bool	Picking(CVIBuffer* pVIBufferCom, _float4& vOut);

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END