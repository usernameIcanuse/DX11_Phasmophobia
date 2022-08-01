#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
public:
	CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

private:	
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

public:
	static CLevel_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

private:
	_float m_fTime = 4.f;
};

END