#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(const CComponent& Prototype);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render() { return S_OK; }

protected:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;
	_bool						m_isCloned = false;

	_bool						m_bEnable = true;
public:
	void	Set_Enable(_bool _bFlag)
	{
		m_bEnable = _bFlag;
	}

	_bool	Get_Enable()
	{
		return m_bEnable;
	}
	
public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END