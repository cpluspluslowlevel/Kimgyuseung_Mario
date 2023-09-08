#ifndef _MARIO_TOOL_WORK_H_
#define _MARIO_TOOL_WORK_H_

#include "../API_Game/Math.h"
#include "../API_Game/Tile.h"

#include "Global.h"
#include "Sprite_Sheet.h"
#include "Tile_Property.h"

class CWork{
public:
	CWork();
	CWork( const CWork& ) = delete;
	CWork( CWork&& ) = delete;
	~CWork() = default;

	bool initialize();
	void release();
	bool load_resource();

	void update();
	void render();

	Vector2				 get_scroll() const;
	Tile_Map::TILE_INDEX get_selected_tile_index();

private:

	void input_drawing();
	void input_select();
	void input_save_load();

	void update_scroll();

	bool                 is_mouse_in_workspace() const;
	Tile_Map::TILE_INDEX calc_mouse_tile_index() const;

	Vector2 m_vscroll;

	//Sprite sheet
	CSprite_Sheet m_sprite_sheet;

	CBitmap_Base* m_ptile_bitmap;
	CAnimator     m_tile_animator;

	Tile_Map::TILE_INDEX m_selected_tile_index;

	//Tile property
	CTile_Property m_tile_property;

};

#endif