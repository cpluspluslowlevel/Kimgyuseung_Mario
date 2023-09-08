#ifndef _MARIO_TOOL_WINDOW_MEDIATOR_H_
#define _MARIO_TOOL_WINDOW_MEDIATOR_H_

#include "../API_Game/Game_Object.h"
#include "../API_Game/Tile.h"

#include "Global.h"

class CWindow_Mediator{
    SINGLETON( CWindow_Mediator, class CWork* pwork, class CSprite_Sheet* psprite_sheet, class CTile_Property* ptile_property );
public:

    void change_tile_property_tile( const Tile_Map::TILE_INDEX& index );
	void set_tile_property_collision_type( Tile_Map::ECollision_Type collision );

    //현재 시트에 선택된 타일 인덱스를 얻습니다.
    XY get_selected_tile_sheet_index() const;

private:
    class CWork*            m_pwork;
    class CSprite_Sheet*    m_psprite_sheet;
    class CTile_Property*   m_ptile_property;
};

#define WINDOW_MEDIATOR Library::CSingleton< CWindow_Mediator >::get()

#endif