#include "Field.h"

#include "SoundMgr.h"
#include "Graphics.h"
#include "Input.h"
#include "Bitmap.h"
#include "Game_Object_Mediator.h"
#include "Tile.h"
#include "Ability.h"
#include "Player.h"

bool CFight_Field::s_binitialize_resource = false;

CFight_Field::CFight_Field() : m_background_animation{},
							   m_background_tile_index{},
							   m_pui_bitmap{},
							   m_ui_number_animation{},
							   m_ui_accel_animation{},
							   m_ui_accel_full_animation{},
							   m_ui_clear_item_animation{},
							   m_dwstage{ ( DWORD )Tile_Map::EAttribute::Stage_0 },
							   m_dwposition{ 0 },
							   m_bpipe{ false }{
}

void CFight_Field::initialize(){

	assert( initialize_resource() );

	if( m_bpipe ){

		Game_Object::CGame_Object_Manager::CMediator::create_player( { 400, 300 }, { 0.0f, -1.0f } );
		Game_Object::CGame_Object_Manager::CMediator::create_tile_map_object();
		Game_Object::CGame_Object_Manager::CMediator::get_player()->get_ability_data()->get_position()->position = TILE_MAP->get_position( m_dwposition );
		Game_Object::Ability::setting_normal( Game_Object::CGame_Object_Manager::CMediator::get_player()->get_ability_data() );

	}else{

		Game_Object::CGame_Object_Manager::CMediator::create_player( { 400, 300 }, { 0.0f, -1.0f } );
		Game_Object::CGame_Object_Manager::CMediator::create_tile_map_object();
		Game_Object::CGame_Object_Manager::CMediator::get_player()->get_ability_data()->get_position()->position = TILE_MAP->get_player_position();
		Game_Object::Ability::setting_normal( Game_Object::CGame_Object_Manager::CMediator::get_player()->get_ability_data() );

	}

	play_bgm();

	GAME_OBJECT_MANAGER->get_wall_group()->start_tile();

	Game_Object::CGame_Object_Manager::CMediator::get_scroll()->set_pt( Game_Object::CGame_Object_Manager::CMediator::get_player()->get_ability_data()->get_position()->position );

}

void CFight_Field::release(){

	GAME_OBJECT_MANAGER->release();
	CSoundMgr::Get_Instance()->StopAll();

}

CField_Base* CFight_Field::update(){

	render();

	GAME_OBJECT_MANAGER->update();

	render_ui();

	Game_Object::Unit::CPlayer* pplayer = Game_Object::CGame_Object_Manager::CMediator::get_player();
	if( pplayer->get_update_result() == Game_Object::Unit::CPlayer::UPDATE_RESULT_PLAYER_CLEAR ){
		
		if( get_next_stage() != ( DWORD )Tile_Map::EAttribute::Stage_3 ){
			CFight_Field* pfield = new CFight_Field{};
			pfield->set_stage( get_next_stage() );
			pfield->set_pipe( false );
			return pfield;
		}else{
			
			CEnding_Field* pfield = new CEnding_Field{};
			return pfield;

		}


	}else if( pplayer->get_update_result() == Game_Object::Unit::CPlayer::UPDATE_RESULT_PLAYER_PIPE ){

		CFight_Field* pfield = new CFight_Field{};
		pfield->set_stage(  pplayer->get_pipe_stage() );
		pfield->set_position(  pplayer->get_pipe_position() );
		pfield->set_pipe( true );

		return pfield;

	}else if( pplayer->get_update_result() == Game_Object::Unit::CPlayer::UPDATE_RESULT_PLAYER_DIE ){

		if( Game_Object::Unit::CPlayer::s_ilife <= 0 ){

			CEnding_Field* pfield = new CEnding_Field{};
			return pfield;

		}else{

			CFight_Field* pfield = new CFight_Field{};
			pfield->set_stage( m_dwstage );
			pfield->set_pipe( false );
			return pfield;

		}

	}

    return this;

}

