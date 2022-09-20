#pragma once
#include "Camera.h"
#include "Client_Defines.h"

BEGIN(Client)

class CCamera_FPS final : public CCamera
{
private:
	CCamera_FPS(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_FPS(const CCamera_FPS& rhs);
	virtual ~CCamera_FPS() = default;

public:

	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Set_Target(CTransform* _pTarget)
	{
		m_pTarget = _pTarget;
	}

	void	Get_SettingPos(_float4 _vPos)
	{
		m_vPos = _vPos;
	}
private:
	CTransform* m_pTarget= nullptr;
	_float4		m_vPos;
public:
	static CCamera_FPS* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END