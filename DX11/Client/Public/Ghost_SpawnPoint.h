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

private:
	void DotsProjecter();

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


private:
	CCollider*		m_pAreaCom = nullptr;
	CCollider*		m_pSpawnPointCom = nullptr;

#ifdef _DEBUG
	CRenderer*		m_pRendererCom = nullptr;
	_tchar		m_szWhispering[MAX_PATH] = TEXT("");
	_float		m_fWhisperingTime = 0.f;
#endif // _DEBUG

	_float		m_fWhisperCoolTime = 100.f;

	CGhost*			m_pGhost = nullptr;
	CGhost_Status* m_pGhost_Status = nullptr;

	_int	   m_iAreaDefaultTemperature = 0;
	_int       m_iAreaTemperature = 0;

	_long	   m_lAnswerFrequency = -1;

	_bool		m_bSpiritBox = true;
	_bool		m_bDotsProjecter = true;
	_bool		m_bFreeze = true;
	_bool		m_bGhostOrb = true;

	_bool		m_bCheckSpiritBox = true;
	_bool		m_bCheckDotsProjecter = true;
	_bool		m_bCheckFreeze = true;
	_bool		m_bCheckGhostOrb = true;

	_float		m_fDotsProjecterTime = 2.f;
	_float		m_fDotsProjecterCoolTime = 0.f;//5~20.f
	_bool		m_bIsInDots = false;
private:
	HRESULT	Setup_Component();
	HRESULT Setup_GhostStatus();

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
