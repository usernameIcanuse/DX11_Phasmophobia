#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUIIcon;


class CUIBackground : public CGameObject
{
public:
	CUIBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIBackground(const CUIBackground& rhs);
	virtual ~CUIBackground() = default;

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


protected:
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4		m_ProjMatrix;

protected:
	_uint		 m_iSelectedMenu =0;
	vector<CGameObject*>	 m_vecUIIcon;


public:
	static CUIBackground* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END