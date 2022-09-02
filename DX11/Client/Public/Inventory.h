#pragma once
#include "GameObject.h"
#include "Client_Defines.h"


BEGIN(Client)

class Player;

class CInventory final : public CGameObject
{
	friend class CPlayer;
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
	void	Add_Item(CGameObject* pItem);
	void	Drop_Item();//아이템 버리기
	void	Install_Item(_float3 _vInstallPos);//아이템 설치
	void	Change_Item();

private:
	vector<CGameObject*>	m_vInventory;
	_uint					m_iIndex = 0;
	CGameObject*			m_pPlayer = nullptr;



public:
	static CInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;//clone을 할 때 플레이어의 주소를 갖다줌
	virtual void Free() override;

};

END
