#ifndef _MARIO_TOOL_TILE_PROPERTY_H_
#define _MARIO_TOOL_TILE_PROPERTY_H_

#include "../API_Game/Bitmap.h"
#include "../API_Game/Tile.h"

#include "Global.h"

class CTile_Index_Edit{
public:
    CTile_Index_Edit() = default;
    CTile_Index_Edit( const CTile_Index_Edit& ) = delete;
    CTile_Index_Edit( CTile_Index_Edit&& ) = delete;
    ~CTile_Index_Edit() = default;

    bool initialize( HWND hparent_window, const RECT& rect0, const RECT& rect1 );

    void set_tile_index( const Tile_Map::TILE_INDEX& index );


private:
    HWND m_hwindow_x;
    HWND m_hwindow_y;
};

class CXY_Edit{
public:
    CXY_Edit() = default;
    CXY_Edit( const CXY_Edit& ) = delete;
    CXY_Edit( CXY_Edit&& ) = delete;
    ~CXY_Edit() = default;

    bool initialize( HWND hparent_window, const RECT& rect0, const RECT& rect1 );

    void set_sprite_index( const XY& xy );


private:
    HWND m_hwindow_x;
    HWND m_hwindow_y;
};

class CCollision_ComboBox{
public:
    CCollision_ComboBox() = default;
    CCollision_ComboBox( const CCollision_ComboBox& ) = delete;
    CCollision_ComboBox( CCollision_ComboBox&& ) = delete;
    ~CCollision_ComboBox() = default;

    bool initialize( HWND hparent_window, const RECT& rect );

    void command( WPARAM wparameter, LPARAM lparameter, Tile_Map::TILE* ptile );

    void reset( Tile_Map::ECollision_Type type );

    HWND get_window_handle();

private:
    HWND m_hwindow;

    std::map< const TCHAR*, Tile_Map::ECollision_Type > m_collision_map;

};

class CAttribute_ComboBox{
public:
    CAttribute_ComboBox() = default;
    CAttribute_ComboBox( const CAttribute_ComboBox& ) = delete;
    CAttribute_ComboBox( CAttribute_ComboBox&& ) = delete;
    ~CAttribute_ComboBox() = default;

    bool initialize( HWND hparent_window, const RECT& rect );

    void command( WPARAM wparameter, LPARAM lparameter, Tile_Map::EAttribute* pattribute );

    void reset( Tile_Map::EAttribute type );

    HWND get_window_handle();

private:

    HWND m_hwindow;

    std::map< const TCHAR*, Tile_Map::EAttribute > m_attribute_map;

};


class CTile_Property{
public:
    CTile_Property() = default;
    CTile_Property( const CTile_Property& ) = delete;
    CTile_Property( CTile_Property&& ) = delete;
    ~CTile_Property() = default;

    bool initialize();

    void render();

    void command( WPARAM wparameter, LPARAM lparameter );

    void change_tile( const Tile_Map::TILE_INDEX& index );

    const Tile_Map::TILE_INDEX& get_selected_index() const;

    static LRESULT CALLBACK message_processor( HWND hwindow, UINT imessage, WPARAM wparameter, LPARAM lparameter );

private:

    bool initialize_window();
    bool initialize_resource();

    HWND m_hwindow;
    RECT m_work_rect;

    CTile_Index_Edit    m_tile_index_edit;
    CXY_Edit            m_xy_edit;
    CCollision_ComboBox m_collision_combobox;
    CAttribute_ComboBox m_attribute_combobox0;
    CAttribute_ComboBox m_attribute_combobox1;
    CAttribute_ComboBox m_attribute_combobox2;
    CAttribute_ComboBox m_attribute_combobox3;

	HWND m_hincrease_left;
	HWND m_hincrease_right;
	HWND m_hincrease_top;
	HWND m_hincrease_button;

	HWND m_hdecrease_left;
	HWND m_hdecrease_right;
	HWND m_hdecrease_top;
	HWND m_hdecrease_button;

    Tile_Map::TILE_INDEX m_selected_index;

};

#endif