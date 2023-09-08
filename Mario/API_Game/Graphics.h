#ifndef _API_GAME_GRAPHICS_H_
#define _API_GAME_GRAPHICS_H_

#include "Global.h"

API_GAME_GRAPHICS_NAMESPACE_BEGIN

class CGraphics{
public:

	bool initialize();
	bool release();

	void begin();
	void end();


	HINSTANCE get_instance_handle();

	HWND get_window_handle();

	HDC get_dc();

private:
	SINGLETON( CGraphics, HINSTANCE hinstance, HWND hwindow );

	HINSTANCE m_hinstance;
	HWND	  m_hwindow;

	HDC m_hdc;
	HDC m_hcompatible_dc;

	HBITMAP m_hold_bitmap;
	HBITMAP m_hcompatible_bitmap;

};

#define GRAPHICS_MANAGER Library::CSingleton< Graphics::CGraphics >::get()

API_GAME_GRAPHICS_NAMESPACE_END

#endif
