#pragma once

#include "Client_Defines.h"
#include "UIBackground.h"


BEGIN(Client)
class CUIIcon;


class CJournal final : public CUIBackground
{
protected:
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
	_uint	Selected_Menu()
	{
		return m_iSelectedMenu;
	}
	
	void		Icon_Lock(_bool _bLock);


protected:
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4		m_ProjMatrix;

protected:
	_uint		 m_iSelectedMenu =0;
	vector<CUIIcon*>	 m_vecUIIcon;


public:
	static CJournal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END