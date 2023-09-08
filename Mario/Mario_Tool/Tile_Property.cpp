#include "Tile_Property.h"

#include "../API_Game/Graphics.h"

bool CTile_Index_Edit::initialize( HWND hparent_window, const RECT& rect0, const RECT& rect1 ){

	m_hwindow_x = CreateWindowEx( NULL, TEXT( "edit" ), nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
								rect0.left, rect0.top, rect0.right - rect0.left, rect0.bottom - rect0.top,
								hparent_window, NULL, GRAPHICS_MANAGER->get_instance_handle(), NULL );

	m_hwindow_y = CreateWindowEx( NULL, TEXT( "edit" ), nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
								rect1.left, rect1.top, rect1.right - rect1.left, rect1.bottom - rect1.top,
								hparent_window, NULL, GRAPHICS_MANAGER->get_instance_handle(), NULL );

	if( m_hwindow_x == NULL || m_hwindow_y == NULL ){
		return false;
	}

	return true;

}

void CTile_Index_Edit::set_tile_index( const Tile_Map::TILE_INDEX& index ){

	TCHAR str[ 256 ] = TEXT( "" );

	wsprintf( str, TEXT( "%d" ), ( int )index.ix );
	SetWindowText( m_hwindow_x, str );

	wsprintf( str, TEXT( "%d" ), ( int )index.iy );
	SetWindowText( m_hwindow_y, str );

}

bool CXY_Edit::initialize( HWND hparent_window, const RECT& rect0, const RECT& rect1 ){

	m_hwindow_x = CreateWindowEx( NULL, TEXT( "edit" ), nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
								rect0.left, rect0.top, rect0.right - rect0.left, rect0.bottom - rect0.top,
								hparent_window, NULL, GRAPHICS_MANAGER->get_instance_handle(), NULL );

	m_hwindow_y = CreateWindowEx( NULL, TEXT( "edit" ), nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
								rect1.left, rect1.top, rect1.right - rect1.left, rect1.bottom - rect1.top,
								hparent_window, NULL, GRAPHICS_MANAGER->get_instance_handle(), NULL );

	if( m_hwindow_x == NULL || m_hwindow_y == NULL ){
		return false;
	}

	return true;

}

void CXY_Edit::set_sprite_index( const XY& xy ){

	TCHAR str[ 256 ] = TEXT( "" );

	wsprintf( str, TEXT( "%d" ), ( int )xy.ix );
	SetWindowText( m_hwindow_x, str );

	wsprintf( str, TEXT( "%d" ), ( int )xy.iy );
	SetWindowText( m_hwindow_y, str );

}

bool CCollision_ComboBox::initialize( HWND hparent_window, const RECT& rect ){

	m_hwindow = CreateWindowEx( NULL, TEXT( "combobox" ), nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
								rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
								hparent_window, NULL, GRAPHICS_MANAGER->get_instance_handle(), NULL );

	if( m_hwindow == NULL ){
		return false;
	}

	m_collision_map.emplace( TEXT( "None" ),		   Tile_Map::ECollision_Type::None  );
	m_collision_map.emplace( TEXT( "Quad" ),		   Tile_Map::ECollision_Type::Quad  );
	m_collision_map.emplace( TEXT( "Quad_Up" ),		   Tile_Map::ECollision_Type::Quad_Up  );
	m_collision_map.emplace( TEXT( "Quad_Crushable" ), Tile_Map::ECollision_Type::Quad_Crushable  );

	for( auto& element : m_collision_map ){
		LRESULT lresult = SendMessage( m_hwindow, CB_ADDSTRING, NULL, ( LPARAM )element.first );
		if( lresult == CB_ERR ){
			MessageBox( NULL, TEXT( "Err" ), TEXT( "ComboBox" ), MB_OK );
		}
	}

	return true;

}

void CCollision_ComboBox::command( WPARAM wparameter, LPARAM lparameter, Tile_Map::TILE* ptile ){

	if( HIWORD( wparameter ) == CBN_SELCHANGE ){

		//선택된 문자열을 찾아 타일의 속성을 변경합니다.
		LRESULT lresult = SendMessage( m_hwindow, CB_GETCURSEL, 0, 0 );
		if( lresult != CB_ERR ){

			TCHAR str[ 64 ];
			SendMessage( m_hwindow, CB_GETLBTEXT, lresult, (LPARAM)str );

			auto find_iter = std::find_if( m_collision_map.begin(), m_collision_map.end(), [ & ]( auto& value ){
				return !_tcscmp( value.first, str );
			} );
			if( find_iter != m_collision_map.end() ){
				ptile->collision_type = find_iter->second;
			}

		}

	}

}

