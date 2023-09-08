#ifndef _MARIO_TOOL_INPUT_H_
#define _MARIO_TOOL_INPUT_H_

#include "Global.h"

#include <Windows.h>

struct KEYSTATE{
	bool bpush;		//처음 누를 때에만 true입니다.
	bool bdown;     //누르면 항상 true입니다.
	bool bup;		//처음 버튼을 때었을 때만 true입니다.
};

class CInput{
	SINGLETON_DEFAULT( CInput );
public:

	void update();

	void insert( DWORD dwKey );

	KEYSTATE get_state( DWORD dwKey );


private:

	std::map< DWORD, KEYSTATE > m_key_state_map;

};

#define INPUT Library::CSingleton< CInput >::get()

#endif