#include "Input.h"

void CInput::update(){

	for( auto& element : m_key_state_map ){
		if( GetKeyState( element.first ) & 0x8000 ){

			element.second.bpush = !element.second.bdown; //������ ������ �ʾ����� true
														  //�������� false
			element.second.bdown = true;
			element.second.bup   = false;
		}else{
			element.second.bpush = false;
			element.second.bup   = element.second.bdown;  //������ �������� true
														  //������ �ʾ����� false
			element.second.bdown = false;
		}
	}

}

void CInput::insert( DWORD dwKey ){
	m_key_state_map.emplace( dwKey, KEYSTATE{ false, false, false } );
}

KEYSTATE CInput::get_state( DWORD dwKey ){
	auto find_iter = m_key_state_map.find( dwKey );
	assert( find_iter != m_key_state_map.end() );
	return find_iter->second;
}
