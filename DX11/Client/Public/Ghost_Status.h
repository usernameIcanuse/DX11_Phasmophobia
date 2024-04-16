#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CRenderer;
END


BEGIN(Client)

class CGhost_SpawnPoint;


class CGhost_Status final: public CGameObject
{
public:

	enum WEIGHT {PLAYER_IN_HOUSE = 2, PLAYER_IN_AREA = 2, ITEM_IN_AREA=1/*total 6*/, SPIRITBOX = 2, DOTSPROJECTER = 1,
				CAMERA = 1/*total 2*/, FIND_EVIDENCE = 5/*total 3*/,TIME_ATTACK = 1/*1 per minute*/ };
				//=>Total Score 40
	friend class CGhost_SpawnPoint;
private:
	CGhost_Status(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGhost_Status(const CGhost_Status& rhs);
	virtual ~CGhost_Status() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void OnEventMessage(const _tchar* pMessage);
	
public:
	void	Add_Score(_int _iScore)
	{
		m_iScore += _iScore;
	}
	void	Subtract_Score(_int _iScore)
	{
		m_iScore -= _iScore;
	}
	void	Set_EMF(_bool _bEnable)
	{
		m_bEMFLevel5 = _bEnable;
	}


private:

#ifdef _DEBUG
	_tchar		m_szAgression[MAX_PATH] = TEXT("");//공격성 등 여러개 출력
	CRenderer* m_pRenderercom = nullptr;
	_float		m_fTimeAcc = 0.f;
#endif

	_int	   m_iScore = 0;
	_uint	   m_iEMF = 1;


	_float		m_fTime = 0.f;//timeattack용
	_int		m_iCnt = 0;

	_float		m_fEventCoolTime = 100.f;//이벤트와 헌팅
	//_float		m_fAttackCoolTime = 100.f;

	_float		m_fTermBeforeEvent = 5.f;
	_tchar		m_szEventMessage[MAX_PATH] = TEXT("");

	_uint		m_iEventWeight = 0;
	//_uint		m_iAttackWeight = 0;

	_bool		m_bEvent = false;
	_bool		m_bAttack = false;
	_bool		m_bTerm = false;


	_bool		m_bEMFLevel5 = false;
	

public:
	static CGhost_Status* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;//SpawnPoint를 넘겨줌
	virtual void Free() override;

};

END
