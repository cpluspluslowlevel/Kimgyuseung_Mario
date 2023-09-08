#ifndef _API_GAME_MATH_H_
#define _API_GAME_MATH_H_

#include "Global.h"

struct Vector2{

	float fx;
	float fy;

	Vector2& operator+=( const Vector2& rhs );
	Vector2& operator-=( const Vector2& rhs );
	Vector2& operator*=( float fscalar );
	Vector2& operator/=( float fscalar );

	bool operator==( const Vector2& rhs );
	bool operator!=( const Vector2& rhs );

	Vector2& operator=( const Vector2& rhs );

};

Vector2 operator+( const Vector2& lhs, const Vector2& rhs );
Vector2 operator-( const Vector2& lhs, const Vector2& rhs );
Vector2 operator*( const Vector2& xy, float fscalar );
Vector2 operator/( const Vector2& xy, float fscalar );

struct LINE{

	Vector2 pt0;
	Vector2 pt1;

};

float length( const Vector2& v );
float length_square( const Vector2& v );
Vector2 normalize( const Vector2& v );

float dot( const Vector2& v0, const Vector2& v1 );

Vector2& normalize( Vector2* pv );

float projection( const Vector2& v, const Vector2& vprojection );
Vector2 projection_vector( const Vector2& v, const Vector2& vprojection );

//선 위의 점에대해 조사합니다.
//line: 선
//pt: 점
//pdistance: 선과 점의 수직거리 입니다. nullptr일시 계산하지 않습니다.
//pprojection_parameter: 선 위에 투영한 점의 선에서의 파라미터 입니다. nullptr일시 계산하지 않습니다.
//pptprojection: 선 위에 투영한 점 입니다. nullptr일시 계산하지 않습니다.
void calc_line_upper_point( const LINE& line, const Vector2& pt, float* pdistance, float* pprojection_parameter, Vector2* pptprojection );

float calc_rect_left( const Vector2& pt, const Vector2& vscaling );
float calc_rect_right( const Vector2& pt, const Vector2& vscaling );
float calc_rect_top( const Vector2& pt, const Vector2& vscaling );
float calc_rect_bottom( const Vector2& pt, const Vector2& vscaling );

#endif