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

class CIcon : public CGameObject
{
protected:
	CIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIcon(const CIcon& rhs);
	virtual ~CIcon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void Set_Enable(_bool _bEnable);
	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);


protected:
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

	void Set_Transform(_matrix WorldMat);

	_bool	Selected()
	{
		return m_bSelected;
	}

	void	Set_Texture(_uint iLevelIndex,const _tchar* _pPrototypeTag);
	void	Set_Lock(_bool bLock)
	{
		m_bLock = bLock;
	}

protected:
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4		m_ProjMatrix;

	_bool			m_bSelected = false;
	_bool			m_bOnMouse = false;
	_bool			m_bLock = true;

protected:
	HRESULT SetUp_Components();

public:
	static CIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END