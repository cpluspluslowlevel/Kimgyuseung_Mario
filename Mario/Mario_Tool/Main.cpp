#include <Windows.h>

#include "../API_Game/Input.h"
#include "../API_Game/Time.h"
#include "../API_Game/Graphics.h"
#include "../API_Game/Bitmap.h"
#include "../API_Game/Tile.h"

#include "Work.h"

LRESULT CALLBACK message_processor( HWND hwindow, UINT imessage, WPARAM wparameter, LPARAM lparameter );

INT APIENTRY WinMain( HINSTANCE hinstance, HINSTANCE hprev_instance, LPSTR scommand, INT ishow_mode ){

	WNDCLASSEX window_class;
	window_class.cbSize		   = sizeof( window_class );
	window_class.cbClsExtra	   = NULL;
	window_class.cbWndExtra	   = NULL;
	window_class.hbrBackground = ( HBRUSH )GetStockObject( WHITE_BRUSH );
	window_class.hCursor       = LoadCursor( NULL, IDC_ARROW ) ;
	window_class.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
	window_class.hIconSm       = LoadIcon( NULL, IDI_APPLICATION );
	window_class.lpfnWndProc   = ( WNDPROC )message_processor;
	window_class.lpszClassName = TEXT( "Window" );
	window_class.lpszMenuName  = NULL;
	window_class.hInstance     = hinstance;
	window_class.style         = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx( &window_class );

	RECT adjust_window_rect{ 0, 0, 800, 600 };
	AdjustWindowRect( &adjust_window_rect, WS_OVERLAPPEDWINDOW, FALSE );

	HWND hwindow = CreateWindowEx( NULL, window_class.lpszClassName, TEXT( "Mario_Tool"  ), WS_OVERLAPPEDWINDOW,
								   CW_USEDEFAULT, CW_USEDEFAULT, adjust_window_rect.right - adjust_window_rect.left, adjust_window_rect.bottom - adjust_window_rect.top,
								   NULL, NULL, hinstance, NULL );

	ShowWindow( hwindow, SW_SHOWDEFAULT );
	UpdateWindow( hwindow );


	Library::CSingleton< CInput >::create();
	Library::CSingleton< Time::CTime_Manager >::create();
	Library::CSingleton< Graphics::CGraphics >::create( hinstance, hwindow );
	Library::CSingleton< CBitmap_Manager >::create();
	Library::CSingleton< Tile_Map::CTile_Map >::create();

	if( !GRAPHICS_MANAGER->initialize() ||
		!TIME_MANAGER->initialize() ){
		return 0;
	}

	INPUT->insert( VK_LBUTTON );
	INPUT->insert( VK_RBUTTON );
	INPUT->insert( VK_LEFT );
	INPUT->insert( VK_RIGHT );
	INPUT->insert( VK_UP );
	INPUT->insert( VK_DOWN );
	INPUT->insert( VK_CONTROL );
	INPUT->insert( VK_DELETE );
	INPUT->insert( VK_RETURN );
	INPUT->insert( 'Q' );
	INPUT->insert( '1' );
	INPUT->insert( '2' );
	INPUT->insert( '3' );
	INPUT->insert( '4' );
	INPUT->insert( 'S' );

	CWork work;
	if( !work.initialize() || !work.load_resource() ){
		return 0;
	}

	MSG message;
	ZeroMemory( &message, sizeof( message ) );
	while( message.message != WM_QUIT ){

		if( PeekMessage( &message, NULL, NULL, NULL, TRUE ) ){
			TranslateMessage( &message );
			DispatchMessage( &message );
		}else{

			TIME_MANAGER->update();
			INPUT->update();
			work.update();

			GRAPHICS_MANAGER->begin();
			work.render();
			GRAPHICS_MANAGER->end();

		}

	}

	Library::CSingleton< Tile_Map::CTile_Map >::release();
	Library::CSingleton< CBitmap_Manager >::release();
	Library::CSingleton< Graphics::CGraphics >::release();
	Library::CSingleton< Time::CTime_Manager >::release();
	Library::CSingleton< CInput >::release();

	return 0;

}

//실 작업내용
LRESULT CALLBACK message_processor( HWND hwindow, UINT imessage, WPARAM wparameter, LPARAM lparameter ){

	switch( imessage ){
	case WM_KEYDOWN:
		if( wparameter == VK_ESCAPE ) DestroyWindow( hwindow );
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	}

	return DefWindowProc( hwindow, imessage, wparameter, lparameter );

}