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

//�� ���� �������� �����մϴ�.
//line: ��
//pt: ��
//pdistance: ���� ���� �����Ÿ� �Դϴ�. nullptr�Ͻ� ������� �ʽ��ϴ�.
//pprojection_parameter: �� ���� ������ ���� �������� �Ķ���� �Դϴ�. nullptr�Ͻ� ������� �ʽ��ϴ�.
//pptprojection: �� ���� ������ �� �Դϴ�. nullptr�Ͻ� ������� �ʽ��ϴ�.
void calc_line_upper_point( const LINE& line, const Vector2& pt, float* pdistance, float* pprojection_parameter, Vector2* pptprojection );

float calc_rect_left( const Vector2& pt, const Vector2& vscaling );
float calc_rect_right( const Vector2& pt, const Vector2& vscaling );
float calc_rect_top( const Vector2& pt, const Vector2& vscaling );
float calc_rect_bottom( const Vector2& pt, const Vector2& vscaling );

#endif