void CCollision_ComboBox::reset( Tile_Map::ECollision_Type type ){

	auto find_iter = std::find_if( m_collision_map.begin(), m_collision_map.end(), [ & ]( auto& value ){
		return value.second == type;
	} );

	if( find_iter != m_collision_map.end() ){
		LRESULT lresult = SendMessage( m_hwindow, CB_FINDSTRING, 0, ( LPARAM )find_iter->first );
		if( lresult != CB_ERR ){
			SendMessage( m_hwindow, CB_SETCURSEL, ( WPARAM )lresult, 0 );
		}
	}

}

HWND CCollision_ComboBox::get_window_handle(){
	return m_hwindow;
}

bool CAttribute_ComboBox::initialize( HWND hparent_window, const RECT& rect ){

	m_hwindow = CreateWindowEx( NULL, TEXT( "combobox" ), nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
								rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
								hparent_window, NULL, GRAPHICS_MANAGER->get_instance_handle(), NULL );

	if( m_hwindow == NULL ){
		return false;
	}

	m_attribute_map.emplace( TEXT( "None" ),					Tile_Map::EAttribute::None );
	m_attribute_map.emplace( TEXT( "Item_Coin" ),				Tile_Map::EAttribute::Item_Coin );
	m_attribute_map.emplace( TEXT( "Item_Mushroom" ),			Tile_Map::EAttribute::Item_Mushroom );
	m_attribute_map.emplace( TEXT( "Goal" ),					Tile_Map::EAttribute::Goal );
	m_attribute_map.emplace( TEXT( "Enemy_Goomba" ),			Tile_Map::EAttribute::Enemy_Goomba );
	m_attribute_map.emplace( TEXT( "Enemy_Turtle" ),			Tile_Map::EAttribute::Enemy_Turtle );
	m_attribute_map.emplace( TEXT( "Move_Direction_Left" ),		Tile_Map::EAttribute::Move_Direction_Left );
	m_attribute_map.emplace( TEXT( "Move_Direction_Right" ),	Tile_Map::EAttribute::Move_Direction_Right );
	m_attribute_map.emplace( TEXT( "Move_Normal" ),				Tile_Map::EAttribute::Move_Normal );
	m_attribute_map.emplace( TEXT( "Move_Jump" ),				Tile_Map::EAttribute::Move_Jump );
	m_attribute_map.emplace( TEXT( "Pipe" ),					Tile_Map::EAttribute::Pipe );
	m_attribute_map.emplace( TEXT( "Player_Position" ),			Tile_Map::EAttribute::Player_Position );
	m_attribute_map.emplace( TEXT( "Position_0" ),				Tile_Map::EAttribute::Position_0 );
	m_attribute_map.emplace( TEXT( "Position_1" ),				Tile_Map::EAttribute::Position_1 );
	m_attribute_map.emplace( TEXT( "Position_2" ),				Tile_Map::EAttribute::Position_2 );
	m_attribute_map.emplace( TEXT( "Position_3" ),				Tile_Map::EAttribute::Position_3 );
	m_attribute_map.emplace( TEXT( "Position_4" ),				Tile_Map::EAttribute::Position_4 );
	m_attribute_map.emplace( TEXT( "Position_5" ),				Tile_Map::EAttribute::Position_5 );
	m_attribute_map.emplace( TEXT( "Position_6" ),				Tile_Map::EAttribute::Position_6 );
	m_attribute_map.emplace( TEXT( "Position_7" ),				Tile_Map::EAttribute::Position_7 );
	m_attribute_map.emplace( TEXT( "Position_8" ),				Tile_Map::EAttribute::Position_8 );
	m_attribute_map.emplace( TEXT( "Position_9" ),				Tile_Map::EAttribute::Position_9 );
	m_attribute_map.emplace( TEXT( "Set_Position_0" ),			Tile_Map::EAttribute::Set_Position_0 );
	m_attribute_map.emplace( TEXT( "Set_Position_1" ),			Tile_Map::EAttribute::Set_Position_1 );
	m_attribute_map.emplace( TEXT( "Set_Position_2" ),			Tile_Map::EAttribute::Set_Position_2 );
	m_attribute_map.emplace( TEXT( "Set_Position_3" ),			Tile_Map::EAttribute::Set_Position_3 );
	m_attribute_map.emplace( TEXT( "Set_Position_4" ),			Tile_Map::EAttribute::Set_Position_4 );
	m_attribute_map.emplace( TEXT( "Set_Position_5" ),			Tile_Map::EAttribute::Set_Position_5 );
	m_attribute_map.emplace( TEXT( "Set_Position_6" ),			Tile_Map::EAttribute::Set_Position_6 );
	m_attribute_map.emplace( TEXT( "Set_Position_7" ),			Tile_Map::EAttribute::Set_Position_7 );
	m_attribute_map.emplace( TEXT( "Set_Position_8" ),			Tile_Map::EAttribute::Set_Position_8 );
	m_attribute_map.emplace( TEXT( "Set_Position_9" ),			Tile_Map::EAttribute::Set_Position_9 );
	m_attribute_map.emplace( TEXT( "Stage_0" ),					Tile_Map::EAttribute::Stage_0 );
	m_attribute_map.emplace( TEXT( "Stage_1" ),					Tile_Map::EAttribute::Stage_1 );
	m_attribute_map.emplace( TEXT( "Stage_2" ),					Tile_Map::EAttribute::Stage_2 );
	m_attribute_map.emplace( TEXT( "Stage_3" ),					Tile_Map::EAttribute::Stage_3 );
	m_attribute_map.emplace( TEXT( "Stage_4" ),					Tile_Map::EAttribute::Stage_4 );
	m_attribute_map.emplace( TEXT( "Bonus_0" ),					Tile_Map::EAttribute::Bonus_0 );
	m_attribute_map.emplace( TEXT( "Bonus_1" ),					Tile_Map::EAttribute::Bonus_1 );
	m_attribute_map.emplace( TEXT( "Bonus_2" ),					Tile_Map::EAttribute::Bonus_2 );
	m_attribute_map.emplace( TEXT( "Bonus_3" ),					Tile_Map::EAttribute::Bonus_3 );
	m_attribute_map.emplace( TEXT( "Bonus_4" ),					Tile_Map::EAttribute::Bonus_4 );
	m_attribute_map.emplace( TEXT( "Enemy_Koopa" ),				Tile_Map::EAttribute::Enemy_Koopa );


	for( auto& element : m_attribute_map ){
		LRESULT lresult = SendMessage( m_hwindow, CB_ADDSTRING, NULL, ( LPARAM )element.first );
		if( lresult == CB_ERR ){
			MessageBox( NULL, TEXT( "Err" ), TEXT( "ComboBox" ), MB_OK );
		}
	}

	return true;

}

