#pragma once

/* 로더가 자우너을 로드하는 동안 화면에 뭔가를 보여줄꺼여. */
/* 로더한테 계쏙 물어볼꺼야 끝났냐?? */
/* 어 라고하면 다음레벨로 가자.고 */

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Loading final : public CLevel
{

private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVEL eNextLevel,_bool bFirst, _bool bLobby = false);
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Layer_Loading(const _tchar* pLayerTag);
	HRESULT Ready_Layer_FirstLoading(const _tchar* pLayerTag);

private:
	LEVEL			m_eNextLevel = LEVEL_END;
	class CLoader*	m_pLoader = nullptr;

	_bool			m_bLobby = false;

public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel, _bool bFirst = false, _bool bLobby = false);

	virtual void Free() override;
};

END