#include "Bitmap.h"

CBitmap::CBitmap() : m_hwindow{ NULL },
					 m_hmemory_dc { NULL },
					 m_hold_bitmap{ NULL },
					 m_hbitmap{ NULL },
					 m_bitmap{}{
	ZeroMemory( &m_bitmap, sizeof( m_bitmap ) );
}

CBitmap::CBitmap( const CBitmap& lvalue ) : m_hwindow{ NULL },
											m_hmemory_dc{ NULL },
											m_hold_bitmap{ NULL },
											m_hbitmap{ NULL },
											m_bitmap{}{

	HDC hdc = GetDC( m_hwindow );

	m_hmemory_dc = CreateCompatibleDC( hdc );

	m_hbitmap = CreateCompatibleBitmap( lvalue.m_hmemory_dc, lvalue.m_bitmap.bmWidth, lvalue.m_bitmap.bmHeight );

	ReleaseDC( m_hwindow, hdc );

	m_hold_bitmap = ( HBITMAP )SelectObject( m_hmemory_dc, m_hbitmap );

	GetObject( m_hbitmap, sizeof( BITMAP ), &m_bitmap );

	//내용을 복사합니다.
	size_t ibuffer_size = m_bitmap.bmWidth * m_bitmap.bmHeight * m_bitmap.bmBitsPixel / 8;
	memcpy_s( m_bitmap.bmBits, ibuffer_size, lvalue.m_bitmap.bmBits, ibuffer_size );

}

CBitmap::CBitmap( CBitmap&& rvalue ) noexcept : m_hwindow{ rvalue.m_hwindow },
												m_hmemory_dc{ rvalue.m_hmemory_dc },
												m_hold_bitmap{ rvalue.m_hold_bitmap },
												m_hbitmap{ rvalue.m_hbitmap },
												m_bitmap{}{
	m_bitmap.bmBits       = rvalue.m_bitmap.bmBits;
	m_bitmap.bmBitsPixel  = rvalue.m_bitmap.bmBitsPixel;
	m_bitmap.bmPlanes     = rvalue.m_bitmap.bmPlanes;
	m_bitmap.bmHeight     = rvalue.m_bitmap.bmHeight;
	m_bitmap.bmType       = rvalue.m_bitmap.bmType;
	m_bitmap.bmWidth      = rvalue.m_bitmap.bmWidth;
	m_bitmap.bmWidthBytes = rvalue.m_bitmap.bmWidthBytes;
}

