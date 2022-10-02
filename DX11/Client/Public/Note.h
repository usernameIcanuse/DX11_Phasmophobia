#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Client)

class CNote final : public CItem
{
private:
	CNote(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNote(const CNote& rhs);
	virtual ~CNote() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


public:
	virtual _bool Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem = nullptr);
	virtual void Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem = nullptr);

private:
	void	Ghost_Writing();
private:
	CItem* m_pTempNoteModel = nullptr;

	CModel* m_pNoteOpenModel = nullptr;

	_bool	m_bFirst = true;

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

private:
	virtual	HRESULT	Setup_Component() override;
	HRESULT			Setup_TempModel();
public:
	static CNote* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END