#ifndef __REVMATRIX44_INL__
#define __REVMATRIX44_INL__

inline void revMatrix44::Multi( const revMatrix44& matrix )
{
	revMatrix44 mat = *this;
	for( int i=0; i<4; i++ ){
		for( int j=0; j<4; j++ ){
			m[i][j] = mat.m[0][j] * matrix.m[i][0]
				+ mat.m[1][j] * matrix.m[i][1]
				+ mat.m[2][j] * matrix.m[i][2]
				+ mat.m[3][j] * matrix.m[i][3];
		}
	}
}

inline revMatrix44 revMatrix44::Multi(const revMatrix44& lhs, const revMatrix44& rhs)
{
	revMatrix44 mat;
	for( int i=0; i<4; i++ ){
		for( int j=0; j<4; j++ ){
			mat.m[i][j]  = lhs.m[0][j] * rhs.m[i][0]
				+ lhs.m[1][j] * rhs.m[i][1]
				+ lhs.m[2][j] * rhs.m[i][2]
				+ lhs.m[3][j] * rhs.m[i][3];
		}
	}
	return mat;
}

inline void revMatrix44::MultiScalar( float s )
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

inline void revMatrix44::Identity()
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
	Identity();

	m[0][0] = vec.x;
	m[1][1] = vec.y;
	m[2][2] = vec.z;
}

inline void revMatrix44::Scaling( float x, float y, float z )
{
	Identity();

	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
}

inline void revMatrix44::RotationX( float radian )
{
	float val_sin, val_cos;

	val_sin = sinf( radian );
	val_cos = cosf( radian );

	Identity();

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

	Identity();

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

	Identity();

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

	Multi( ry );
	Multi( rz );
}

inline void revMatrix44::RotationXYZ( float x, float y, float z )
{
	revMatrix44 ry,rz;

	RotationX( x );
	ry.RotationY( y );
	rz.RotationZ( z );

	Multi( ry );
	Multi( rz );
}

inline void revMatrix44::RotationZYX( const revVector3& vec )
{
	revMatrix44 ry,rx;

	RotationZ( vec.z );
	ry.RotationY( vec.y );
	rx.RotationX( vec.x );

	Multi( ry );
	Multi( rx );
}

inline void revMatrix44::RotationZYX( float x, float y, float z )
{
	revMatrix44 ry,rx;

	RotationZ( z );
	ry.RotationY( y );
	rx.RotationX( x );

	Multi( ry );
	Multi( rx );
}


inline void revMatrix44::Translation( const revVector3& vec )
{
	Identity();

	m[3][0] = vec.x;
	m[3][1] = vec.y;
	m[3][2] = vec.z;
}

inline void revMatrix44::Translation( float x, float y, float z )
{
	Identity();

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

inline revVector4 revMatrix44::Vector3Transform(const revVector3& vec)
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


inline revVector3 revMatrix44::Vector3TransformCoord(const revVector3& vec)
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


inline revVector3 revMatrix44::Vector3TransformNormal(const revVector3& vec)
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
	for(int i = 0; i < 16; ++i){
		data[i] = num[i];
	}
}

inline revVector3 operator * (const revMatrix44& mat, const revVector3& vec)
{
	return revMatrix44::Vector3TransformCoord(vec, mat);
}

inline revMatrix44 operator * (const revMatrix44& lhs, revMatrix44& rhs)
{
	return revMatrix44::Multi(lhs, rhs);
}

inline revVector4 operator * (const revMatrix44& mat, const revVector4& vec)
{
	return mat.Vector4Transform(vec);
}

#endif