void CFight_Field::render(){

    Vector2	vtile_size		= TILE_MAP->get_tile_property().vtile_size;
	Vector2 vscroll = Game_Object::CGame_Object_Manager::CMediator::get_scroll()->get_pt();

	m_background_animation.render( GRAPHICS_MANAGER->get_dc(), { 0, 0, 800, 600 }, RGB( 255, 0, 255 ) );

	//타일 구분선을 그립니다.
	if( INPUT->get_state( VK_RETURN ).bdown ){

		for( float fy = fmodf( -vscroll.fy, vtile_size.fy ); fy <= 600.0f + vtile_size.fy; fy += vtile_size.fy ){
			
			MoveToEx( GRAPHICS_MANAGER->get_dc(), 0, ( int )fy, nullptr );
			LineTo( GRAPHICS_MANAGER->get_dc(), 800, ( int )fy );

		}

		for( float fx = fmodf( -vscroll.fx, vtile_size.fx ); fx <= 800.0f + vtile_size.fx; fx += vtile_size.fx ){

			MoveToEx( GRAPHICS_MANAGER->get_dc(), ( int )fx, 0, nullptr );
			LineTo( GRAPHICS_MANAGER->get_dc(), ( int )fx, 600 );

		}

	}

}

void CFight_Field::set_stage( DWORD dwstage ){
	m_dwstage = dwstage;
}

void CFight_Field::set_position( DWORD dwposition ){
	m_dwposition = dwposition;
}

void CFight_Field::set_pipe( bool bpipe ){
	m_bpipe = bpipe;
}

DWORD CFight_Field::get_stage() const{
	return m_dwstage;
}

DWORD CFight_Field::get_position() const{
	return m_dwposition;
}

bool CFight_Field::get_pipe() const{
	return m_bpipe;
}

const char* CFight_Field::get_tile_map_path(){

	switch( m_dwstage ){
	case ( DWORD )Tile_Map::EAttribute::Stage_0:
		return "Resource/Data/Stage/TileMap0.txt";
	case ( DWORD )Tile_Map::EAttribute::Stage_1:
		return "Resource/Data/Stage/TileMap1.txt";
	case ( DWORD )Tile_Map::EAttribute::Stage_2:
		return "Resource/Data/Stage/TileMap2.txt";
	case ( DWORD )Tile_Map::EAttribute::Stage_3:
		return "Resource/Data/Stage/TileMap3.txt";
	case ( DWORD )Tile_Map::EAttribute::Stage_4:
		return "Resource/Data/Stage/TileMap4.txt";
	case ( DWORD )Tile_Map::EAttribute::Bonus_0:
		return "Resource/Data/Bonus/TileMap0.txt";
	case ( DWORD )Tile_Map::EAttribute::Bonus_1:
		return "Resource/Data/Bonus/TileMap1.txt";
	case ( DWORD )Tile_Map::EAttribute::Bonus_2:
		return "Resource/Data/Bonus/TileMap2.txt";
	case ( DWORD )Tile_Map::EAttribute::Bonus_3:
		return "Resource/Data/Bonus/TileMap3.txt";
	case ( DWORD )Tile_Map::EAttribute::Bonus_4:
		return "Resource/Data/Bonus/TileMap4.txt";
	}

	return nullptr;

}

