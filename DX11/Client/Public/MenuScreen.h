#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
END

BEGIN(Client)

class CMenuScreen final : public CGameObject
{
public:
	typedef struct tagMenuscreenDesc
	{
		_float4x4 WorldMat;
		_bool bGamePlay;
	}SCREENDESC;
protected:
	CMenuScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMenuScreen(const CMenuScreen& rhs);
	virtual ~CMenuScreen() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);


private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CCollider* m_pOBBCom = nullptr;

	/////////////////////////screen////////////////////////////
	class CLobby_Main* m_pMain = nullptr;
	class CLobby_WaitingRoom* m_pWaitingRoom = nullptr;
	class CLobby_GameResult* m_pGameResult = nullptr;
	//class CLobby_Store* m_pStore = nullptr;
	//class CLobby_AddItems *m_pAddItems = nullptr;

	class CUIBackground* m_pCurUI = nullptr;

	_bool				m_bLock = true;

private:
	virtual	HRESULT	Setup_Component();
	HRESULT		Setup_Screen(_bool bGameResult);
	HRESULT		Setup_Camera();

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	static CMenuScreen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END