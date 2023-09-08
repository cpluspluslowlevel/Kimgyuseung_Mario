#include "MainGame.h"

#include "SoundMgr.h"
#include "Graphics.h"
#include "Bitmap.h"
#include "Input.h"
#include "Time.h"
#include "Tile.h"
#include "Game_Object_Mediator.h"

MainGame::MainGame() : m_pfield{ new CFight_Field{} },
                       m_fps_timer{},
                       m_ifps{ 0 }{



}

bool MainGame::initialize( HINSTANCE hinstance, HWND hwindow ){

    CSoundMgr::Get_Instance()->Initialize();
    Library::CSingleton< Graphics::CGraphics >::create(hinstance, hwindow);
    Library::CSingleton< CBitmap_Manager >::create();
    Library::CSingleton< CInput >::create();
    Library::CSingleton< Time::CTime_Manager >::create();
    Library::CSingleton< Game_Object::CGame_Object_Manager >::create();
    Library::CSingleton< Tile_Map::CTile_Map >::create();

    if( !GRAPHICS_MANAGER->initialize() ){
        return false;
    }

    INPUT->insert( VK_RETURN );
	INPUT->insert( VK_SPACE );
	INPUT->insert( VK_LEFT );
	INPUT->insert( VK_RIGHT );
	INPUT->insert( VK_UP );
	INPUT->insert( VK_DOWN );
	INPUT->insert( 'Z' );
	INPUT->insert( 'X' );
	INPUT->insert( 'C' );
	INPUT->insert( '1' );
	INPUT->insert( '2' );
	INPUT->insert( '3' );
	INPUT->insert( '4' );
	INPUT->insert( '5' );

    if( !TIME_MANAGER->initialize() ){
        return false;
    }

    if( !GAME_OBJECT_MANAGER->initialize() ){
        return false;
    }

    m_pfield->initialize();

    return true;

}

void MainGame::release(){

    GAME_OBJECT_MANAGER->release();
    TIME_MANAGER->release();
    GRAPHICS_MANAGER->release();
    CSoundMgr::Get_Instance()->Release();

    
    Library::CSingleton< Tile_Map::CTile_Map >::release();
	Library::CSingleton< Game_Object::CGame_Object_Manager >::release();
    Library::CSingleton< Time::CTime_Manager >::release();
    Library::CSingleton< CInput >::release();
    Library::CSingleton< CBitmap_Manager >::release();
    Library::CSingleton< Graphics::CGraphics >::release();
    CSoundMgr::Destroy_Instance();

}

bool MainGame::update(){

    TIME_MANAGER->update();
    INPUT->update();


    GRAPHICS_MANAGER->begin();

    CField_Base* preturn_field = m_pfield->update();

    //m_pfield->render();
    //GAME_OBJECT_MANAGER->update();

    GRAPHICS_MANAGER->end();

    //fps를 계산합니다.
    ++m_ifps;
    if( m_fps_timer.signal( 1000 ) ){

        TCHAR str[ 256 ];
	    swprintf_s( str, TEXT( "FPS: %d" ), m_ifps );
	    SetWindowText( Library::CSingleton< Graphics::CGraphics >::get()->get_window_handle(), str );

        m_ifps = 0;

    }

    //필드가 바뀌었으면 교체합니다.
    if( preturn_field == m_pfield ){
        return true;
    }else if( preturn_field == nullptr ){
        return false;
    }else{
        m_pfield->release();
        Library::safe_delete( m_pfield );
        m_pfield = preturn_field;
        m_pfield->initialize();
    }

    return true;

}