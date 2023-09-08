#include "Tile.h"

API_GAME_TILE_MAP_NAMESPACE_USING

void CTile_Map::initialize(){

	for( auto& tile : m_tile_vector ){

		EAttribute attribute_array[] = { tile.attribute0, tile.attribute1, tile.attribute2, tile.attribute3 };
		for( auto attribute : attribute_array ){

			switch( attribute ){
			case EAttribute::Set_Position_0:
				m_position_array[ 0 ] = calc_tile_position( tile.index );
				break;
			case EAttribute::Set_Position_1:
				m_position_array[ 1 ] = calc_tile_position( tile.index );
				break;
			case EAttribute::Set_Position_2:
				m_position_array[ 2 ] = calc_tile_position( tile.index );
				break;
			case EAttribute::Set_Position_3:
				m_position_array[ 3 ] = calc_tile_position( tile.index );
				break;
			case EAttribute::Set_Position_4:
				m_position_array[ 4 ] = calc_tile_position( tile.index );
				break;
			case EAttribute::Set_Position_5:
				m_position_array[ 5 ] = calc_tile_position( tile.index );
				break;
			case EAttribute::Set_Position_6:
				m_position_array[ 6 ] = calc_tile_position( tile.index );
				break;
			case EAttribute::Set_Position_7:
				m_position_array[ 7 ] = calc_tile_position( tile.index );
				break;
			case EAttribute::Set_Position_8:
				m_position_array[ 8 ] = calc_tile_position( tile.index );
				break;
			case EAttribute::Set_Position_9:
				m_position_array[ 9 ] = calc_tile_position( tile.index );
				break;
			case EAttribute::Player_Position:
				m_ptplayer = calc_tile_position( tile.index );
				break;
			}

		}

	}

}

bool CTile_Map::reset( const TILE_PROPERTY& tile_property, const TILE_MAP_PROPERTY& tile_map_property ){

    m_tile_property     = tile_property;
    m_tile_map_property = tile_map_property;

    //메모리를 재할당 하고 기본값으로 초기화 합니다.
    CTile_Vector temp{};
    temp.resize( m_tile_map_property.tile_map_size.ix * m_tile_map_property.tile_map_size.iy );
    m_tile_vector.swap( temp );
    temp.clear();

    for( size_t iy = 0; iy < ( size_t )m_tile_map_property.tile_map_size.iy; ++iy ){
        for( size_t ix = 0; ix < ( size_t )m_tile_map_property.tile_map_size.ix; ++ix ){

            m_tile_vector[ iy * m_tile_map_property.tile_map_size.ix + ix ].index             = { ( int )ix, ( int )iy };
            m_tile_vector[ iy * m_tile_map_property.tile_map_size.ix + ix ].collision_type    = ECollision_Type::None;
            m_tile_vector[ iy * m_tile_map_property.tile_map_size.ix + ix ].sprite_index      = { 0, 0 };
            m_tile_vector[ iy * m_tile_map_property.tile_map_size.ix + ix ].attribute0        = EAttribute::None;
            m_tile_vector[ iy * m_tile_map_property.tile_map_size.ix + ix ].attribute1        = EAttribute::None;
            m_tile_vector[ iy * m_tile_map_property.tile_map_size.ix + ix ].attribute2        = EAttribute::None;
            m_tile_vector[ iy * m_tile_map_property.tile_map_size.ix + ix ].attribute3        = EAttribute::None;

        }
    }

    return true;

}

void CTile_Map::resize( const TILE_MAP_PROPERTY& tile_map_property ){

	assert( tile_map_property.tile_map_size.ix >= m_tile_map_property.tile_map_size.ix &&
			tile_map_property.tile_map_size.iy >= m_tile_map_property.tile_map_size.iy );


	//크기에 맞는 컨테이너를 만든후 값을 복사합니다.
	CTile_Vector temp{};
    temp.resize( tile_map_property.tile_map_size.ix * tile_map_property.tile_map_size.iy );

	for( size_t iy = 0; iy < ( size_t )m_tile_map_property.tile_map_size.iy; ++iy ){
        for( size_t ix = 0; ix < ( size_t )m_tile_map_property.tile_map_size.ix; ++ix ){

			if( iy < ( size_t )tile_map_property.tile_map_size.iy &&
				ix < ( size_t )tile_map_property.tile_map_size.ix ){

				temp[ iy * ( size_t )tile_map_property.tile_map_size.ix + ix ].index             = { ( int )ix, ( int )iy };
				temp[ iy * ( size_t )tile_map_property.tile_map_size.ix + ix ].collision_type    = ECollision_Type::None;
				temp[ iy * ( size_t )tile_map_property.tile_map_size.ix + ix ].sprite_index      = { 0, 0 };
				temp[ iy * ( size_t )tile_map_property.tile_map_size.ix + ix ].attribute0        = EAttribute::None;
				temp[ iy * ( size_t )tile_map_property.tile_map_size.ix + ix ].attribute1        = EAttribute::None;
				temp[ iy * ( size_t )tile_map_property.tile_map_size.ix + ix ].attribute2        = EAttribute::None;
				temp[ iy * ( size_t )tile_map_property.tile_map_size.ix + ix ].attribute3        = EAttribute::None;

			}else{
				temp[ iy * ( size_t )tile_map_property.tile_map_size.ix + ix ] = m_tile_vector[ iy * m_tile_map_property.tile_map_size.ix + ix ];
			}


        }
    }

    m_tile_vector.swap( temp );
    temp.clear();

	m_tile_map_property = tile_map_property;

}

