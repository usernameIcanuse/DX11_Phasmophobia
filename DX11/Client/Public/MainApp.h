#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "ImguiMgr.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)
//class CGameInstance;
class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT Initialize();	
	void Tick(float fTimeDelta);
	HRESULT Render();

private:
	CGameInstance*			m_pGameInstance = nullptr;
	CImguiMgr*				m_pImguiMgr = nullptr;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	CRenderer*				m_pRenderer = nullptr;

private:
	HRESULT Open_Level(LEVEL eLevelID);
	HRESULT Ready_Prototype_Component();
	HRESULT Ready_Loading_Background();

public:
	

public:
	static CMainApp* Create();
	virtual void Free();

};

END