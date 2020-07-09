#ifndef __REVMATRIX44_INL__
#define __REVMATRIX44_INL__
#include "revTypedef.h"

inline void revMatrix44::Mul( const revMatrix44& matrix )
{
	revMatrix44 mat = *this;
	f32 x = mat.m[0][0];
	f32 y = mat.m[0][1];
	f32 z = mat.m[0][2];
	f32 w = mat.m[0][3];
	m[0][0] = (matrix.m[0][0] * x) + (matrix.m[1][0] * y) + (matrix.m[2][0] * z) + (matrix.m[3][0] * w);
	m[0][1] = (matrix.m[0][1] * x) + (matrix.m[1][1] * y) + (matrix.m[2][1] * z) + (matrix.m[3][1] * w);
	m[0][2] = (matrix.m[0][2] * x) + (matrix.m[1][2] * y) + (matrix.m[2][2] * z) + (matrix.m[3][2] * w);
	m[0][3] = (matrix.m[0][3] * x) + (matrix.m[1][3] * y) + (matrix.m[2][3] * z) + (matrix.m[3][3] * w);

	x = mat.m[1][0];
	y = mat.m[1][1];
	z = mat.m[1][2];
	w = mat.m[1][3];
	m[1][0] = (matrix.m[0][0] * x) + (matrix.m[1][0] * y) + (matrix.m[2][0] * z) + (matrix.m[3][0] * w);
	m[1][1] = (matrix.m[0][1] * x) + (matrix.m[1][1] * y) + (matrix.m[2][1] * z) + (matrix.m[3][1] * w);
	m[1][2] = (matrix.m[0][2] * x) + (matrix.m[1][2] * y) + (matrix.m[2][2] * z) + (matrix.m[3][2] * w);
	m[1][3] = (matrix.m[0][3] * x) + (matrix.m[1][3] * y) + (matrix.m[2][3] * z) + (matrix.m[3][3] * w);

	x = mat.m[2][0];
	y = mat.m[2][1];
	z = mat.m[2][2];
	w = mat.m[2][3];
	m[2][0] = (matrix.m[0][0] * x) + (matrix.m[1][0] * y) + (matrix.m[2][0] * z) + (matrix.m[3][0] * w);
	m[2][1] = (matrix.m[0][1] * x) + (matrix.m[1][1] * y) + (matrix.m[2][1] * z) + (matrix.m[3][1] * w);
	m[2][2] = (matrix.m[0][2] * x) + (matrix.m[1][2] * y) + (matrix.m[2][2] * z) + (matrix.m[3][2] * w);
	m[2][3] = (matrix.m[0][3] * x) + (matrix.m[1][3] * y) + (matrix.m[2][3] * z) + (matrix.m[3][3] * w);

	x = mat.m[3][0];
	y = mat.m[3][1];
	z = mat.m[3][2];
	w = mat.m[3][3];
	m[3][0] = (matrix.m[0][0] * x) + (matrix.m[1][0] * y) + (matrix.m[2][0] * z) + (matrix.m[3][0] * w);
	m[3][1] = (matrix.m[0][1] * x) + (matrix.m[1][1] * y) + (matrix.m[2][1] * z) + (matrix.m[3][1] * w);
	m[3][2] = (matrix.m[0][2] * x) + (matrix.m[1][2] * y) + (matrix.m[2][2] * z) + (matrix.m[3][2] * w);
	m[3][3] = (matrix.m[0][3] * x) + (matrix.m[1][3] * y) + (matrix.m[2][3] * z) + (matrix.m[3][3] * w);

}

