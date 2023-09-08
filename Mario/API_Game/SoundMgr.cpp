#include "SoundMgr.h"

#include <io.h>

CSoundMgr* CSoundMgr::m_pInstance = nullptr;
CSoundMgr::CSoundMgr()
{
	m_pSystem = nullptr; 
}


CSoundMgr::~CSoundMgr()
{
	Release(); 
}

void CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);
	
	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init( m_pSystem, 32, FMOD_INIT_NORMAL, NULL );
}
void CSoundMgr::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear(); 
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}




void CSoundMgr::play_sound( const TCHAR* skey, CHANNELID channel ){

	std::map< const TCHAR*, FMOD_SOUND*>::iterator iter; 

	iter = find_if( m_mapSound.begin(), m_mapSound.end(), [ & ](  auto& iter ){
		return !lstrcmp( skey, iter.first );
	} );

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bplay = FALSE; 
	//if( FMOD_Channel_IsPlaying( m_pChannelArr[ channel ], &bplay ) ){
		FMOD_System_PlaySound( m_pSystem, iter->second, nullptr, false, &m_pChannelArr[ channel ] );
	//}
	FMOD_System_Update( m_pSystem );

}

void CSoundMgr::play_bgm( const TCHAR* skey ){
	
	std::map<const TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(skey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, false, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound( CHANNELID channel )
{
	FMOD_Channel_Stop( m_pChannelArr[ channel ] );
}

void CSoundMgr::StopAll()
{
	for (int i = 0 ; i < MAXCHANNEL ; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

bool CSoundMgr::load( const char* sfile_path, const TCHAR* key ){

	FMOD_SOUND* psound	= nullptr; 
	FMOD_RESULT result	= FMOD_System_CreateSound( m_pSystem, sfile_path, FMOD_DEFAULT, 0, &psound );

	if( result == FMOD_OK ){
		m_mapSound.emplace( key, psound );
	}else{
		return false;
	}

	FMOD_System_Update( m_pSystem );

	return true;

}
