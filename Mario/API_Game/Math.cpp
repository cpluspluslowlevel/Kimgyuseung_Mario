#include "Math.h"

#include "Graphics.h"

Vector2& Vector2::operator+=( const Vector2& rhs ){
	fx += rhs.fx;
	fy += rhs.fy;
	return *this;
}

Vector2& Vector2::operator-=( const Vector2& rhs ){
	fx -= rhs.fx;
	fy -= rhs.fy;
	return *this;
}

Vector2& Vector2::operator*=( float fscalar ){
	fx *= fscalar;
	fy *= fscalar;
	return *this;
}

Vector2& Vector2::operator/=( float fscalar ){
	fx /= fscalar;
	fy /= fscalar;
	return *this;
}

bool Vector2::operator==( const Vector2& rhs ){
	return fx == rhs.fx && fy == rhs.fy;
}

bool Vector2::operator!=( const Vector2& rhs ){
	return !operator==( rhs );
}

Vector2& Vector2::operator=( const Vector2& rhs ){
	fx = rhs.fx;
	fy = rhs.fy;
	return *this;
}

Vector2 operator+( const Vector2& lhs, const Vector2& rhs ){
	return Vector2{ lhs.fx + rhs.fx, lhs.fy + rhs.fy };
}

Vector2 operator-( const Vector2& lhs, const Vector2& rhs ){
	return Vector2{ lhs.fx - rhs.fx, lhs.fy - rhs.fy };
}

Vector2 operator*( const Vector2& xy, float fscalar ){
	return Vector2{ xy.fx * fscalar, xy.fy * fscalar };
}

Vector2 operator/( const Vector2& xy, float fscalar ){
	return Vector2{ xy.fx / fscalar, xy.fy / fscalar };
}

float length( const Vector2& v ){
	return std::sqrtf( v.fx * v.fx + v.fy * v.fy );
}

float length_square( const Vector2& v ){
	return dot( v, v );
}

Vector2 normalize( const Vector2& v ){
	return v / length( v );
}

float dot( const Vector2 & v0, const Vector2 & v1 ){
	return v0.fx * v1.fx + v0.fy * v1.fy;
}

Vector2& normalize( Vector2* pv ){
	*pv /= length( *pv );
	return *pv;
}

float projection( const Vector2& v, const Vector2& vprojection ){
	//v dot w				= ||v||||w||cos
	//						= ||v||||w|| * projection / ||w||    ( projection 두 벡터로 만들어지는 '직각' 삼각형의 밑변 projection과 v의 길이는 다릅니다. )
	//						= ||v|| * projection
	//( v dot w ) / ||v||	= projection
	return dot( v, vprojection ) / length( v );
}

Vector2 projection_vector( const Vector2& v, const Vector2& vprojection ){
	//투영된 밑변의 길이를 벡터의 크기로 설정합니다.
	//projection_vector = projection * v / ||v||					( v / ||v||를 해야 벡터의 길이가 1고 그래야 크기를 곱했을때 벡터의 길이가 그 크기랑 동일해집니다. )
	//					= ( ( v dot w ) / ||v|| ) * ( v / ||v|| )
	//					= ( v dot w )v / ||v||^2
	//					==( v dot w )v / ( v dot v )
	return v * ( dot( v, vprojection ) / dot( v, v ) );
}

void calc_line_upper_point( const LINE& line, const Vector2& pt, float* pdistance, float* pprojection_parameter, Vector2* pptprojection ){

	Vector2 direction = line.pt1 - line.pt0;

	Vector2	normal{ -direction.fy, direction.fx };
	normalize( &normal );

	Vector2 target = pt - line.pt0;

	if( pdistance != nullptr ){
		*pdistance = dot( normal, target );
	}

	if( pprojection_parameter != nullptr ){
		*pprojection_parameter = dot( direction, target ) / dot( direction, direction );
	}

	if( pptprojection != nullptr ){
		*pptprojection = line.pt0 + projection_vector( direction, target );
	}

}

float calc_rect_left( const Vector2& pt, const Vector2& vscaling ){
	return pt.fx - vscaling.fx * 0.5f;
}

float calc_rect_right( const Vector2& pt, const Vector2& vscaling ){
	return pt.fx + vscaling.fx * 0.5f;
}

float calc_rect_top( const Vector2& pt, const Vector2& vscaling ){
	return pt.fy - vscaling.fy * 0.5f;
}

float calc_rect_bottom( const Vector2& pt, const Vector2& vscaling ){
	return pt.fy + vscaling.fy * 0.5f;
}
