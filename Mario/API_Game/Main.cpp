#include <Windows.h>

#include "Graphics.h"
#include "MainGame.h"

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

	HWND hwindow = CreateWindowEx( NULL, window_class.lpszClassName, TEXT( "Win API Start!"  ), WS_OVERLAPPEDWINDOW,
								   CW_USEDEFAULT, CW_USEDEFAULT, adjust_window_rect.right - adjust_window_rect.left, adjust_window_rect.bottom - adjust_window_rect.top,
								   NULL, NULL, hinstance, NULL );

	ShowWindow( hwindow, SW_SHOWDEFAULT );
	UpdateWindow( hwindow );
	
	MainGame main_game{};
	if( main_game.initialize( hinstance, hwindow ) ){

		MSG message;
		ZeroMemory( &message, sizeof( message ) );
		while( message.message != WM_QUIT ){

			if( PeekMessage( &message, NULL, NULL, NULL, TRUE ) ){
				TranslateMessage( &message );
				DispatchMessage( &message );
			}else{
				if( !main_game.update() ){
					break;
				}
			}

		}

		main_game.release();

	}

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