inline revMatrix44 revMatrix44::Mul(const revMatrix44& lhs, const revMatrix44& rhs)
{
	revMatrix44 result;
	f32 x = lhs.m[0][0];
	f32 y = lhs.m[0][1];
	f32 z = lhs.m[0][2];
	f32 w = lhs.m[0][3];
	result.m[0][0] = (rhs.m[0][0] * x) + (rhs.m[1][0] * y) + (rhs.m[2][0] * z) + (rhs.m[3][0] * w);
	result.m[0][1] = (rhs.m[0][1] * x) + (rhs.m[1][1] * y) + (rhs.m[2][1] * z) + (rhs.m[3][1] * w);
	result.m[0][2] = (rhs.m[0][2] * x) + (rhs.m[1][2] * y) + (rhs.m[2][2] * z) + (rhs.m[3][2] * w);
	result.m[0][3] = (rhs.m[0][3] * x) + (rhs.m[1][3] * y) + (rhs.m[2][3] * z) + (rhs.m[3][3] * w);

	x = lhs.m[1][0];
	y = lhs.m[1][1];
	z = lhs.m[1][2];
	w = lhs.m[1][3];
	result.m[1][0] = (rhs.m[0][0] * x) + (rhs.m[1][0] * y) + (rhs.m[2][0] * z) + (rhs.m[3][0] * w);
	result.m[1][1] = (rhs.m[0][1] * x) + (rhs.m[1][1] * y) + (rhs.m[2][1] * z) + (rhs.m[3][1] * w);
	result.m[1][2] = (rhs.m[0][2] * x) + (rhs.m[1][2] * y) + (rhs.m[2][2] * z) + (rhs.m[3][2] * w);
	result.m[1][3] = (rhs.m[0][3] * x) + (rhs.m[1][3] * y) + (rhs.m[2][3] * z) + (rhs.m[3][3] * w);

	x = lhs.m[2][0];
	y = lhs.m[2][1];
	z = lhs.m[2][2];
	w = lhs.m[2][3];
	result.m[2][0] = (rhs.m[0][0] * x) + (rhs.m[1][0] * y) + (rhs.m[2][0] * z) + (rhs.m[3][0] * w);
	result.m[2][1] = (rhs.m[0][1] * x) + (rhs.m[1][1] * y) + (rhs.m[2][1] * z) + (rhs.m[3][1] * w);
	result.m[2][2] = (rhs.m[0][2] * x) + (rhs.m[1][2] * y) + (rhs.m[2][2] * z) + (rhs.m[3][2] * w);
	result.m[2][3] = (rhs.m[0][3] * x) + (rhs.m[1][3] * y) + (rhs.m[2][3] * z) + (rhs.m[3][3] * w);

	x = lhs.m[3][0];
	y = lhs.m[3][1];
	z = lhs.m[3][2];
	w = lhs.m[3][3];
	result.m[3][0] = (rhs.m[0][0] * x) + (rhs.m[1][0] * y) + (rhs.m[2][0] * z) + (rhs.m[3][0] * w);
	result.m[3][1] = (rhs.m[0][1] * x) + (rhs.m[1][1] * y) + (rhs.m[2][1] * z) + (rhs.m[3][1] * w);
	result.m[3][2] = (rhs.m[0][2] * x) + (rhs.m[1][2] * y) + (rhs.m[2][2] * z) + (rhs.m[3][2] * w);
	result.m[3][3] = (rhs.m[0][3] * x) + (rhs.m[1][3] * y) + (rhs.m[2][3] * z) + (rhs.m[3][3] * w);



	return result;
}

inline void revMatrix44::MulScalar( float s )
{
	for( int i=0; i<16; ++i ){
		data[i] *= s;
	}
}

inline void revMatrix44::DivScalar( float s )
{
	for( int i=0; i<16; ++i ){
		data[i] /= s;
	}
}

inline void revMatrix44::CreateIdentity()
{
	for( int i=0; i<4; i++ ){
		for( int j=0; j<4; j++ ){
			if( i == j ){
				m[i][j] = 1;
			}
			else{
				m[i][j] = 0;
			}
		}
	}
}


inline void revMatrix44::Scaling( const revVector3& vec )
{
	CreateIdentity();

	m[0][0] = vec.x;
	m[1][1] = vec.y;
	m[2][2] = vec.z;
}

inline void revMatrix44::Scaling( float x, float y, float z )
{
	CreateIdentity();

	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
}

inline void revMatrix44::RotationX( float radian )
{
	float val_sin, val_cos;

	val_sin = sinf( radian );
	val_cos = cosf( radian );

	CreateIdentity();

	m[1][1] =  val_cos;
	m[1][2] =  val_sin;
	m[2][1] = -val_sin;
	m[2][2] =  val_cos;
}

inline void revMatrix44::RotationY( float radian )
{
	float val_sin, val_cos;

	val_sin = sinf( radian );
	val_cos = cosf( radian );

	CreateIdentity();

	m[0][0] =  val_cos;
	m[0][2] = -val_sin;
	m[2][0] =  val_sin;
	m[2][2] =  val_cos;
}

inline void revMatrix44::RotationZ( float radian )
{
	float val_sin, val_cos;

	val_sin = sinf( radian );
	val_cos = cosf( radian );

	CreateIdentity();

	m[0][0] =  val_cos;
	m[0][1] =  val_sin;
	m[1][0] = -val_sin;
	m[1][1] =  val_cos;
}

inline void revMatrix44::RotationXYZ( const revVector3& vec )
{
	revMatrix44 ry,rz;

	RotationX( vec.x );
	ry.RotationY( vec.y );
	rz.RotationZ( vec.z );

	Mul( ry );
	Mul( rz );
}

inline void revMatrix44::RotationXYZ( float x, float y, float z )
{
	revMatrix44 ry,rz;

	RotationX( x );
	ry.RotationY( y );
	rz.RotationZ( z );

	Mul( ry );
	Mul( rz );
}

inline void revMatrix44::RotationZYX( const revVector3& vec )
{
	revMatrix44 ry,rx;

	RotationZ( vec.z );
	ry.RotationY( vec.y );
	rx.RotationX( vec.x );

	Mul( ry );
	Mul( rx );
}

