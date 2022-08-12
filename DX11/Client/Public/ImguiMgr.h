#pragma once

#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "Base.h"

class CImguiMgr final : public CBase
{
	DECLARE_SINGLETON(CImguiMgr)

public:
	CImguiMgr();
	virtual ~CImguiMgr() = default;

public:
	virtual void Free() override;

public:
	void Init(ID3D11Device* _pDevice,	ID3D11DeviceContext* _pContext);
	void Tick(_float fTimeDelta);
	void Render();

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	
	bool show_demo_window = true;
	bool show_another_window = false;

private:
	//void CleanupDeviceD3D();
	void ShowWindow(bool* p_open);
	void Tile_Texture();
	void TestList();
};