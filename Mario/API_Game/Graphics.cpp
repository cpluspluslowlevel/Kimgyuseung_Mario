#include "Graphics.h"

API_GAME_GRAPHICS_NAMESPACE_USING;

CGraphics::CGraphics( HINSTANCE hinstance, HWND hwindow ) : m_hinstance{ hinstance },
															m_hwindow( hwindow ),
															m_hdc{ NULL },
														    m_hcompatible_dc{ NULL },
															m_hold_bitmap{ NULL },
															m_hcompatible_bitmap{ NULL }{
	
}

bool CGraphics::initialize(){

	m_hdc				 = GetDC( m_hwindow );
	m_hcompatible_dc	 = CreateCompatibleDC( m_hdc );
	m_hcompatible_bitmap = CreateCompatibleBitmap( m_hdc, 800, 600 );
	m_hold_bitmap        = ( HBITMAP )SelectObject( m_hcompatible_dc, m_hcompatible_bitmap );

	return true;

}

bool CGraphics::release(){

	SelectObject( m_hcompatible_dc, m_hold_bitmap );
	DeleteObject( m_hcompatible_bitmap );
	DeleteDC( m_hcompatible_dc );
	ReleaseDC( m_hwindow, m_hdc );

	return true;

}

void CGraphics::begin(){
	Rectangle( get_dc(), -10, -10, 810, 610 );
}

void CGraphics::end(){

	BitBlt( m_hdc, 0, 0, 800, 600, m_hcompatible_dc, 0, 0, SRCCOPY );

	

}

HINSTANCE CGraphics::get_instance_handle(){
	return m_hinstance;
}

HWND CGraphics::get_window_handle(){
	return m_hwindow;
}

HDC CGraphics::get_dc(){
	return m_hcompatible_dc;
}