inline void revMatrix44::RotationZYX( float x, float y, float z )
{
	revMatrix44 ry,rx;

	RotationZ( z );
	ry.RotationY( y );
	rx.RotationX( x );

	Mul( ry );
	Mul( rx );
}


inline void revMatrix44::Translation( const revVector3& vec )
{
	CreateIdentity();

	m[3][0] = vec.x;
	m[3][1] = vec.y;
	m[3][2] = vec.z;
}

inline void revMatrix44::Translation( float x, float y, float z )
{
	CreateIdentity();

	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
}

inline void revMatrix44::Transpose()
{
	revMatrix44 mat = *this;

	for( int i=0; i<4; ++i ){
		for( int j=0;j<4; ++j ){
			m[i][j] = mat.m[j][i];
		}
	}
}

inline revVector4 revMatrix44::Vector3Transform(const revVector3& vec) const
{
	return revVector4(
		m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0],
		m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1],
		m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2],
		1.0f
		);
}


inline revVector4 revMatrix44::Vector3Transform(const revVector3& vec, const revMatrix44& matrix)
{
	return revVector4(
		matrix.m[0][0] * vec.x + matrix.m[1][0] * vec.y + matrix.m[2][0] * vec.z + matrix.m[3][0],
		matrix.m[0][1] * vec.x + matrix.m[1][1] * vec.y + matrix.m[2][1] * vec.z + matrix.m[3][1],
		matrix.m[0][2] * vec.x + matrix.m[1][2] * vec.y + matrix.m[2][2] * vec.z + matrix.m[3][2],
		1.0f
		);
}


inline revVector3 revMatrix44::Vector3TransformCoord(const revVector3& vec) const
{
	return revVector3(
		m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0],
		m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1],
		m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2]
		);
}


inline revVector3 revMatrix44::Vector3TransformCoord(const revVector3& vec, const revMatrix44& matrix)
{
	return revVector3(
		matrix.m[0][0] * vec.x + matrix.m[1][0] * vec.y + matrix.m[2][0] * vec.z + matrix.m[3][0],
		matrix.m[0][1] * vec.x + matrix.m[1][1] * vec.y + matrix.m[2][1] * vec.z + matrix.m[3][1],
		matrix.m[0][2] * vec.x + matrix.m[1][2] * vec.y + matrix.m[2][2] * vec.z + matrix.m[3][2]
		);
}


inline revVector3 revMatrix44::Vector3TransformNormal(const revVector3& vec) const
{
	return revVector3(
		m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z,
		m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z,
		m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z
		);
}


inline revVector3 revMatrix44::Vector3TransformNormal(const revVector3& vec, const revMatrix44& matrix)
{
	return revVector3(
		matrix.m[0][0] * vec.x + matrix.m[1][0] * vec.y + matrix.m[2][0] * vec.z,
		matrix.m[0][1] * vec.x + matrix.m[1][1] * vec.y + matrix.m[2][1] * vec.z,
		matrix.m[0][2] * vec.x + matrix.m[1][2] * vec.y + matrix.m[2][2] * vec.z
		);
}


inline revVector4 revMatrix44::Vector4Transform(const revVector4& vec) const
{
	return revVector4(
		m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0] * vec.w,
		m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1] * vec.w,
		m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2] * vec.w,
		m[0][3] * vec.x + m[1][3] * vec.y + m[2][3] * vec.z + m[3][3] * vec.w
		);
}


inline revVector4 revMatrix44::Vector4Transform(const revVector4& vec, const revMatrix44& matrix)
{
	return revVector4(
		matrix.m[0][0] * vec.x + matrix.m[1][0] * vec.y + matrix.m[2][0] * vec.z + matrix.m[3][0] * vec.w,
		matrix.m[0][1] * vec.x + matrix.m[1][1] * vec.y + matrix.m[2][1] * vec.z + matrix.m[3][1] * vec.w,
		matrix.m[0][2] * vec.x + matrix.m[1][2] * vec.y + matrix.m[2][2] * vec.z + matrix.m[3][2] * vec.w,
		matrix.m[0][3] * vec.x + matrix.m[1][3] * vec.y + matrix.m[2][3] * vec.z + matrix.m[3][3] * vec.w
		);
}

inline void revMatrix44::Set(float* num)
{
	for(int32 i = 0; i < 16; ++i){
		data[i] = num[i];
	}
}

inline revVector3 operator * (const revMatrix44& mat, const revVector3& vec)
{
	return revMatrix44::Vector3TransformCoord(vec, mat);
}

inline revMatrix44 operator * (const revMatrix44& lhs, const revMatrix44& rhs)
{
	return revMatrix44::Mul(lhs, rhs);
}

inline revVector4 operator * (const revMatrix44& mat, const revVector4& vec)
{
	return mat.Vector4Transform(vec);
}


#endif