DWORD CFight_Field::get_next_stage() const{

	switch( m_dwstage ){
	case ( DWORD )Tile_Map::EAttribute::Stage_0:
		return ( DWORD )Tile_Map::EAttribute::Stage_1;
	case ( DWORD )Tile_Map::EAttribute::Stage_1:
		return ( DWORD )Tile_Map::EAttribute::Stage_2;
	case ( DWORD )Tile_Map::EAttribute::Stage_2:
		return ( DWORD )Tile_Map::EAttribute::Stage_3;
	case ( DWORD )Tile_Map::EAttribute::Stage_3:
		return ( DWORD )Tile_Map::EAttribute::Stage_1;
	case ( DWORD )Tile_Map::EAttribute::Stage_4:
		return ( DWORD )Tile_Map::EAttribute::Stage_1;
	case ( DWORD )Tile_Map::EAttribute::Bonus_0:
		return ( DWORD )Tile_Map::EAttribute::Stage_1;
	case ( DWORD )Tile_Map::EAttribute::Bonus_1:
		return ( DWORD )Tile_Map::EAttribute::Stage_1;
	case ( DWORD )Tile_Map::EAttribute::Bonus_2:
		return ( DWORD )Tile_Map::EAttribute::Stage_1;
	case ( DWORD )Tile_Map::EAttribute::Bonus_3:
		return ( DWORD )Tile_Map::EAttribute::Stage_1;
	case ( DWORD )Tile_Map::EAttribute::Bonus_4:
		return ( DWORD )Tile_Map::EAttribute::Stage_1;
	}

	return 0;

}

XY CFight_Field::get_background_tile_index(){

	switch( m_dwstage ){
	case ( DWORD )Tile_Map::EAttribute::Stage_0:
		return { 41, 1 };
	case ( DWORD )Tile_Map::EAttribute::Stage_1:
		return { 41, 1 };
	case ( DWORD )Tile_Map::EAttribute::Stage_2:
		return { 41, 2 };
	case ( DWORD )Tile_Map::EAttribute::Stage_3:
		return { 41, 1 };
	case ( DWORD )Tile_Map::EAttribute::Stage_4:
		return { 41, 1 };
	case ( DWORD )Tile_Map::EAttribute::Bonus_0:
		return { 41, 2 };
	case ( DWORD )Tile_Map::EAttribute::Bonus_1:
		return { 41, 2 };
	case ( DWORD )Tile_Map::EAttribute::Bonus_2:
		return { 41, 2 }; 
	case ( DWORD )Tile_Map::EAttribute::Bonus_3:
		return { 41, 2 };
	case ( DWORD )Tile_Map::EAttribute::Bonus_4:
		return { 41, 2 };
	}

	return XY();

}

void CFight_Field::play_bgm(){

	switch( m_dwstage ){
	case ( DWORD )Tile_Map::EAttribute::Stage_0:
		CSoundMgr::Get_Instance()->play_bgm( TEXT( "BGM_Stage_0" ) );
		break;
	case ( DWORD )Tile_Map::EAttribute::Stage_1:
		CSoundMgr::Get_Instance()->play_bgm( TEXT( "BGM_Stage_1" ) );
		break;
	case ( DWORD )Tile_Map::EAttribute::Stage_2:
		CSoundMgr::Get_Instance()->play_bgm( TEXT( "BGM_Stage_2" ) );
		break;
	case ( DWORD )Tile_Map::EAttribute::Stage_3:
		CSoundMgr::Get_Instance()->play_bgm( TEXT( "BGM_Stage_0" ) );
		break;
	case ( DWORD )Tile_Map::EAttribute::Stage_4:
		CSoundMgr::Get_Instance()->play_bgm( TEXT( "BGM_Stage_0" ) );
		break;
	case ( DWORD )Tile_Map::EAttribute::Bonus_0:
	case ( DWORD )Tile_Map::EAttribute::Bonus_1:
	case ( DWORD )Tile_Map::EAttribute::Bonus_2:
	case ( DWORD )Tile_Map::EAttribute::Bonus_3:
	case ( DWORD )Tile_Map::EAttribute::Bonus_4:
		CSoundMgr::Get_Instance()->play_bgm( TEXT( "BGM_Bonus" ) );
		break;
	}

}

