#include "Warp.h"

#include "Graphics.h"
#include "Input.h"
#include "Player.h"
#include "Game_Object_Mediator.h"

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_USING

CWarp::CWarp() : m_dwstage{ 0 },
				 m_dwlocation{ 0 }{

	get_ability_data()->get_position()->vscaling = { 80.0f, 60.0f };

}

Game_Object::CGame_Object::Update_Result CWarp::update(){

	CPlayer* pplayer = CGame_Object_Manager::CMediator::get_player();
	if( pplayer != nullptr ){

		if( !pplayer->get_ability_data()->get_physics()->bjump ){

			if( CGame_Object_Manager::CMediator::intersect_rect( pplayer->get_ability_data(), get_ability_data() ) &&
				INPUT->get_state( VK_DOWN ).bdown ){

				pplayer->set_controll_mode( CPlayer::EControl_Mode::Pipe );
				pplayer->set_pipe_stage( m_dwstage );
				pplayer->set_pipe_position( m_dwlocation );
				destroy();

			}

		}

	}

	if( INPUT->get_state( VK_SPACE ).bdown ){

		Vector2 ptscroll = CGame_Object_Manager::CMediator::get_scroll()->get_pt();

		Rectangle( GRAPHICS_MANAGER->get_dc(), ( LONG )( get_ability_data()->get_position()->position.fx - get_ability_data()->get_position()->vscaling.fx * 0.5f - ptscroll.fx ),
											   ( LONG )( get_ability_data()->get_position()->position.fy - get_ability_data()->get_position()->vscaling.fy * 0.5f - ptscroll.fy ),
											   ( LONG )( get_ability_data()->get_position()->position.fx + get_ability_data()->get_position()->vscaling.fx * 0.5f - ptscroll.fx ),
											   ( LONG )( get_ability_data()->get_position()->position.fy + get_ability_data()->get_position()->vscaling.fy * 0.5f - ptscroll.fy ) );

	}

	return get_update_result();

}

void CWarp::set_stage( DWORD dwstage ){
	m_dwstage = dwstage;
}

void CWarp::set_location( DWORD dwlocation ){
	m_dwlocation = dwlocation;
}

DWORD CWarp::get_stage() const{
	return m_dwstage;
}

DWORD CWarp::get_location() const{
	return m_dwlocation;
}
