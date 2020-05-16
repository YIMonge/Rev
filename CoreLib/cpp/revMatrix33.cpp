#include "../include/revMatrix33.h"

revMatrix33::revMatrix33()
{
	Identity();
}

revMatrix33::revMatrix33(
		f32 m11, f32 m12, f32 m13,
		f32 m21, f32 m22, f32 m23,
		f32 m31, f32 m32, f32 m33) :
_11(m11), _12(m12), _13(m13),
_21(m21), _22(m22), _23(m23), 
_31(m31), _32(m32), _33(m33)
{
}

void revMatrix33::Add( const revMatrix33 &m )
{
	for( int i=0; i<9; ++i ){
		data[i] += m.data[i];
	}
}

void revMatrix33::Sub( const revMatrix33 &m )
{
	for( int i=0; i<9; ++i ){
		data[i] -= m.data[i];
	}
}


void revMatrix33::Multi( const revMatrix33& matrix )
{
	revMatrix33 Mat = *this;
	for( int i=0; i<3; i++ ){
		for( int j=0; j<3; j++ ){
			m[i][j]  = Mat.m[0][j] * matrix.m[i][0]
			+ Mat.m[1][j] * matrix.m[i][1]
			+ Mat.m[2][j] * matrix.m[i][2];
		}
	}
}

revMatrix33 revMatrix33::Multi(const revMatrix33& lhs, const revMatrix33& rhs)
{
	revMatrix33 Mat;
	for( int i=0; i<3; i++ ){
		for( int j=0; j<3; j++ ){
			Mat.m[i][j]  = lhs.m[0][j] * rhs.m[i][0]
			+ lhs.m[1][j] * rhs.m[i][1]
			+ lhs.m[2][j] * rhs.m[i][2];
		}
	}
	return Mat;
}

inline void revMatrix33::MultiScalar( f32 s )
{
	for( int i=0; i<9; ++i ){
		data[i] *= s;
	}
}

void revMatrix33::DivScalar( f32 s )
{
	for( int i=0; i<9; ++i ){
		data[i] /= s;
	}
}

void revMatrix33::Identity()
{
	for( int i=0; i<3; i++ ){
		for( int j=0; j<3; j++ ){
			if( i == j ){
				m[i][j] = 1;
			}
			else{
				m[i][j] = 0;
			}
		}
	}
}

revMatrix33 revMatrix33::Inverse()
{
	revMatrix33 matrix;

	matrix = *this;

	return matrix;
}

void revMatrix33::Scaling( const revVector2& vec )
{
	this->Identity();

	m[0][0] = vec.x;
	m[1][1] = vec.y;
}

void revMatrix33::Scaling( f32 x, f32 y )
{
	this->Identity();

	m[0][0] = x;
	m[1][1] = y;
}

void revMatrix33::Rotation( f32 radian )
{
	f32 val_sin, val_cos;

	val_sin = sinf( radian );
	val_cos = cosf( radian );

	this->Identity();

	m[0][1] =  val_cos;
	m[0][2] =  val_sin;
	m[1][1] = -val_sin;
	m[1][2] =  val_cos;
}

void revMatrix33::Transpose()
{
	revMatrix33 matrix = *this;

	for( int i=0; i<3; ++i ){
		for( int j=0;j<3; ++j ){
			m[i][j] = matrix.m[j][i];
		}
	}
}


revVector2 revMatrix33::Vector2Transform(const revVector2& vec)
{
	return revVector2(
		m[0][0] * vec.x + m[1][0] * vec.y + m[2][0],
		m[0][1] * vec.x + m[1][1] * vec.y + m[2][1]
		);
}


revVector2 revMatrix33::Vector2Transform(const revVector2& vec, const revMatrix33& matrix)
{
	return revVector2(
		matrix.m[0][0] * vec.x + matrix.m[1][0] * vec.y + matrix.m[2][0],
		matrix.m[0][1] * vec.x + matrix.m[1][1] * vec.y + matrix.m[2][1]
		);
}