TILE_INDEX CTile_Map::calc_tile_index( const Vector2& pt ) const{
    return { ( int )( pt.fx / m_tile_property.vtile_size.fx ),
             ( int )( pt.fy / m_tile_property.vtile_size.fy ) };
}

Vector2 CTile_Map::calc_tile_position( const TILE_INDEX& index ) const{
    return Vector2{ ( float )index.ix * m_tile_property.vtile_size.fx,
                    ( float )index.iy * m_tile_property.vtile_size.fy };
}

bool CTile_Map::is_available_index( const TILE_INDEX& index ) const{
    return ( index.ix >= 0 && index.ix < m_tile_map_property.tile_map_size.ix ) &&
           ( index.iy >= 0 && index.iy < m_tile_map_property.tile_map_size.iy );
}

void CTile_Map::increase_left(){

	CTile_Vector temp{};
    temp.resize( ( m_tile_map_property.tile_map_size.ix + 1 ) * m_tile_map_property.tile_map_size.iy );

	for( size_t iy = 0; iy < ( size_t )m_tile_map_property.tile_map_size.iy; ++iy ){
        for( size_t ix = 0; ix < ( size_t )m_tile_map_property.tile_map_size.ix + 1; ++ix ){

			if( ix == 0 ){

				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].collision_type    = ECollision_Type::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].sprite_index      = { 0, 0 };
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].attribute0        = EAttribute::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].attribute1        = EAttribute::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].attribute2        = EAttribute::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].attribute3        = EAttribute::None;

			}else{
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ] = m_tile_vector[ iy * m_tile_map_property.tile_map_size.ix + ( ix - 1 ) ];
			}

			temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].index = { ( int )ix, ( int )iy };

        }
    }

    m_tile_vector.swap( temp );
    temp.clear();

	m_tile_map_property.tile_map_size.ix += 1;

}

void CTile_Map::increase_right(){

	CTile_Vector temp{};
    temp.resize( ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) * ( size_t )m_tile_map_property.tile_map_size.iy );

	for( size_t iy = 0; iy < ( size_t )m_tile_map_property.tile_map_size.iy; ++iy ){
        for( size_t ix = 0; ix < ( size_t )m_tile_map_property.tile_map_size.ix + 1; ++ix ){

			if( ix == ( size_t )m_tile_map_property.tile_map_size.ix ){

				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].collision_type    = ECollision_Type::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].sprite_index      = { 0, 0 };
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].attribute0        = EAttribute::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].attribute1        = EAttribute::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].attribute2        = EAttribute::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].attribute3        = EAttribute::None;

			}else{
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ] = m_tile_vector[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix ];
			}

			temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix + 1 ) + ix ].index = { ( int )ix, ( int )iy };

        }
    }

    m_tile_vector.swap( temp );
    temp.clear();

	m_tile_map_property.tile_map_size.ix += 1;

}

void CTile_Map::increase_top(){

	CTile_Vector temp{};
    temp.resize( ( size_t )m_tile_map_property.tile_map_size.ix * ( ( size_t )m_tile_map_property.tile_map_size.iy + 1 ) );

	for( size_t iy = 0; iy < ( size_t )m_tile_map_property.tile_map_size.iy + 1; ++iy ){
        for( size_t ix = 0; ix < ( size_t )m_tile_map_property.tile_map_size.ix; ++ix ){

			if( iy == 0 ){

				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix ) + ix ].collision_type    = ECollision_Type::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix ) + ix ].sprite_index      = { 0, 0 };
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix ) + ix ].attribute0        = EAttribute::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix ) + ix ].attribute1        = EAttribute::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix ) + ix ].attribute2        = EAttribute::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix ) + ix ].attribute3        = EAttribute::None;

			}else{
				temp[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix ] = m_tile_vector[ ( iy - 1 ) * ( size_t )m_tile_map_property.tile_map_size.ix + ix ];
			}

			temp[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix ].index = { ( int )ix, ( int )iy };

        }
    }

    m_tile_vector.swap( temp );
    temp.clear();

	m_tile_map_property.tile_map_size.iy += 1;


}

