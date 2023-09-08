#include "Work.h"

#include "../API_Game/Input.h"
#include "../API_Game/Time.h"
#include "../API_Game/Graphics.h"
#include "../API_Game/Bitmap.h"
#include "../API_Game/Tile.h"

#include "Window_Mediator.h"

CWork::CWork() : m_vscroll{ 0.0f, 0.0f },
				 m_sprite_sheet{},
				 m_ptile_bitmap{ nullptr },
				 m_tile_animator{},
				 m_selected_tile_index{},
				 m_tile_property{}{
}

bool CWork::initialize(){

	Tile_Map::TILE_PROPERTY		tile_property{};
	Tile_Map::TILE_MAP_PROPERTY tile_map_property{};
	tile_property.vtile_size = { 4.0f, 4.0f };
	tile_map_property.tile_map_size = { 7, 21 };
	if( !TILE_MAP->reset( tile_property, tile_map_property ) ){
		return false;
	}

	TILE_MAP->load( "Resource/Data/Stage/TileMap0.txt" );

	//타일맵 크기를 확장합니다.
	//필요할때만 주석을 푸세요.
	//Tile_Map::TILE_MAP_PROPERTY resize_tile_map_property{};
	//resize_tile_map_property.m_tile_map_size = { 150, 21 };
	//TILE_MAP->resize( resize_tile_map_property );

	if( !m_sprite_sheet.initialize() || !m_tile_property.initialize() ){
		return false;
	}

	Library::CSingleton< CWindow_Mediator >::create( this, &m_sprite_sheet, &m_tile_property );

	return true;

}

void CWork::release(){
	Library::CSingleton< CWindow_Mediator >::release();
}

bool CWork::load_resource(){

	assert( BITMAP_MANAGER->inset_bitmap( GRAPHICS_MANAGER->get_window_handle(), TEXT( "Tile" ), TEXT( "Resource/Bitmap/Tile.bmp" ) ) );
	m_ptile_bitmap = BITMAP_MANAGER->find_bitmap( TEXT( "Tile" ) );

	const SPRITE* psprite = BITMAP_MANAGER->find_sprite( TEXT( "Tile_Sheet" ) );
	m_tile_animator.set_bitmap( m_ptile_bitmap );
	m_tile_animator.set_sprite( psprite );
	m_tile_animator.set_frame( { 0, 0 } );
	m_tile_animator.set_animation_tick_ms( 100 );

	return true;

}

void CWork::update(){

	update_scroll();
	input_drawing();
	input_select();
	input_save_load();

}

void CWork::render(){

	Vector2						vtile_size		= TILE_MAP->get_tile_property().vtile_size;
	const Tile_Map::TILE_INDEX& vtile_map_size	= TILE_MAP->get_tile_map_property().tile_map_size;

	//타일 맵을 그립니다.
	size_t istart_x = min( ( size_t )( m_vscroll.fx / vtile_size.fx ), ( size_t )( vtile_map_size.ix - 1LL ) );
	size_t istart_y = min( ( size_t )( m_vscroll.fy / vtile_size.fy ), ( size_t )( vtile_map_size.iy - 1LL ) );
	size_t iend_x   = min( istart_x + ( size_t )( 800.0f / vtile_size.fx ) + 1LL, ( size_t )( vtile_map_size.ix ) );
	size_t iend_y   = min( istart_y + ( size_t )( 600.0f / vtile_size.fy ) + 1LL, ( size_t )( vtile_map_size.iy ) );
	for( size_t iy = istart_y; iy < iend_y; ++iy ){
		for( size_t ix = istart_x; ix < iend_x; ++ix ){

			RECT rect;
			rect.left	= ( LONG )( -m_vscroll.fx + ( float )ix * vtile_size.fx );
			rect.top	= ( LONG )( -m_vscroll.fy + ( float )iy * vtile_size.fy );
			rect.right	= ( LONG )vtile_size.fx;
			rect.bottom = ( LONG )vtile_size.fy;

			const Tile_Map::TILE& tile = TILE_MAP->get_tile( { ( int )ix, ( int )iy } );

			m_tile_animator.set_frame( tile.sprite_index );
			m_tile_animator.render( GRAPHICS_MANAGER->get_dc(), rect, RGB( 255, 0, 255 ) );

		}
	}

	//타일 구분선을 그립니다.
	if( INPUT->get_state( VK_RETURN ).bdown ){
		for( float fy = fmodf( -m_vscroll.fy, vtile_size.fy ); fy <= 600.0f + vtile_size.fy; fy += vtile_size.fy ){
			
			MoveToEx( GRAPHICS_MANAGER->get_dc(), 0, ( int )fy, nullptr );
			LineTo( GRAPHICS_MANAGER->get_dc(), 800, ( int )fy );

		}

		for( float fx = fmodf( -m_vscroll.fx, vtile_size.fx ); fx <= 800.0f + vtile_size.fx; fx += vtile_size.fx ){

			MoveToEx( GRAPHICS_MANAGER->get_dc(), ( int )fx, 0, nullptr );
			LineTo( GRAPHICS_MANAGER->get_dc(), ( int )fx, 600 );

		}
	}

	//선택된 타일의 테두리를 그립니다.
	RECT rect;
	rect.left	= ( LONG )( -m_vscroll.fx + ( float )m_selected_tile_index.ix * vtile_size.fx );
	rect.top	= ( LONG )( -m_vscroll.fy + ( float )m_selected_tile_index.iy * vtile_size.fy );
	rect.right	= rect.left + ( LONG )vtile_size.fx;
	rect.bottom = rect.top + ( LONG )vtile_size.fy;

	HPEN hpen		= CreatePen( PS_DASHDOT, 1, RGB( 255, 0, 0 ) );
	HPEN hold_pen	= (HPEN)SelectObject( GRAPHICS_MANAGER->get_dc(), hpen );
	Rectangle( GRAPHICS_MANAGER->get_dc(), rect.left, rect.top, rect.right, rect.bottom );
	SelectObject( GRAPHICS_MANAGER->get_dc(), hold_pen );
	DeleteObject( hpen );

}