void CFight_Field::render_ui(){

	GdiTransparentBlt( GRAPHICS_MANAGER->get_dc(), 0, 500, 800, 100,
					   m_pui_bitmap->get_dc(), 0, 0, 264, 34,
					   RGB( 255, 0, 255 ) );


	//Coin
	int icoin = Game_Object::Unit::CPlayer::s_icoin;
	Vector2 ptnumber = { 460,  522 };
	for( int i = 0; i < 2; ++i ){

		m_ui_number_animation.set_frame( { icoin % 10, 0 } );
		m_ui_number_animation.render( GRAPHICS_MANAGER->get_dc(), { ( LONG )ptnumber.fx, ( LONG )ptnumber.fy, 17, 35 }, RGB( 255, 0, 255 ) );

		ptnumber.fx -= 17.0f;

		icoin /= 10;

	}

	//Life
	int ilife = Game_Object::Unit::CPlayer::s_ilife;
	Vector2 ptlife{ 135.0f, 550.0f };
	for( int i = 0; i < 2; ++i ){

		m_ui_number_animation.set_frame( { ilife % 10, 0 } );
		m_ui_number_animation.render( GRAPHICS_MANAGER->get_dc(), { ( LONG )ptlife.fx, ( LONG )ptlife.fy, 17, 35 }, RGB( 255, 0, 255 ) );

		ptlife.fx -= 17.0f;

		ilife /= 10;

	}

	//Score
	int iscore = Game_Object::Unit::CPlayer::s_iscore;
	Vector2 ptscore{ 340.0f, 550.0f };
	for( int i = 0; i < 10; ++i ){

		m_ui_number_animation.set_frame( { iscore % 10, 0 } );
		m_ui_number_animation.render( GRAPHICS_MANAGER->get_dc(), { ( LONG )ptscore.fx, ( LONG )ptscore.fy, 17, 35 }, RGB( 255, 0, 255 ) );

		ptscore.fx -= 17.0f;

		iscore /= 10;

	}

	//Stage
	m_ui_number_animation.set_frame( { ( int )( m_dwstage - ( DWORD )Tile_Map::EAttribute::Stage_0 + 1 ), 0 } );
	m_ui_number_animation.render( GRAPHICS_MANAGER->get_dc(), { 145, 520, 17, 35 }, RGB( 255, 0, 255 ) );

	//Accel
	int iaccel = 0;
	Game_Object::Unit::CPlayer* pplayer = Game_Object::CGame_Object_Manager::CMediator::get_player();
	if( pplayer != nullptr ){
		iaccel = pplayer->get_accel();
	}
	Vector2 ptaccel{ 187.0f, 520.0f };
	for( int i = 1; i <= 10; ++i ){

		if( i <= iaccel ){
			m_ui_accel_animation.set_frame( { 0, 0 } );
		}else{
			m_ui_accel_animation.set_frame( { 1, 0 } );
		}
		m_ui_accel_animation.render( GRAPHICS_MANAGER->get_dc(), { ( LONG )ptaccel.fx, ( LONG )ptaccel.fy, 17, 35 }, RGB( 255, 0, 255 ) );

		ptaccel.fx += 17.0f;

	}

	if( iaccel == 10 ){
		m_ui_accel_full_animation.set_frame( { 0, 0 } );
	}else{
		m_ui_accel_full_animation.set_frame( { 1, 0 } );
	}
	m_ui_accel_full_animation.render( GRAPHICS_MANAGER->get_dc(), { 357, 520, 51, 35 }, RGB( 255, 0, 255 ) );

	//Clear item
	Vector2 ptclear_item = { 528.0f, 518.0f };
	for( int i = 0; i < 3; ++i ){

		m_ui_clear_item_animation.set_frame( { Game_Object::Unit::CPlayer::s_iclear_item[ i ], 0 } );
		m_ui_clear_item_animation.render( GRAPHICS_MANAGER->get_dc(), { ( LONG )ptclear_item.fx, ( LONG )ptclear_item.fy, 60, 60 }, RGB( 255, 0, 255 ) );

		ptclear_item.fx += 72.0f;

	}

}