void CTile_Map::increase_bottom(){

	CTile_Vector temp{};
    temp.resize( ( size_t )m_tile_map_property.tile_map_size.ix * ( ( size_t )m_tile_map_property.tile_map_size.iy + 1 ) );

	for( size_t iy = 0; iy < ( size_t )m_tile_map_property.tile_map_size.iy + 1; ++iy ){
        for( size_t ix = 0; ix < ( size_t )m_tile_map_property.tile_map_size.ix; ++ix ){

			if( iy == ( size_t )m_tile_map_property.tile_map_size.iy ){

				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix ) + ix ].collision_type    = ECollision_Type::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix ) + ix ].sprite_index      = { 0, 0 };
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix ) + ix ].attribute0        = EAttribute::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix ) + ix ].attribute1        = EAttribute::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix ) + ix ].attribute2        = EAttribute::None;
				temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix ) + ix ].attribute3        = EAttribute::None;

			}else{
				temp[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix ] = m_tile_vector[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix ];
			}

			temp[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix ].index = { ( int )ix, ( int )iy };

        }
    }

    m_tile_vector.swap( temp );
    temp.clear();

	m_tile_map_property.tile_map_size.iy += 1;

}

void CTile_Map::decrease_left(){

	assert( m_tile_map_property.tile_map_size.ix > 0 );

	CTile_Vector temp{};
    temp.resize( ( ( size_t )m_tile_map_property.tile_map_size.ix - 1 ) * ( size_t )m_tile_map_property.tile_map_size.iy );

	for( size_t iy = 0; iy < ( size_t )m_tile_map_property.tile_map_size.iy; ++iy ){
        for( size_t ix = 0; ix < ( size_t )m_tile_map_property.tile_map_size.ix - 1; ++ix ){
			temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix - 1 ) + ix ] = m_tile_vector[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix + 1 ];
			temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix - 1 ) + ix ].index = { ( int )ix, ( int )iy };
        }
    }

	m_tile_map_property.tile_map_size.ix -= 1;

	//교체합니다.
    m_tile_vector.swap( temp );
    temp.clear();


}

void CTile_Map::decrease_right(){

	assert( m_tile_map_property.tile_map_size.ix > 0 );

	CTile_Vector temp{};
    temp.resize( ( ( size_t )m_tile_map_property.tile_map_size.ix - 1 ) * ( size_t )m_tile_map_property.tile_map_size.iy );

	for( size_t iy = 0; iy < ( size_t )m_tile_map_property.tile_map_size.iy; ++iy ){
        for( size_t ix = 0; ix < ( size_t )m_tile_map_property.tile_map_size.ix - 1; ++ix ){
			temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix - 1 ) + ix ] = m_tile_vector[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix ];
			temp[ iy * ( ( size_t )m_tile_map_property.tile_map_size.ix - 1 ) + ix ].index = { ( int )ix, ( int )iy };
        }
    }

	m_tile_map_property.tile_map_size.ix -= 1;

	//교체합니다.
    m_tile_vector.swap( temp );
    temp.clear();

}

void CTile_Map::decrease_top(){

	assert( m_tile_map_property.tile_map_size.iy > 0 );

	CTile_Vector temp{};
    temp.resize( ( size_t )m_tile_map_property.tile_map_size.ix * ( ( size_t )m_tile_map_property.tile_map_size.iy - 1 ) );

	for( size_t iy = 0; iy < ( size_t )m_tile_map_property.tile_map_size.iy - 1; ++iy ){
        for( size_t ix = 0; ix < ( size_t )m_tile_map_property.tile_map_size.ix; ++ix ){
			temp[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix ] = m_tile_vector[ ( iy + 1 ) * ( size_t )m_tile_map_property.tile_map_size.ix + ix ];
			temp[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix ].index = { ( int )ix, ( int )iy };
        }
    }

	m_tile_map_property.tile_map_size.iy -= 1;

	//교체합니다.
    m_tile_vector.swap( temp );
    temp.clear();

}

