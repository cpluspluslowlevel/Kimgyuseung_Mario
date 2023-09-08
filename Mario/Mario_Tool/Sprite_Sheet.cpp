#include "Sprite_Sheet.h"

#include "../API_Game/Graphics.h"
#include "../API_Game/Bitmap.h"

bool CSprite_Sheet::initialize(){

	m_psheet = nullptr;

    if( !initialize_window() ){
		return false;
	}
	if( !initialize_resource() ){
		return false;
	}

	InvalidateRect( m_hwindow, nullptr, true );

	constexpr int netflix = 14500;
	constexpr int light_house = 12000;
	constexpr int tv          = 14300;
	constexpr int phone = 129210 - 36000;
	constexpr int adobe = 12900;
	constexpr int bus = 24800 + 31900;
	constexpr int total = netflix + phone + light_house + tv + adobe + bus;

	constexpr int i = ( 10000 * 23 + 10000 * 8 + total );


    return true;

}

void CSprite_Sheet::render(){

	if( m_psheet != nullptr ){

		PAINTSTRUCT paint_struct{};
		HDC hdc = BeginPaint( m_hwindow, &paint_struct );

		GdiTransparentBlt( hdc, m_work_rect.left, m_work_rect.top, m_work_rect.right, m_work_rect.bottom,
						   m_psheet->get_dc(), m_work_rect.left, m_work_rect.top, m_work_rect.right, m_work_rect.bottom,
						   RGB( 255, 0, 255 ) );

		Rectangle( hdc, m_selected_index.ix * m_tile_size.ix, m_selected_index.iy * m_tile_size.iy,
						( m_selected_index.ix + 1 ) * m_tile_size.ix, ( m_selected_index.iy + 1 ) * m_tile_size.iy );

		EndPaint( m_hwindow, &paint_struct );
	}


}

void CSprite_Sheet::click( const XY& ptcursor ){

	if( ptcursor.ix >= 0 && ptcursor.ix < m_work_rect.right &&
		ptcursor.iy >= 0 && ptcursor.iy < m_work_rect.bottom ){
		
		m_selected_index.ix = ptcursor.ix / m_tile_size.ix;
		m_selected_index.iy = ptcursor.iy / m_tile_size.iy;

		InvalidateRect( m_hwindow, nullptr, true );

	}

}

const XY& CSprite_Sheet::get_selected_index() const{
	return m_selected_index;
}

LRESULT CSprite_Sheet::message_processor( HWND hwindow, UINT imessage, WPARAM wparameter, LPARAM lparameter ){

	CSprite_Sheet* psprite_sheet = ( CSprite_Sheet* )GetWindowLongPtr( hwindow, GWLP_USERDATA );

	switch( imessage ){
	case WM_PAINT:

		psprite_sheet->render();

		break;

	case WM_LBUTTONDOWN:

		psprite_sheet->click( { LOWORD( lparameter ), HIWORD( lparameter ) } );

		break;

	}

    return DefWindowProc( hwindow, imessage, wparameter, lparameter );

}

bool CSprite_Sheet::initialize_window(){

	WNDCLASSEX window_class;
	window_class.cbSize		   = sizeof( window_class );
	window_class.cbClsExtra	   = NULL;
	window_class.cbWndExtra	   = sizeof( this );
	window_class.hbrBackground = ( HBRUSH )GetStockObject( WHITE_BRUSH );
	window_class.hCursor       = LoadCursor( NULL, IDC_ARROW ) ;
	window_class.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
	window_class.hIconSm       = LoadIcon( NULL, IDI_APPLICATION );
	window_class.lpfnWndProc   = ( WNDPROC )message_processor;
	window_class.lpszClassName = TEXT( "Sprite_Sheet" );
	window_class.lpszMenuName  = NULL;
	window_class.hInstance     = GRAPHICS_MANAGER->get_instance_handle();
	window_class.style         = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx( &window_class );

	m_work_rect = { 0, 0, 1242, 358 };
	RECT adjust_window_rect = m_work_rect;
	AdjustWindowRect( &adjust_window_rect, WS_POPUP | WS_CAPTION, FALSE );

	m_hwindow = CreateWindowEx( NULL, window_class.lpszClassName, TEXT( "Sprite sheet"  ), WS_POPUP | WS_CAPTION,
								CW_USEDEFAULT, CW_USEDEFAULT, adjust_window_rect.right - adjust_window_rect.left, adjust_window_rect.bottom - adjust_window_rect.top,
								NULL, NULL, GRAPHICS_MANAGER->get_instance_handle(), NULL );
	if( m_hwindow == NULL ){
		return false;
	}

	SetWindowLongPtr( m_hwindow, GWLP_USERDATA, ( LONG_PTR )this );

	ShowWindow( m_hwindow, SW_SHOWDEFAULT );
	UpdateWindow( m_hwindow );

	return true;

}

bool CSprite_Sheet::initialize_resource(){

	assert( BITMAP_MANAGER->inset_bitmap( m_hwindow, TEXT( "Tile_Sheet" ), TEXT( "Resource/Bitmap/Tile.bmp" ) ) );
	m_psheet = BITMAP_MANAGER->find_bitmap( TEXT( "Tile_Sheet" ) );

	m_tile_size.ix = m_work_rect.right / 73;
	m_tile_size.iy = m_work_rect.bottom / 21;

	SPRITE sprite;
	sprite.size					= { 16, 16 };
	sprite.origin				= { 1, 1 };
	sprite.step					= { 17, 17 };
	sprite.frame_range_begin	= { 0, 0 };
	sprite.frame_range_end		= { 73, 21 };
	assert( BITMAP_MANAGER->insert_sprite( TEXT( "Tile_Sheet" ), sprite ) );


	m_selected_index = { 0, 0 };

	return true;

}