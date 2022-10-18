#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CRenderer;
END


BEGIN(Client)

class CGhost;
class CGhost_Status;
/*main*/

class CGhost_SpawnPoint final: public CGameObject
{
	friend class CGhost;

public:
	typedef struct
	{
		_float4x4 matWorld;
		_int iCurrentIndex;
	}GHOST_LOADDATA;

private:
	CGhost_SpawnPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGhost_SpawnPoint(const CGhost_SpawnPoint& rhs);
	virtual ~CGhost_SpawnPoint() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void Set_Enable(_bool _bEnable);
	virtual void OnEventMessage(const _tchar* pMessage);
	virtual void Call_EventFunc(_float fTimeDelta = 0.f);

private:
	void Light_Attack(_float fTimeDelta = 0.f); //ÀÌº¥Æ®
	void Attack(_float fTimeDelta = 0.f); //ÇåÆÃ
	void Normal_Operation(_float fTimeDelta = 0.f);

public:
	void	Add_Score(_int _iScoreIndex);
	_int	Get_Anger();
	_uint   Get_EMFLevel();
	
	_int	Get_AreaTemperature()
	{
		return m_iAreaTemperature;
	}

	_int    Get_SpawnPointTemperature();
	
	void	Get_Answer(_long _lFrequency, _float& _fTime);
	void	Set_Ghost(CGhost* _pGhost)
	{
		m_pGhost = _pGhost;
	}

	_vector Get_SpawnPoint()
	{
		return m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);			
	}


private:
	function<void(CGhost_SpawnPoint*, _float)> m_EventFunc;

	CCollider*		m_pAreaCom = nullptr;
	CCollider*		m_pSpawnPointCom = nullptr;

	_int			m_iSpawnPointIndex = 0;

#ifdef _DEBUG
	CRenderer*		m_pRendererCom = nullptr;
	_tchar		m_szWhispering[MAX_PATH] = TEXT("");
	_float		m_fWhisperingTime = 0.f;
#endif // _DEBUG

	_float		m_fWhisperCoolTime = 100.f;

	CGhost*			m_pGhost = nullptr;
	CGhost_Status* m_pGhost_Status = nullptr;

	class CHandPrint* m_pHandPrint = nullptr;


	_int	   m_iAreaDefaultTemperature = 0;
	_int       m_iAreaTemperature = 0;

	_long	   m_lAnswerFrequency = -1;

	_bool		m_bSpiritBox = true;
	_bool		m_bDotsProjecter = true;
	_bool		m_bFreeze = true;
	_bool		m_bGhostOrb = true;
	_bool		m_bGhostWriting = false;
	_bool		m_bHandPrint = true;


	_bool		m_bCheckSpiritBox = true;
	_bool		m_bCheckDotsProjecter = true;
	_bool		m_bCheckFreeze = true;
	_bool		m_bCheckGhostOrb = true;
	_bool		m_bCheckGhostWriting = false;
	_bool		m_bCheckHandPrint = false;

	_float		m_fDotsProjecterTime = 2.f;
	_float		m_fDotsProjecterCoolTime = 0.f;//5~20.f

	_float		m_fIdleTime = 3.f;
	_float		m_fHandPrintCoolTime = -1.f;
	_bool		m_bIsInDots = false;
	_float		m_fDotsTime = 1.5f;



private:
	HRESULT	Setup_Component();
	HRESULT Setup_Ghost();
	HRESULT Setup_GhostStatus();
	HRESULT Load_Point(const _tchar* pFilePath);

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	static CGhost_SpawnPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