void CAttribute_ComboBox::command( WPARAM wparameter, LPARAM lparameter, Tile_Map::EAttribute* pattribute ){

	if( HIWORD( wparameter ) == CBN_SELCHANGE ){

		//선택된 문자열을 찾아 타일의 속성을 변경합니다.
		LRESULT lresult = SendMessage( m_hwindow, CB_GETCURSEL, 0, 0 );
		if( lresult != CB_ERR ){

			TCHAR str[ 64 ];
			SendMessage( m_hwindow, CB_GETLBTEXT, lresult, (LPARAM)str );

			auto find_iter = std::find_if( m_attribute_map.begin(), m_attribute_map.end(), [ & ]( auto& value ){
				return !_tcscmp( value.first, str );
			} );
			if( find_iter != m_attribute_map.end() ){
				*pattribute = find_iter->second;
			}

		}

	}

}

void CAttribute_ComboBox::reset( Tile_Map::EAttribute type ){

	auto find_iter = std::find_if( m_attribute_map.begin(), m_attribute_map.end(), [ & ]( auto& value ){
		return value.second == type;
	} );

	if( find_iter != m_attribute_map.end() ){
		LRESULT lresult = SendMessage( m_hwindow, CB_FINDSTRING, 0, ( LPARAM )find_iter->first );
		if( lresult != CB_ERR ){
			SendMessage( m_hwindow, CB_SETCURSEL, ( WPARAM )lresult, 0 );
		}
	}

}

HWND CAttribute_ComboBox::get_window_handle(){
	return m_hwindow;
}


