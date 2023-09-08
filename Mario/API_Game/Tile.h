#ifndef _API_GAME_TILE_H_
#define _API_GAME_TILE_H_

#include "Global.h"
#include "Math.h"

API_GAME_TILE_MAP_NAMESPACE_BEGIN

enum class ECollision_Type : DWORD{
    Quad,
    Quad_Up,
	Quad_Crushable,
	Diagonal_Right,				//대간선. 왼쪽에서 오른쪽으로 올라갑니다.
	Diagonal_Right_Bottom,		//대각선. 왼쪽에서 오른쪽으로 올라가며 사각형의 절반 만큼만 올라가는데 이건 사각형의 바닥부터 중간까지 올라갑니다.
	Diagonal_Right_Middle,		//대각선. 왼쪽에서 오른쪽으로 올라가며 사각형의 절반 만큼만 올라가는데 이건 사각형의 중간부터 정상까지 올라갑니다.
	Diagonal_Left,				//대각선. 오른쪽에서 왼쪽으로 올라갑니다.
	Diagonal_Left_Bottom,		//대각선. 오른쪽에서 왼쪽으로 올라가며 사각형의 절반 만큼만 올라가는데 이건 사각형의 바닥부터 중간까지 올라갑니다.
	Diagonal_Left_Middle,		//대각선. 오른쪽에서 왼쪽으로 올라가며 사각형의 절반 만큼만 올라가는데 이건 사각형의 중간부터 정상까지 올라갑니다.
    None
};

enum class EAttribute : DWORD{
    None,
	Item_Coin,
	Item_Mushroom,
    Goal,
    Enemy_Goomba,
    Enemy_Turtle,
    Move_Direction_Left,
    Move_Direction_Right,
    Move_Normal,
    Move_Jump,
	Pipe,
	Position_0,
	Position_1,
	Position_2,
	Position_3,
	Position_4,
	Position_5,
	Position_6,
	Position_7,
	Position_8,
	Position_9,
	Set_Position_0,
	Set_Position_1,
	Set_Position_2,
	Set_Position_3,
	Set_Position_4,
	Set_Position_5,
	Set_Position_6,
	Set_Position_7,
	Set_Position_8,
	Set_Position_9,
	Player_Position,
	Stage_0,
	Stage_1,
	Stage_2,
	Stage_3,
	Stage_4,
	Bonus_0,
	Bonus_1,
	Bonus_2,
	Bonus_3,
	Bonus_4,
	Enemy_Koopa

};

struct TILE_INDEX{
    long long ix;
    long long iy;
};

struct TILE{

    TILE_INDEX index;

    ECollision_Type collision_type;

    XY sprite_index;

    EAttribute attribute0;
    EAttribute attribute1;
    EAttribute attribute2;
    EAttribute attribute3;

};

struct TILE_PROPERTY{
    Vector2 vtile_size;
};

struct TILE_MAP_PROPERTY{
    TILE_INDEX tile_map_size;
};

class CTile_Map{
    SINGLETON_DEFAULT( CTile_Map );
public:

    typedef std::vector< TILE > CTile_Vector;

public:

	void initialize();

    bool reset( const TILE_PROPERTY& tile_property, const TILE_MAP_PROPERTY& tile_map_property );
	void resize( const TILE_MAP_PROPERTY& tile_map_property );

    TILE_INDEX  calc_tile_index( const Vector2& pt ) const;
    Vector2     calc_tile_position( const TILE_INDEX& index ) const;

    bool is_available_index( const TILE_INDEX& index ) const;

	void increase_left();
	void increase_right();
	void increase_top();
	void increase_bottom();

	void decrease_left();
	void decrease_right();
	void decrease_top();
	void decrease_bottom();

    bool save( const char* sfile_path );
    bool load( const char* sfile_path );

    void set_tile_size( const Vector2& vtile_size );
    void set_tile( const TILE& tile );

    const TILE_PROPERTY&       get_tile_property() const;
    const TILE_MAP_PROPERTY&   get_tile_map_property() const;
    const TILE&                get_tile( const TILE_INDEX& index ) const;

	const Vector2& get_player_position() const;
	const Vector2& get_position( DWORD dwposition  ) const;

	Vector2 get_map_size() const;

private:

    TILE_PROPERTY m_tile_property;

    TILE_MAP_PROPERTY   m_tile_map_property;
    CTile_Vector        m_tile_vector;

	Vector2 m_ptplayer;

	//타일 맵에서 설정된 위치. 용도는 정해져있지않고 정의하기 나름입니다. 예) 파이프에 들어갔을때 이동할 위치
	Vector2 m_position_array[ 10 ];

};

//부딫히는 벽인지 확인합니다.
bool is_quad_collision(  ECollision_Type collision );

//위에 설 수 있는지 확인합니다.
bool is_ground_collision( ECollision_Type collision );

#define TILE_MAP Library::CSingleton< Tile_Map::CTile_Map >::get()

API_GAME_TILE_MAP_NAMESPACE_END

#endif