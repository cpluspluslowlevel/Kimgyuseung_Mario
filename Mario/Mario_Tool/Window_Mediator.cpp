#include "Window_Mediator.h"

#include "Work.h"
#include "Sprite_Sheet.h"
#include "Tile_Property.h"

CWindow_Mediator::CWindow_Mediator( CWork* pwork, CSprite_Sheet* psprite_sheet, CTile_Property* ptile_property ) :  m_pwork{ pwork },
                                                                                                                    m_psprite_sheet{ psprite_sheet },
                                                                                                                    m_ptile_property{ ptile_property }{
}

void CWindow_Mediator::change_tile_property_tile( const Tile_Map::TILE_INDEX& index ){
    m_ptile_property->change_tile( index );
}

void CWindow_Mediator::set_tile_property_collision_type( Tile_Map::ECollision_Type collision ){

	Tile_Map::TILE tile = TILE_MAP->get_tile( m_ptile_property->get_selected_index() );

	tile.collision_type = collision;

	TILE_MAP->set_tile( tile );
	m_ptile_property->change_tile( tile.index );

}

XY CWindow_Mediator::get_selected_tile_sheet_index() const{
    return m_psprite_sheet->get_selected_index();
}