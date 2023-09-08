#ifndef _SPRITE_SHEET_H_
#define _SPRITE_SHEET_H_

#include "../API_Game/Bitmap.h"

#include "Global.h"

class CSprite_Sheet{
public:
    CSprite_Sheet() = default;
    CSprite_Sheet( const CSprite_Sheet& ) = delete;
    CSprite_Sheet( CSprite_Sheet&& ) = delete;
    ~CSprite_Sheet() = default;

    bool initialize();

    void render();

    void click( const XY& ptcursor );

    const XY& get_selected_index() const;


    static LRESULT CALLBACK message_processor( HWND hwindow, UINT imessage, WPARAM wparameter, LPARAM lparameter );

private:

    bool initialize_window();
    bool initialize_resource();

    HWND m_hwindow;
    RECT m_work_rect;

    CBitmap_Base* m_psheet;
    XY            m_tile_size;

    //73 * 21
    XY m_selected_index;

};

#endif