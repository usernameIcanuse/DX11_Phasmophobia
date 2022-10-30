#include "stdafx.h"
//#include"../FMOD/include/fmod.hpp"
//#include"../FMOD/include/fmod_errors.h"
#include "../Public/SoundMgr.h"
#include "../Bin/FMOD/fmod.hpp"
#include "../Bin/FMOD/fmod_errors.h"
#include "GameInstance.h"

CSoundMgr* CSoundMgr::m_pInstance = nullptr;
CSoundMgr::CSoundMgr()
{
	m_pSystem = nullptr;
	m_NextSound = nullptr;
}


CSoundMgr::~CSoundMgr()
{
	Release();
}

HRESULT CSoundMgr::Initialize()
{
	m_fSound = 1;
	m_bBGMOut = false;
	m_fSoundMaxDistance = 200.f;
	//ZeroMemory(m_eCurChannel, sizeof(m_eCurChannel));

	m_eCurChannel[CHANNEL_BGM] = BGM;
	m_eCurChannel[CHANNEL_PLAYER] = CH5;
	m_eCurChannel[CHANNEL_ITEM] = CH10;
	m_eCurChannel[CHANNEL_GHOST] = CH20;
	m_eCurChannel[CHANNEL_UI] = CH30;

	m_eChannelMinNum[CHANNEL_BGM] = BGM;
	m_eChannelMinNum[CHANNEL_PLAYER] = CH1;
	m_eChannelMinNum[CHANNEL_ITEM] = CH6;
	m_eChannelMinNum[CHANNEL_GHOST] = CH16;
	m_eChannelMinNum[CHANNEL_UI] = CH26;

	m_eChannelMaxNum[CHANNEL_BGM] = BGM;
	m_eChannelMaxNum[CHANNEL_PLAYER] = CH5;
	m_eChannelMaxNum[CHANNEL_ITEM] = CH15;
	m_eChannelMaxNum[CHANNEL_GHOST] = CH25;
	m_eChannelMaxNum[CHANNEL_UI] = CH30;



	FMOD_System_Create(&m_pSystem);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	if(FAILED(FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL)))
		return E_FAIL;

	LoadSoundFile();

	return S_OK;
}
void CSoundMgr::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

int CSoundMgr::VolumeUp(CHANNELID eID, _float _vol)
{
	if (m_volume < SOUND_MAX) {
		m_volume += _vol;
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_volume);

	return 0;
}

int CSoundMgr::VolumeDown(CHANNELID eID, _float _vol)
{
	if (m_volume > SOUND_MIN) {
		m_volume -= _vol;
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_volume);

	return 0;
}

int CSoundMgr::BGMVolumeUp(_float _vol)
{
	if (m_BGMvolume < SOUND_MAX) {
		m_BGMvolume += _vol;
	}

	FMOD_Channel_SetVolume(m_pChannelArr[BGM], m_BGMvolume);

	return 0;
}

int CSoundMgr::BGMVolumeDown(_float _vol)
{
	if (m_BGMvolume > SOUND_MIN) {
		m_BGMvolume -= _vol;
	}

	FMOD_Channel_SetVolume(m_pChannelArr[BGM], m_BGMvolume);

	return 0;
}

int CSoundMgr::Pause(CHANNELID eID)
{
	m_bPause = !m_bPause;
	FMOD_Channel_SetPaused(m_pChannelArr[eID], m_bPause);

	return 0;
}