bool CTile_Property::initialize(){

    if( !initialize_window() ){
		return false;
	}
	if( !initialize_resource() ){
		return false;
	}

	InvalidateRect( m_hwindow, nullptr, true );

    return true;

}

void CTile_Property::render(){
}

void CTile_Property::command( WPARAM wparameter, LPARAM lparameter ){

	if( TILE_MAP->is_available_index( m_selected_index ) ){

		Tile_Map::TILE tile = TILE_MAP->get_tile( m_selected_index );

		/*
		if( ( HWND )lparameter == m_collision_combobox.get_window_handle() ){

			m_collision_combobox.command( wparameter, lparameter, &tile );
			TILE_MAP->set_tile( tile );

		}else*/ if( ( HWND )lparameter == m_attribute_combobox0.get_window_handle() ){

			m_attribute_combobox0.command( wparameter, lparameter, &tile.attribute0 );
			TILE_MAP->set_tile( tile );

		}else if( ( HWND )lparameter == m_attribute_combobox1.get_window_handle() ){

			m_attribute_combobox1.command( wparameter, lparameter, &tile.attribute1 );
			TILE_MAP->set_tile( tile );

		}else if( ( HWND )lparameter == m_attribute_combobox2.get_window_handle() ){

			m_attribute_combobox2.command( wparameter, lparameter, &tile.attribute2 );
			TILE_MAP->set_tile( tile );

		}else if( ( HWND )lparameter == m_attribute_combobox3.get_window_handle() ){

			m_attribute_combobox3.command( wparameter, lparameter, &tile.attribute3 );
			TILE_MAP->set_tile( tile );

		}

	}

	if( ( HWND )lparameter == m_hincrease_left ){
		TILE_MAP->increase_left();
	}else if( ( HWND )lparameter == m_hincrease_right ){
		TILE_MAP->increase_right();
	}else if( ( HWND )lparameter == m_hincrease_top ){
		TILE_MAP->increase_top();
	}else if( ( HWND )lparameter == m_hincrease_button ){
		TILE_MAP->increase_bottom();
	}else if( ( HWND )lparameter == m_hdecrease_left ){
		TILE_MAP->decrease_left();
	}else if( ( HWND )lparameter == m_hdecrease_right ){
		TILE_MAP->decrease_right();
	}else if( ( HWND )lparameter == m_hdecrease_top ){
		TILE_MAP->decrease_top();
	}else if( ( HWND )lparameter == m_hdecrease_button ){
		TILE_MAP->decrease_bottom();
	}

}

void CTile_Property::change_tile( const Tile_Map::TILE_INDEX& index ){

	m_selected_index = index;

	if( TILE_MAP->is_available_index( m_selected_index ) ){

		const Tile_Map::TILE& tile = TILE_MAP->get_tile( m_selected_index );

		m_tile_index_edit.set_tile_index( tile.index );
		m_xy_edit.set_sprite_index( tile.sprite_index );
		//m_collision_combobox.reset( tile.collision_type );
		m_attribute_combobox0.reset( tile.attribute0 );
		m_attribute_combobox1.reset( tile.attribute1 );
		m_attribute_combobox2.reset( tile.attribute2 );
		m_attribute_combobox3.reset( tile.attribute3 );


	}

	

}

const Tile_Map::TILE_INDEX& CTile_Property::get_selected_index() const{
	return m_selected_index;
}

LRESULT CTile_Property::message_processor( HWND hwindow, UINT imessage, WPARAM wparameter, LPARAM lparameter ){

	CTile_Property* ptile_property = ( CTile_Property* )GetWindowLongPtr( hwindow, GWLP_USERDATA );

	switch( imessage ){
	case WM_PAINT:

		ptile_property->render();

		break;
	case WM_COMMAND:

		ptile_property->command( wparameter, lparameter );

		break;
	}

    return DefWindowProc( hwindow, imessage, wparameter, lparameter );

}

