#ifndef _API_GAME_MAIN_GAME_
#define _API_GAME_MAIN_GAME_

#include "Time.h"
#include "Field.h"

class MainGame{
public:
    MainGame();
    MainGame( const MainGame& ) = delete;
    MainGame( MainGame&& ) = delete;
    ~MainGame() = default;

    bool initialize( HINSTANCE hinstance, HWND hwindow );
    void release();

    bool update();

private:

    CField_Base* m_pfield;

    Time::CTimer m_fps_timer;
    unsigned int m_ifps;

};

#endif