bool CFight_Field::initialize_resource(){

	TILE_MAP->load( get_tile_map_path() );
	TILE_MAP->set_tile_size( { 60, 60 } );
	TILE_MAP->initialize();

	GAME_OBJECT_MANAGER->get_wall_group()->reset_tile_map();

	if( !s_binitialize_resource ){

		if( !initialize_bitmap() ){
			return false;
		}

		if( !initialize_sprite() ){
			return false;
		}

		if( !initialize_sound() ){
			return false;
		}

		s_binitialize_resource = true;

	}

	m_background_tile_index = get_background_tile_index();
	m_background_animation.set_bitmap( BITMAP_MANAGER->find_bitmap( TEXT( "Tile" ) ) );
	m_background_animation.set_sprite( BITMAP_MANAGER->find_sprite( TEXT( "Tile" ) ) );
	m_background_animation.set_frame( m_background_tile_index );
	m_background_animation.set_animation_tick_ms( 0 );

	m_pui_bitmap = BITMAP_MANAGER->find_bitmap( TEXT( "UI" ) );

	m_ui_number_animation.set_bitmap( BITMAP_MANAGER->find_bitmap( TEXT( "UI_Number" ) ) );
	m_ui_number_animation.set_sprite( BITMAP_MANAGER->find_sprite( TEXT( "UI_Number" ) ) );
	m_ui_number_animation.set_frame( { 0, 0 } );
	m_ui_number_animation.set_animation_tick_ms( 0 );

	m_ui_accel_animation.set_bitmap( BITMAP_MANAGER->find_bitmap( TEXT( "UI_Accel" ) ) );
	m_ui_accel_animation.set_sprite( BITMAP_MANAGER->find_sprite( TEXT( "UI_Accel" ) ) );
	m_ui_accel_animation.set_frame( { 0, 0 } );
	m_ui_accel_animation.set_animation_tick_ms( 0 );

	m_ui_accel_full_animation.set_bitmap( BITMAP_MANAGER->find_bitmap( TEXT( "UI_Accel" ) ) );
	m_ui_accel_full_animation.set_sprite( BITMAP_MANAGER->find_sprite( TEXT( "UI_Accel_Full" ) ) );
	m_ui_accel_full_animation.set_frame( { 0, 0 } );
	m_ui_accel_full_animation.set_animation_tick_ms( 0 );

	m_ui_clear_item_animation.set_bitmap( BITMAP_MANAGER->find_bitmap( TEXT( "UI_Clear_Item" ) ) );
	m_ui_clear_item_animation.set_sprite( BITMAP_MANAGER->find_sprite( TEXT( "UI_Clear_Item" ) ) );
	m_ui_clear_item_animation.set_frame( { 0, 0 } );
	m_ui_clear_item_animation.set_animation_tick_ms( 0 );

	return true;

}

bool CFight_Field::initialize_bitmap(){

	assert( BITMAP_MANAGER->inset_bidirection_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "Mario_Small" ), TEXT( "Resource/Bitmap/Mario_Small.bmp" ), 18 ) );
	assert( BITMAP_MANAGER->inset_bidirection_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "Mario_Big" ), TEXT( "Resource/Bitmap/Mario_Big.bmp" ), 18 ) );
	assert( BITMAP_MANAGER->inset_bidirection_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "Enemy_1x1" ), TEXT( "Resource/Bitmap/Enemy_1x1.bmp" ), 8 ) );
	assert( BITMAP_MANAGER->inset_bidirection_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "Enemy_1x2" ), TEXT( "Resource/Bitmap/Enemy_1x2.bmp" ), 8 ) );
	assert( BITMAP_MANAGER->inset_bidirection_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "Koopa" ), TEXT( "Resource/Bitmap/Koopa.bmp" ), 9 ) );
	assert( BITMAP_MANAGER->inset_bidirection_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "Koopa_Fire" ), TEXT( "Resource/Bitmap/Koopa_Fire.bmp" ), 2 ) );
	assert( BITMAP_MANAGER->inset_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "Item" ), TEXT( "Resource/Bitmap/Item.bmp" ) ) );
	assert( BITMAP_MANAGER->inset_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "Tile" ), TEXT( "Resource/Bitmap/Tile.bmp" ) ) );
	assert( BITMAP_MANAGER->inset_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "Ending" ), TEXT( "Resource/Bitmap/Ending.bmp" ) ) );
	assert( BITMAP_MANAGER->inset_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "UI" ), TEXT( "Resource/Bitmap/UI.bmp" ) ) );
	assert( BITMAP_MANAGER->inset_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "UI_Number" ), TEXT( "Resource/Bitmap/UI_Number.bmp" ) ) );
	assert( BITMAP_MANAGER->inset_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "UI_Accel" ), TEXT( "Resource/Bitmap/UI_Accel.bmp" ) ) );
	assert( BITMAP_MANAGER->inset_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "UI_Clear_Item" ), TEXT( "Resource/Bitmap/UI_Clear_Item.bmp" ) ) );

	return true;

}