bool CTile_Property::initialize_window(){

	WNDCLASSEX window_class;
	window_class.cbSize		   = sizeof( window_class );
	window_class.cbClsExtra	   = NULL;
	window_class.cbWndExtra	   = sizeof( this );
	window_class.hbrBackground = ( HBRUSH )GetStockObject( WHITE_BRUSH );
	window_class.hCursor       = LoadCursor( NULL, IDC_ARROW ) ;
	window_class.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
	window_class.hIconSm       = LoadIcon( NULL, IDI_APPLICATION );
	window_class.lpfnWndProc   = ( WNDPROC )message_processor;
	window_class.lpszClassName = TEXT( "Tile_Map" );
	window_class.lpszMenuName  = NULL;
	window_class.hInstance     = GRAPHICS_MANAGER->get_instance_handle();
	window_class.style         = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx( &window_class );

	m_work_rect = { 0, 0, 400, 600 };
	RECT adjust_window_rect = m_work_rect;
	AdjustWindowRect( &adjust_window_rect, WS_POPUP | WS_CAPTION, FALSE );

	m_hwindow = CreateWindowEx( NULL, window_class.lpszClassName, TEXT( "Tile property"  ), WS_POPUP | WS_CAPTION,
								CW_USEDEFAULT, CW_USEDEFAULT, adjust_window_rect.right - adjust_window_rect.left, adjust_window_rect.bottom - adjust_window_rect.top,
								NULL, NULL, GRAPHICS_MANAGER->get_instance_handle(), NULL );
	if( m_hwindow == NULL ){
		return false;
	}

	SetWindowLongPtr( m_hwindow, GWLP_USERDATA, ( LONG_PTR )this );

	ShowWindow( m_hwindow, SW_SHOWDEFAULT );
	UpdateWindow( m_hwindow );

	if( !m_tile_index_edit.initialize( m_hwindow, { 30, 30, 130, 60 }, { 160, 30, 260, 60 } )	||
		!m_xy_edit.initialize( m_hwindow, { 30, 70, 130, 100 }, { 160, 70, 260, 100 } )			||
		//!m_collision_combobox.initialize( m_hwindow, { 30, 110, 130, 200 } )					||
		!m_attribute_combobox0.initialize( m_hwindow, { 30, 150, 250, 1740 } )					||
		!m_attribute_combobox1.initialize( m_hwindow, { 30, 190, 250, 1780 } )					||
		!m_attribute_combobox2.initialize( m_hwindow, { 30, 230, 250, 1820 } )					||
		!m_attribute_combobox3.initialize( m_hwindow, { 30, 270, 250, 1860 } ) ){
		return false;
	}

	m_hincrease_left = CreateWindowEx( NULL, TEXT( "button" ), TEXT( "Left" ), WS_CHILD | WS_VISIBLE,
									   30, 350, 70, 35,
									   m_hwindow, NULL, NULL, nullptr );
	m_hincrease_right = CreateWindowEx( NULL, TEXT( "button" ), TEXT( "Right" ), WS_CHILD | WS_VISIBLE,
									    130, 350, 70, 35,
									    m_hwindow, NULL, NULL, nullptr );
	m_hincrease_top = CreateWindowEx( NULL, TEXT( "button" ), TEXT( "Top" ), WS_CHILD | WS_VISIBLE,
									  30, 400, 70, 35,
									  m_hwindow, NULL, NULL, nullptr );
	m_hincrease_button = CreateWindowEx( NULL, TEXT( "button" ), TEXT( "Bottom" ), WS_CHILD | WS_VISIBLE,
										 130, 400, 70, 35,
										 m_hwindow, NULL, NULL, nullptr );

	m_hdecrease_left = CreateWindowEx( NULL, TEXT( "button" ), TEXT( "Left" ), WS_CHILD | WS_VISIBLE,
									   30, 450, 70, 35,
									   m_hwindow, NULL, NULL, nullptr );
	m_hdecrease_right = CreateWindowEx( NULL, TEXT( "button" ), TEXT( "Right" ), WS_CHILD | WS_VISIBLE,
									    130, 450, 70, 35,
									    m_hwindow, NULL, NULL, nullptr );
	m_hdecrease_top = CreateWindowEx( NULL, TEXT( "button" ), TEXT( "Top" ), WS_CHILD | WS_VISIBLE,
									  30, 500, 70, 35,
									  m_hwindow, NULL, NULL, nullptr );
	m_hdecrease_button = CreateWindowEx( NULL, TEXT( "button" ), TEXT( "Bottom" ), WS_CHILD | WS_VISIBLE,
										 130, 500, 70, 35,
										 m_hwindow, NULL, NULL, nullptr );

	return true;

}

bool CTile_Property::initialize_resource(){
	return true;
}