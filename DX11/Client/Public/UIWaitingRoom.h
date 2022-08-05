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

class CUIWaitingRoom final : public CGameObject
{
public:
	CUIWaitingRoom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIWaitingRoom(const CUIWaitingRoom& rhs);
	virtual ~CUIWaitingRoom() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	Set_Enable(_bool _bEnable);

	_bool	Selected()
	{
		return m_bSelected;
	}

private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;		
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
public:
	void	Set_IconPosition(_float _fX, _float _fY, _float _fSizeX, _float _fSizeY)
	{
		m_fX = _fX;
		m_fY = _fY;
		m_fSizeX = _fSizeX;
		m_fSizeY = _fSizeY;
	}

private:
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4		m_ProjMatrix;

	_bool			m_bSelected = false;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CUIWaitingRoom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END