bool CBitmap::load( HWND hwindow, const TCHAR* sfile_path ){

	m_hwindow = hwindow;

	//DC와 비트맵을 만듭니다.
	HDC hdc = GetDC( m_hwindow );

	m_hmemory_dc = CreateCompatibleDC( hdc );

	ReleaseDC( m_hwindow, hdc );

	m_hbitmap = ( HBITMAP )LoadImage( NULL, sfile_path, IMAGE_BITMAP, 0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	if( m_hbitmap == NULL ) return false;

	m_hold_bitmap = ( HBITMAP )SelectObject( m_hmemory_dc, m_hbitmap );

	GetObject( m_hbitmap, sizeof( BITMAP ), &m_bitmap );

	return true;

}

void CBitmap::release(){
	if( m_hmemory_dc != NULL )	SelectObject( m_hmemory_dc, m_hold_bitmap);
	if( m_hbitmap != NULL )		DeleteObject( m_hbitmap );
	if( m_hmemory_dc != NULL )	DeleteDC( m_hmemory_dc );
}

void CBitmap::flip_x(){

	char* pbuffer = ( char* )m_bitmap.bmBits;
	size_t ibuffer_size = m_bitmap.bmWidth * m_bitmap.bmHeight * m_bitmap.bmBitsPixel / 8;
	for( size_t iy = 0; iy < m_bitmap.bmHeight; ++iy ){
		for( size_t ix = 0; ix < m_bitmap.bmWidth >> 1; ++ix ){
			
			char itemp = pbuffer[ iy * m_bitmap.bmWidthBytes + ix ];
			pbuffer[ iy * ( size_t )m_bitmap.bmWidthBytes + ix ] = pbuffer[ iy * ( size_t )m_bitmap.bmWidthBytes + ( ( size_t )m_bitmap.bmWidth - 1LL ) - ix ];
			pbuffer[ iy * ( size_t )m_bitmap.bmWidthBytes + ( ( size_t )m_bitmap.bmWidth - 1LL ) - ix ] = itemp;

		}
	}

}

void CBitmap::flip_y(){

	char* pbuffer = ( char* )m_bitmap.bmBits;
	size_t ibuffer_size = m_bitmap.bmWidth * m_bitmap.bmHeight * m_bitmap.bmBitsPixel / 8;
	for( size_t iy = 0; iy < m_bitmap.bmHeight >> 1; ++iy ){
		for( size_t ix = 0; ix < m_bitmap.bmWidth; ++ix ){
			
			char itemp = pbuffer[ iy * m_bitmap.bmWidthBytes + ix ];
			pbuffer[ iy * ( size_t )m_bitmap.bmWidthBytes + ix ] = pbuffer[ ( ( ( size_t )m_bitmap.bmHeight - 1LL ) - iy ) * ( size_t )m_bitmap.bmWidthBytes + ix ];
			pbuffer[ ( ( ( size_t )m_bitmap.bmHeight - 1LL ) - iy ) * ( size_t )m_bitmap.bmWidthBytes + ix ] = itemp;

		}
	}

}

void CBitmap::flip_sprite_x( int islice ){

	size_t islice_width = ( size_t )( m_bitmap.bmWidth / islice );
	size_t istart_x		= 0;
	size_t iend_x		= 0;
	size_t ipixel_size  = m_bitmap.bmBitsPixel / 8;
	char* pbuffer = ( char* )m_bitmap.bmBits;
	size_t ibuffer_size = m_bitmap.bmWidth * m_bitmap.bmHeight * m_bitmap.bmBitsPixel / 8;
	for( size_t iy = 0; iy < m_bitmap.bmHeight; ++iy ){

		for( size_t irect = 0; irect < islice; ++irect ){

			istart_x = irect * islice_width;
			iend_x   = istart_x + islice_width - 1;
			for( size_t irelative_x = 0; irelative_x < islice_width >> 1; ++irelative_x ){

				size_t ix		  = istart_x + irelative_x;
				size_t iinverse_x = iend_x - irelative_x;
				size_t ileft      = ( iy * ( size_t )m_bitmap.bmWidthBytes ) + ix * ipixel_size;
				size_t iright     = ( iy * ( size_t )m_bitmap.bmWidthBytes  ) + iinverse_x * ipixel_size;
				
				char itemp[ 4 ];
				memcpy_s( itemp, ipixel_size, &pbuffer[ ileft ], ipixel_size );
				memcpy_s( &pbuffer[ ileft ], ipixel_size, &pbuffer[ iright ], ipixel_size );
				memcpy_s( &pbuffer[ iright ], ipixel_size, itemp, ipixel_size );

			}

		}
		
	}

}

void CBitmap::flip_sprite_y( int islice ){
}

HDC CBitmap::get_dc(){
	return m_hmemory_dc;
}

HBITMAP CBitmap::get_bitmap(){
	return m_hbitmap;
}

const BITMAP& CBitmap::get_data() const{
	return m_bitmap;
}

CBidirection_Bitmap::CBidirection_Bitmap() : m_porigin_bitmap{ nullptr },
										   m_pflip_x_bitmap{ nullptr },
										   m_pselect_bitmap{ nullptr }{
}

CBidirection_Bitmap::CBidirection_Bitmap( const CBidirection_Bitmap& lvalue ) : m_porigin_bitmap{ nullptr },
																			    m_pflip_x_bitmap{ nullptr },
																			    m_pselect_bitmap{ nullptr }{

	m_porigin_bitmap = new CBitmap{ *lvalue.m_porigin_bitmap };
	m_pflip_x_bitmap = new CBitmap{ *m_porigin_bitmap };
	m_pflip_x_bitmap->flip_x();
	m_pselect_bitmap = m_porigin_bitmap;

}

CBidirection_Bitmap::CBidirection_Bitmap( CBidirection_Bitmap&& rvalue ) noexcept : m_porigin_bitmap{ rvalue.m_porigin_bitmap },
																					m_pflip_x_bitmap{ rvalue.m_pflip_x_bitmap },
																					m_pselect_bitmap{ rvalue.m_pselect_bitmap }{

	rvalue.m_porigin_bitmap = nullptr;
	rvalue.m_pflip_x_bitmap = nullptr;
	rvalue.m_pselect_bitmap = nullptr;

}

CBidirection_Bitmap::~CBidirection_Bitmap(){
	Library::safe_delete( m_porigin_bitmap );
	Library::safe_delete( m_pflip_x_bitmap );
}

bool CBidirection_Bitmap::load( HWND hwindow, const TCHAR* sfile_path, int islice ){

	m_porigin_bitmap = new CBitmap{};
	if( !m_porigin_bitmap->load( hwindow, sfile_path ) ){
		return false;
	}

	m_pflip_x_bitmap = new CBitmap{ *m_porigin_bitmap };
	m_pflip_x_bitmap->flip_sprite_x( islice );

	m_pselect_bitmap = m_porigin_bitmap;

	return true;

}

HDC CBidirection_Bitmap::get_dc(){
	return m_pselect_bitmap->get_dc();
}

HBITMAP CBidirection_Bitmap::get_bitmap(){
	return m_pselect_bitmap->get_bitmap();
}

const BITMAP& CBidirection_Bitmap::get_data() const{
	return m_pselect_bitmap->get_data();
}

void CBidirection_Bitmap::choice_origin(){
	m_pselect_bitmap = m_porigin_bitmap;
}

void CBidirection_Bitmap::choice_flip(){
	m_pselect_bitmap = m_pflip_x_bitmap;
}


CBitmap_Base* CBitmap_Manager::inset_bitmap( HWND hwindow, const TCHAR* skey, const TCHAR* sfile_path ){

	auto iter = std::find_if( m_pbitmap_map.begin(), m_pbitmap_map.end(), [ & ]( auto& rPair ){
		return !lstrcmp( skey, rPair.first );
	} );

	if( iter != m_pbitmap_map.end() ){
		return nullptr;
	}

	CBitmap* pbitmap = new CBitmap;
	assert( pbitmap->load( hwindow, sfile_path ) );
	m_pbitmap_map.emplace( skey, pbitmap );

	return pbitmap;

}

CBidirection_Bitmap* CBitmap_Manager::inset_bidirection_bitmap( HWND hwindow, const TCHAR* skey, const TCHAR* sfile_path, int islice ){
	

	auto iter = std::find_if( m_pbitmap_map.begin(), m_pbitmap_map.end(), [ & ]( auto& rPair ){
		return !lstrcmp( skey, rPair.first );
	} );

	if( iter != m_pbitmap_map.end() ){
		return nullptr;
	}

	CBidirection_Bitmap* pbitmap = new CBidirection_Bitmap{};
	assert( pbitmap->load( hwindow, sfile_path, islice ) );
	m_pbitmap_map.emplace( skey, pbitmap );

	return pbitmap;

}

CBitmap_Base* CBitmap_Manager::find_bitmap( const TCHAR* skey ){

	auto iter = std::find_if( m_pbitmap_map.begin(), m_pbitmap_map.end(), [ & ]( auto& rPair ){
		return !lstrcmp( skey, rPair.first );
	} );

	if( iter == m_pbitmap_map.end() ){
		return nullptr; 
	}

	return iter->second;

}

bool CBitmap_Manager::insert_sprite( const TCHAR* skey, const SPRITE& sprite ){

	auto iter = std::find_if( m_psprite_map.begin(), m_psprite_map.end(), [ & ]( auto& rPair ){
		return !lstrcmp( skey, rPair.first );
	} );

	if( iter != m_psprite_map.end() ){
		return false;
	}

	m_psprite_map.emplace( skey, sprite );

	return true;

}

const SPRITE* CBitmap_Manager::find_sprite( const TCHAR* skey ){

	auto iter = std::find_if( m_psprite_map.begin(), m_psprite_map.end(), [ & ]( auto& rPair ){
		return !lstrcmp( skey, rPair.first );
	} );

	if( iter == m_psprite_map.end() ){
		return nullptr;
	}

	return &iter->second;
}

void CBitmap_Manager::release(){
	
	for( auto& element : m_pbitmap_map ){
		Library::safe_delete( element.second );
	}
	m_pbitmap_map.clear();

}

CAnimator::CAnimator() : m_pbitmap{ nullptr },
						 m_psprite{ nullptr },
						 m_frame{ 0, 0 },
						 m_animation_tick_timer{ Time::CTimer{} },
						 m_dwanimation_tick_ms{ 0 },
						 m_bdirection{ true }{
}

void CAnimator::update(){
	if( m_animation_tick_timer.signal( m_dwanimation_tick_ms ) ){
		next_x();
	}
}

void CAnimator::inverse_update(){
	if( m_animation_tick_timer.signal( m_dwanimation_tick_ms ) ){
		back_x();
	}
}

void CAnimator::update_y(){
	if( m_animation_tick_timer.signal( m_dwanimation_tick_ms ) ){
		next_y();
	}
}

void CAnimator::inverse_update_y(){
	if( m_animation_tick_timer.signal( m_dwanimation_tick_ms ) ){
		back_y();
	}
}

void CAnimator::next_x(){
	m_frame.ix += 1;
	addressing();
}

void CAnimator::back_x(){
	m_frame.ix -= 1;
	addressing();
}

void CAnimator::next_y(){
	m_frame.iy -= 1;
	addressing();
}

void CAnimator::back_y(){
	m_frame.iy += 1;
	addressing();
}

void CAnimator::render( HDC hdc, const RECT& rect, COLORREF color_key ){

	RECT bitmap_rect = calc_bitmap_rect();

	GdiTransparentBlt( hdc, rect.left, rect.top, rect.right, rect.bottom,
					   m_pbitmap->get_dc(), bitmap_rect.left, bitmap_rect.top, bitmap_rect.right, bitmap_rect.bottom,
					   color_key );

}

RECT CAnimator::calc_bitmap_rect() const{
	return { ( LONG )( m_psprite->origin.ix + m_frame.ix * ( float )m_psprite->step.ix ),
			 ( LONG )( m_psprite->origin.iy + m_frame.iy * ( float )m_psprite->step.iy ),
			 m_psprite->size.ix,
			 m_psprite->size.iy };
}

void CAnimator::set_bitmap( CBitmap_Base* pbitmap ){
	m_pbitmap = pbitmap;
}

void CAnimator::set_sprite( const SPRITE* psprite ){
	m_psprite = psprite;
}

void CAnimator::set_frame( const XY& frame ){
	m_frame.ix = m_psprite->frame_range_begin.ix + frame.ix;
	m_frame.iy = m_psprite->frame_range_begin.iy + frame.iy;
	addressing();
}

void CAnimator::set_animation_tick_ms( DWORD dwanimation_tick_ms ){
	m_dwanimation_tick_ms = dwanimation_tick_ms;
}

CBitmap_Base* CAnimator::get_bitmap(){
	return m_pbitmap;
}

const SPRITE* CAnimator::get_sprite() const{
	return m_psprite;
}

const XY& CAnimator::get_frame() const{
	return m_frame;
}

DWORD CAnimator::get_animation_tick_ms() const{
	return m_dwanimation_tick_ms;
}

void CAnimator::addressing(){

	if( m_frame.ix < m_psprite->frame_range_begin.ix ){
		m_frame.ix = m_psprite->frame_range_end.ix;
	}else if( m_frame.ix > m_psprite->frame_range_end.ix ){
		m_frame.ix = m_psprite->frame_range_begin.ix;
	}

	if( m_frame.iy < m_psprite->frame_range_begin.iy ){
		m_frame.iy = m_psprite->frame_range_end.iy;
	}else if( m_frame.iy > m_psprite->frame_range_end.iy ){
		m_frame.iy = m_psprite->frame_range_begin.iy;
	}

}
