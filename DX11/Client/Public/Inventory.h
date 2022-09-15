#pragma once
#include "GameObject.h"
#include "Client_Defines.h"


BEGIN(Client)

class Player;
class CItem;

class CInventory final : public CGameObject
{
private:
	CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInventory(const CInventory& rhs);
	virtual ~CInventory() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void	Add_Item(CItem* pItem);
	void	Drop_Item();//������ ������
	void	Change_Item();

	void	Install_Item(_float3 _vInstallPos,COLLISION_TYPE _eCollisionType, _float4 vLook, CItem* pConnectObject = nullptr);//������ ��ġ
	void	Turn_Switch();

	void	Item_TempModel(_float3 _vInstallPos, COLLISION_TYPE _eCollisionType, _float4 vLook, CItem* pConnectObject = nullptr);
private:
	void	Adjust_Item(CItem* pItem);

private:
	vector<CItem*>	m_vInventory;
	_uint					m_iIndex = 0;

	CTransform*			m_pPlayerTransform = nullptr;

	
public:
	static CInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;//clone�� �� �� �÷��̾��� �ּҸ� ������
	virtual void Free() override;

};

END