bool CFight_Field::initialize_sprite(){

	SPRITE sprite;
	sprite.size					= { 16, 16 };
	sprite.origin				= { 1, 1 };
	sprite.step					= { 17, 17 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 73, 21 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Tile" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 1, 1 };
	sprite.step					= { 17, 17 };
	sprite.frame_range_begin	= { 56, 4 };
	sprite.frame_range_end		= { 60, 4 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Coin_Effect" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 5, 16 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Item" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 0, 2 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Item_Goal" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 0, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Small_Stand" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 1, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Small_Walk" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 3, 0 };
	sprite.frame_range_end		= { 4, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Small_Run" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 2, 0 };
	sprite.frame_range_end		= { 2, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Small_Jump" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 5, 0 };
	sprite.frame_range_end		= { 5, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Small_Power_Jump" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 11, 0 };
	sprite.frame_range_end		= { 11, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Small_Sliding" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 17, 0 };
	sprite.frame_range_end		= { 17, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Small_Die" ), sprite ) );

	sprite.size					= { 20, 32 };
	sprite.origin				= { 6, 0 };
	sprite.step					= { 32, 32 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 0, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Big_Stand" ), sprite ) );

	sprite.size					= { 20, 32 };
	sprite.origin				= { 6, 0 };
	sprite.step					= { 32, 32 };
	sprite.frame_range_begin	= { 1, 0 };
	sprite.frame_range_end		= { 2, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Big_Walk" ), sprite ) );

	sprite.size					= { 24, 32 };
	sprite.origin				= { 4, 0 };
	sprite.step					= { 32, 32 };
	sprite.frame_range_begin	= { 6, 0 };
	sprite.frame_range_end		= { 7, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Big_Run" ), sprite ) );
	
	sprite.size					= { 20, 32 };
	sprite.origin				= { 6, 0 };
	sprite.step					= { 32, 32 };
	sprite.frame_range_begin	= { 4, 0 };
	sprite.frame_range_end		= { 4, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Big_Jump" ), sprite ) );

	sprite.size					= { 24, 32 };
	sprite.origin				= { 4, 0 };
	sprite.step					= { 32, 32 };
	sprite.frame_range_begin	= { 8, 0 };
	sprite.frame_range_end		= { 8, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Big_Power_Jump" ), sprite ) );

	sprite.size					= { 20, 32 };
	sprite.origin				= { 6, 0 };
	sprite.step					= { 32, 32 };
	sprite.frame_range_begin	= { 16, 0 };
	sprite.frame_range_end		= { 16, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Mario_Big_Sliding" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 1, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Enemy_Goomba_Walk" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 2, 0 };
	sprite.frame_range_end		= { 3, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Enemy_Goomba_Die_Push" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 4, 0 };
	sprite.frame_range_end		= { 4, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Enemy_Goomba_Die_Damage" ), sprite ) );

	sprite.size					= { 16, 32 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 32 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 1, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Enemy_Turtle_Walk" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 2, 0 };
	sprite.frame_range_end		= { 5, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Enemy_Turtle_Stun" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 6, 0 };
	sprite.frame_range_end		= { 6, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Enemy_Turtle_Die_Damage" ), sprite ) );

	sprite.size					= { 32, 40 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 32, 40 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 0, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Koopa_Walk" ), sprite ) );

	sprite.size					= { 32, 40 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 32, 40 };
	sprite.frame_range_begin	= { 0, 1 };
	sprite.frame_range_end		= { 2, 1 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Koopa_Fire" ), sprite ) );

	sprite.size					= { 32, 40 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 32, 40 };
	sprite.frame_range_begin	= { 0, 2 };
	sprite.frame_range_end		= { 1, 2 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Koopa_Jump" ), sprite ) );

	sprite.size					= { 32, 40 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 32, 40 };
	sprite.frame_range_begin	= { 2, 2 };
	sprite.frame_range_end		= { 8, 2 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Koopa_Push" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 1, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Koopa_Fire_X" ), sprite ) );

	sprite.size					= { 16, 16 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 16, 16 };
	sprite.frame_range_begin	= { 0, 1 };
	sprite.frame_range_end		= { 1, 1 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Koopa_Fire_Diagonal" ), sprite ) );

	sprite.size					= { 10, 10 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 10, 10 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 9, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "UI_Number" ), sprite ) );

	sprite.size					= { 17, 9 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 17, 9 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 1, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "UI_Accel_Full" ), sprite ) );

	sprite.size					= { 9, 9 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 9, 9 };
	sprite.frame_range_begin	= { 0, 1 };
	sprite.frame_range_end		= { 1, 1 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "UI_Accel" ), sprite ) );
	
	sprite.size					= { 18, 20 };
	sprite.origin				= { 0, 0 };
	sprite.step					= { 18, 20 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 3, 0 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "UI_Clear_Item" ), sprite ) );

	return true;

}

bool CFight_Field::initialize_sound(){

	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Appears.wav",			TEXT( "Appears" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Clear.wav",			TEXT( "Clear" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Coin.wav",				TEXT( "Coin" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Game_Over.wav",		TEXT( "Game_Over" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Jump.wav",				TEXT( "Jump" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Lost.wav",				TEXT( "Lost" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Pipe.wav",				TEXT( "Pipe" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Power_Up.wav",			TEXT( "Power_Up" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Stomp.wav",			TEXT( "Stomp" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Kick.wav",				TEXT( "Kick" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Break.wav",			TEXT( "Break" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/BGM_Stage_0.mp3",		TEXT( "BGM_Stage_0" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/BGM_Stage_1.mp3",		TEXT( "BGM_Stage_1" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/BGM_Stage_2.mp3",		TEXT( "BGM_Stage_2" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/BGM_Bonus.mp3",		TEXT( "BGM_Bonus" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Boss_Clear.mp3",		TEXT( "Boss_Clear" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Fire.wav",				TEXT( "Fire" ) ) );
	assert( CSoundMgr::Get_Instance()->load( "Resource/Sound/Ending.mp3",			TEXT( "Ending" ) ) );

	return true;

}

CEnding_Field::CEnding_Field() : CField_Base{},
							   m_pbitmap{ nullptr }{
}

void CEnding_Field::initialize(){

	m_pbitmap = BITMAP_MANAGER->find_bitmap( TEXT( "Ending" ) );

	CSoundMgr::Get_Instance()->play_bgm( TEXT( "Ending" ) );
	
}

void CEnding_Field::release(){
}

CField_Base* CEnding_Field::update(){

	render();

	if( INPUT->get_state( VK_RETURN ).bpush ){
		return nullptr;
	}

	return this;

}

void CEnding_Field::render(){

	GdiTransparentBlt( GRAPHICS_MANAGER->get_dc(), 0, 0, 800, 600,
					   m_pbitmap->get_dc(), 0, 0, 800, 600,
					   RGB( 255, 0, 255 ) );

}