void CTile_Map::decrease_bottom(){

	assert( m_tile_map_property.tile_map_size.iy > 0 );

	CTile_Vector temp{};
    temp.resize( m_tile_map_property.tile_map_size.ix * ( m_tile_map_property.tile_map_size.iy - 1 ) );

	for( size_t iy = 0; iy < ( size_t )m_tile_map_property.tile_map_size.iy - 1; ++iy ){
        for( size_t ix = 0; ix < ( size_t )m_tile_map_property.tile_map_size.ix; ++ix ){
			temp[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix ] = m_tile_vector[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix ];
			temp[ iy * ( size_t )m_tile_map_property.tile_map_size.ix + ix ].index = { ( int )ix, ( int )iy };
        }
    }

	m_tile_map_property.tile_map_size.iy -= 1;

}

bool CTile_Map::save( const char* sfile_path ){

    FILE* pfile = nullptr;

    auto destructor_technique = Library::make_destructor_technique([&]() {
        if( pfile != nullptr ){
            fclose( pfile );
            pfile = nullptr;
        }
    } );

    fopen_s( &pfile, sfile_path, "wb" );
    if( pfile == nullptr ){
        return false;
    }

    fwrite( &m_tile_property, sizeof( m_tile_property ), 1, pfile );
    fwrite( &m_tile_map_property, sizeof( m_tile_map_property ), 1, pfile );
    fwrite( &m_tile_vector[ 0 ], sizeof( TILE ), m_tile_map_property.tile_map_size.ix * m_tile_map_property.tile_map_size.iy, pfile );

    return true;

}

bool CTile_Map::load( const char* sfile_path ){

    FILE* pfile = nullptr;

    auto destructor_technique = Library::make_destructor_technique( [ & ]() {
        if( pfile != nullptr ){
            fclose( pfile );
            pfile = nullptr;
        }
    } );

    fopen_s( &pfile, sfile_path, "rb" );
    if( pfile == nullptr ){
        return false;
    }

    fread( &m_tile_property, sizeof( m_tile_property ), 1, pfile );
    fread( &m_tile_map_property, sizeof( m_tile_map_property ), 1, pfile );

    CTile_Vector temp{};
    temp.resize( m_tile_map_property.tile_map_size.ix * m_tile_map_property.tile_map_size.iy );
    m_tile_vector.swap( temp );
    temp.clear();

    fread( &m_tile_vector[ 0 ], sizeof( TILE ), m_tile_map_property.tile_map_size.ix * m_tile_map_property.tile_map_size.iy, pfile );

    return true;

}

void CTile_Map::set_tile_size( const Vector2& vtile_size ){
    m_tile_property.vtile_size = vtile_size;
}

void CTile_Map::set_tile( const TILE& tile ){
    assert( is_available_index( tile.index ) );
    m_tile_vector[ tile.index.iy * m_tile_map_property.tile_map_size.ix + tile.index.ix ] = tile;
}

const TILE_PROPERTY& CTile_Map::get_tile_property() const{
    return m_tile_property;
}

const TILE_MAP_PROPERTY& CTile_Map::get_tile_map_property() const{
    return m_tile_map_property;
}

const TILE& CTile_Map::get_tile( const TILE_INDEX& index ) const{
    assert( is_available_index( index ) );
    return m_tile_vector[ index.iy * m_tile_map_property.tile_map_size.ix + index.ix ];
}

const Vector2& CTile_Map::get_player_position() const{
	return m_ptplayer;
}

const Vector2& CTile_Map::get_position( DWORD dwposition  ) const{
	assert( dwposition - ( DWORD )EAttribute::Position_0 < 10 );
	return m_position_array[ ( size_t )dwposition - ( DWORD )EAttribute::Position_0 ];
}

Vector2 CTile_Map::get_map_size() const{

	Vector2 vscaling{
		m_tile_property.vtile_size.fx * m_tile_map_property.tile_map_size.ix,
		m_tile_property.vtile_size.fy * m_tile_map_property.tile_map_size.iy
	};

	return vscaling;

}

bool Tile_Map::is_quad_collision(  ECollision_Type collision ){
	return collision == ECollision_Type::Quad ||
		   collision == ECollision_Type::Quad_Crushable;
}

bool Tile_Map::is_ground_collision( ECollision_Type collision ){
	return collision == ECollision_Type::Quad_Up				||
		   collision == ECollision_Type::Diagonal_Right			||
		   collision == ECollision_Type::Diagonal_Right_Bottom	||
		   collision == ECollision_Type::Diagonal_Right_Middle	||
		   collision == ECollision_Type::Diagonal_Left			||
		   collision == ECollision_Type::Diagonal_Left_Bottom	||
		   collision == ECollision_Type::Diagonal_Left_Middle	||
		   is_quad_collision( collision );
}
