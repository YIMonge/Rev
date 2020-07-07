#ifndef __REVQUATERNION_INL__
#define __REVQUATERNION_INL__

inline revQuaternion::revQuaternion( ) :
x(0.0f),
y(0.0f),
z(0.0f),
w(0.0f)
{
}

inline revQuaternion::revQuaternion( float wval, float xval, float yval, float zval ) :
x(xval),
y(yval),
z(zval),
w(wval)
{
}

inline revQuaternion::revQuaternion( const revVector3& vec ) : 
x(vec.x),
y(vec.y),
z(vec.z),
w(1.0f)
{
}

inline revQuaternion::revQuaternion( const revVector4& vec ) :
x(vec.x),
y(vec.y),
z(vec.z),
w(vec.w)
{
}

inline void revQuaternion::Identity( )
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

inline void revQuaternion::Mul( const revQuaternion& quat )
{
	revQuaternion leftrevQuat = *this;

	x = leftrevQuat.w * quat.x + leftrevQuat.x * quat.w - leftrevQuat.y * quat.z + leftrevQuat.z * quat.y;
	y = leftrevQuat.w * quat.y + leftrevQuat.x * quat.z + leftrevQuat.y * quat.w - leftrevQuat.z * quat.x;
	z = leftrevQuat.w * quat.z - leftrevQuat.x * quat.y + leftrevQuat.y * quat.x + leftrevQuat.z * quat.w;
	w = leftrevQuat.w * quat.w - leftrevQuat.x * quat.x - leftrevQuat.y * quat.y - leftrevQuat.z * quat.z;
}

inline void revQuaternion::MulScalar( float s )
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

inline void revQuaternion::DivScalar( float s )
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
}

inline void revQuaternion::CreateRotation(float radian, float xval, float yval, float zval)
{
	float rad = radian / 2.0f;
	float sinval = sinf(rad);
	w = cosf(rad);
	x = xval * sinval;
	y = yval * sinval;
	z = zval * sinval;
}

inline void revQuaternion::CreateRotation(float radian, const revVector3& axis)
{
	float rad = radian / 2.0f;
	float sinval = sinf(rad);
	w = cosf(rad);
	x = axis.x * sinval;
	y = axis.y * sinval;
	z = axis.z * sinval;
}

inline float revQuaternion::GetSqrNorm()
{
	return w * w + x * x + y * y + z * z;
}


inline float revQuaternion::GetNorm()
{
	return sqrtf( GetSqrNorm() );
}

inline revVector3 revQuaternion::Vector3TransformCoord(const revVector3& vec)
{
	revQuaternion qur(w, -x, -y, -z), qpos(vec);
	qur.Mul(qpos);
	qur.Mul(*this);

	return revVector3(qur.x, qur.y, qur.z);
}

#endif