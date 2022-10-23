#pragma once

#include "Client_Defines.h"
#include "UIBackground.h"


BEGIN(Client)
class CUIIcon;


class CJournal final : public CUIBackground
{
private:
	CJournal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CJournal(const CJournal& rhs);
	virtual ~CJournal() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void Set_Enable(_bool _bEnable);

public:
	void	Main_On(_bool _bEnable);
	void	Evidence_On(_bool _bEnable);
	void	Off_AllUI();

private:
	class CMain* m_pMain = nullptr;
	class CEvidence* m_pEvidence = nullptr;

private:
	HRESULT Setup_UI();
	HRESULT Setup_Icon();

	


public:
	static CJournal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END