#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CLevel_Lobby final : public CLevel
{
private:
	CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Lobby() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_WaitingRoom(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Store(const _tchar* pLayerTag);
	HRESULT Ready_Layer_AddItems(const _tchar* pLayerTag);

private://�� �κ� ������ �뵵�θ�
	CGameObject* m_pLobby;
	CGameObject* m_pWaitingRoom;
	CGameObject* m_pStore;
	CGameObject* m_pAddItems;


public:
	static CLevel_Lobby* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END