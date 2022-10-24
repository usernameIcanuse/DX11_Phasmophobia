#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CGameObject;
class CLight;
END

BEGIN(Client)

class CLevel_Lobby final : public CLevel
{
private:
	CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Lobby() = default;

public:
	virtual HRESULT Initialize(_bool bGamePlay);
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

private:

	HRESULT Ready_Layer_SkyBox(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Lights();
	HRESULT Load_Stage();


private://각 로비 저장할 용도로만
	CGameObject* m_pLobby;
	CGameObject* m_pWaitingRoom;
	CGameObject* m_pStore;
	CGameObject* m_pAddItems;

	CLight* m_pBaseLight = nullptr;


	_bool	m_bGamePlay = false;
public:
	static CLevel_Lobby* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _bool bGamePlay);
	virtual void Free() override;
};

END