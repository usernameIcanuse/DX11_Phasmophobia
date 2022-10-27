#pragma once

/* 다음레벨에 필요한 자원을 로드하낟. */

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	CRITICAL_SECTION Get_CS() const {
		return m_CriticalSection;
	}

	LEVEL Get_NextLevelID() const {
		return m_eNextLevel;
	}

	_bool is_Finished() const {
		return m_isFinished;
	}

	const _tchar* Get_LoadingText() const {
		return m_szLoadingText;
	}

public:
	HRESULT Initialize(LEVEL eNextLevel,_bool _bFirst, STAGE eStage = STAGE_END);
	HRESULT Loading_ForLogoLevel();
	HRESULT Loading_ForStaticProps();
	HRESULT Loading_ForLobbyLevel();
	HRESULT Loading_ForTutorialLevel();
	HRESULT Loading_ForStreetHouseLevel();

public:
	_float	Get_LoadingRatio()
	{
		return m_fLoadingRatio*100.f;
	}

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;	
	LEVEL						m_eNextLevel = LEVEL_END;


private:
	HANDLE					m_hThread;
	CRITICAL_SECTION		m_CriticalSection;
	_bool					m_isFinished = false;
	_tchar					m_szLoadingText[MAX_PATH] = TEXT("");

	_float					m_fLoadingRatio = 0.f;


public:
	_bool					m_bFirst = false;
	STAGE					m_eStage = STAGE_END;
	
public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel, _bool _bFirst = false, STAGE eStage = STAGE_END);
	virtual void Free() override;
};

END