Vector2 CWork::get_scroll() const{
	return m_vscroll;
}
void CWork::input_drawing(){

	Tile_Map::TILE_INDEX index = calc_mouse_tile_index();
	if( TILE_MAP->is_available_index( index ) && is_mouse_in_workspace() ){

		Tile_Map::TILE tile = TILE_MAP->get_tile( index );

		if( INPUT->get_state( 'Q' ).bdown ){

			tile.sprite_index = WINDOW_MEDIATOR->get_selected_tile_sheet_index();
			TILE_MAP->set_tile( tile );

		}

	}

}

void CWork::input_select(){

	if( INPUT->get_state( VK_LBUTTON ).bpush && is_mouse_in_workspace() ){

		m_selected_tile_index = calc_mouse_tile_index();
		if( TILE_MAP->is_available_index( m_selected_tile_index ) ){
			WINDOW_MEDIATOR->change_tile_property_tile( m_selected_tile_index );
		}

    }

}

void CWork::input_save_load(){

	if( INPUT->get_state( 'S' ).bpush && 
		INPUT->get_state( VK_CONTROL ).bdown ){

		TILE_MAP->save( "Resource/Data/TileMap.txt" );

	}


}


void CWork::update_scroll(){

	if( INPUT->get_state( VK_LEFT ).bdown ){
		m_vscroll.fx -= 1500.0f * TIME_MANAGER->get_tick();
    }else if( INPUT->get_state( VK_RIGHT ).bdown ){
		m_vscroll.fx += 1500.0f * TIME_MANAGER->get_tick();
    }

	if( INPUT->get_state( VK_UP ).bdown ){
		m_vscroll.fy -= 1500.0f * TIME_MANAGER->get_tick();
    }else if( INPUT->get_state( VK_DOWN ).bdown ){
		m_vscroll.fy += 1500.0f * TIME_MANAGER->get_tick();
    }

}

bool CWork::is_mouse_in_workspace() const{

	POINT ptcursor;
	GetCursorPos( &ptcursor );
	ScreenToClient( GRAPHICS_MANAGER->get_window_handle(), &ptcursor );

	if( ptcursor.x < 0 || ptcursor.x > 800 ||
		ptcursor.y < 0 || ptcursor.y > 600 ) return false;

	return true;

}

Tile_Map::TILE_INDEX CWork::calc_mouse_tile_index() const{

	POINT ptcursor_int;
	GetCursorPos( &ptcursor_int );
	ScreenToClient( GRAPHICS_MANAGER->get_window_handle(), &ptcursor_int );

	Vector2 ptcursor{ ( float )ptcursor_int.x + m_vscroll.fx, ( float )ptcursor_int.y + m_vscroll.fy };

	return TILE_MAP->calc_tile_index( ptcursor );

}
