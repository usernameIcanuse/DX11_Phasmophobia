#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CLight;
END

BEGIN(Client)

class CLevel_Stage1 final : public CLevel
{
private:
	CLevel_Stage1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Stage1() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Layer_SkyBox(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayertag);
	HRESULT Load_Stage();
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Terrain(const _tchar* pLayertag);
	HRESULT Load_TruckProps();

private:
	CLight* m_pBaseLight = nullptr;

public:
	static CLevel_Stage1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END