void CSoundMgr::PlaySound(TCHAR * pSoundKey, CHANNELID eID, _float _vol)//계속플레이하고 싶을 때
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem,FMOD_CHANNEL_FREE,iter->second, FALSE, &m_pChannelArr[eID]);
		if (_vol >= SOUND_MAX)
			_vol = 1.f;
		else if (_vol <= SOUND_MIN)
			_vol = 0.f;
		FMOD_Channel_SetVolume(m_pChannelArr[eID], _vol);
	}
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlayBGM(TCHAR * pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;
	
	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlayBGM(TCHAR * pSoundKey, _float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[BGM], fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < CH_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::PlaySound(const TCHAR * strSoundKey, CHANNEL_TYPE eType, _float fVolume)
{
 	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
	{
		return !lstrcmp(strSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	//if (FMOD_Channel_IsPlaying(m_pChannelArr[m_eCurChannel[eType]], &bPlay))
	//{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[m_eCurChannel[eType]]);
		FMOD_Channel_SetVolume(m_pChannelArr[UINT(m_eCurChannel[eType])], fVolume);
	//}
	//else
	//{
	//	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[m_eCurChannel[eType]]);
	//	FMOD_Channel_SetVolume(m_pChannelArr[UINT(m_eCurChannel[eType])], fVolume);
	//}
	//if (m_eCurChannel[eType] >= CH_END)
	//	m_eCurChannel[eType] = CH1;

	if (m_eCurChannel[eType] >= m_eChannelMaxNum[eType])
	{
		m_eCurChannel[eType] = m_eChannelMinNum[eType];
	}
	else
	{
		m_eCurChannel[eType] = (CHANNELID)(m_eCurChannel[eType] + 1);
	}

	//FMOD_Channel_SetVolume(m_pChannelArr[UINT(m_eCurChannel[eType])], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlaySoundDistance(const TCHAR * strSoundKey, CHANNEL_TYPE eType,  CGameObject * pTarget, _float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
	{
		return !lstrcmp(strSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;


	_float	DistanceVolume = 0.f;
	_vector vTargetPos = pTarget->Get_Pos();
	_vector vPlayerPos = m_pPlayer->Get_Pos();
	_float fDistance = XMVectorGetX(XMVector3Length(vTargetPos - vPlayerPos));
	if (m_fSoundMaxDistance < fDistance)
	{
		fDistance = m_fSoundMaxDistance;
	}
	DistanceVolume = (1.f - (fDistance / m_fSoundMaxDistance)) * fVolume;


	//if (FMOD_Channel_IsPlaying(m_pChannelArr[m_eCurChannel[eType]], &bPlay))
	//{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[m_eCurChannel[eType]]);
		FMOD_Channel_SetVolume(m_pChannelArr[UINT(m_eCurChannel[eType])], DistanceVolume);
	//}
	//else
	//{
	//	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[m_eCurChannel[eType]]);
	//	FMOD_Channel_SetVolume(m_pChannelArr[UINT(m_eCurChannel[eType])], DistanceVolume);
	//}
	//if (m_eCurChannel[eType] >= CH_END)
	//	m_eCurChannel[eType] = CH1;

	if (m_eCurChannel[eType] >= m_eChannelMaxNum[eType])
	{
		m_eCurChannel[eType] = m_eChannelMinNum[eType];
	}
	else
	{
		m_eCurChannel[eType] = (CHANNELID)(m_eCurChannel[eType] + 1);
	}


	//FMOD_Channel_SetVolume(m_pChannelArr[UINT(m_eCurChannel[eType])], DistanceVolume);

	FMOD_System_Update(m_pSystem);

	RELEASE_INSTANCE(CGameInstance);
}




void CSoundMgr::ChangeBGM(TCHAR * pSoundKey, _float fVolume)
{
	m_bBGMOut = true;
	m_NextSound = pSoundKey;
	m_fMaxBGMVolume = fVolume;
}

void CSoundMgr::Tick()
{
	if (m_bBGMOut)
	{
		m_fSound -= 0.01f;
		FMOD_Channel_SetVolume(m_pChannelArr[BGM], m_fSound);
		if (0 > m_fSound)
		{
			PlayBGM(m_NextSound, m_fSound);
			m_bBGMOut = false;
		}
	}
	else
	{
		if (m_fMaxBGMVolume>m_fSound)
		{
			m_fSound += 0.01f;
		}
		else
		{
			m_fSound = m_fMaxBGMVolume;
		}
		FMOD_Channel_SetVolume(m_pChannelArr[BGM], m_fSound);
	}
}


void CSoundMgr::LoadSoundFile()
{
	_tfinddata_t fd;
	__int64 handle = _tfindfirst(L"../Bin/Sound/*.*", &fd);
	if (handle == -1 || handle == 0)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Bin/Sound/";
	char szFullPath[128] = "";
	char szFilename[MAX_PATH];
	while (iResult != -1)
	{
		WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, szFilename);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);
		if (eRes == FMOD_OK)
		{
			size_t iLength = strlen(szFilename) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, szFilename, (int)iLength, pSoundKey, (int)iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _tfindnext(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);

	_findclose(handle);
}
