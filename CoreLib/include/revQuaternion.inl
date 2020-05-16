#ifndef __REVQUATERNION_INL__
#define __REVQUATERNION_INL__

inline revQuaternion::revQuaternion( ) :
x(0.0f),
y(0.0f),
z(0.0f),
w(0.0f)
{
}

inline revQuaternion::revQuaternion( f32 wval, f32 xval, f32 yval, f32 zval ) :
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

inline void revQuaternion::Add( const revQuaternion& quat )
{
	x += quat.x;
	y += quat.y;
	z += quat.z;
	w += quat.w;
}

inline void revQuaternion::Sub( const revQuaternion& quat )
{
	x -= quat.x;
	y -= quat.y;
	z -= quat.z;
	w -= quat.w;
}

inline void revQuaternion::mult( const revQuaternion& quat )
{
	revQuaternion leftrevQuat = *this;

	x = leftrevQuat.w * quat.x + leftrevQuat.x * quat.w - leftrevQuat.y * quat.z + leftrevQuat.z * quat.y;
	y = leftrevQuat.w * quat.y + leftrevQuat.x * quat.z + leftrevQuat.y * quat.w - leftrevQuat.z * quat.x;
	z = leftrevQuat.w * quat.z - leftrevQuat.x * quat.y + leftrevQuat.y * quat.x + leftrevQuat.z * quat.w;
	w = leftrevQuat.w * quat.w - leftrevQuat.x * quat.x - leftrevQuat.y * quat.y - leftrevQuat.z * quat.z;
}

inline void revQuaternion::MultiScalar( f32 s )
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

inline void revQuaternion::DivScalar( f32 s )
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
}

inline void revQuaternion::CreateRotation(f32 Radius, f32 xval, f32 yval, f32 zval)
{
	f32 rad = Radius / 2.0f;
	f32 sinval = sinf(rad);
	w = cosf(rad);
	x = xval * sinval;
	y = yval * sinval;
	z = zval * sinval;
}

inline void revQuaternion::CreateRotation(f32 Radius, revVector3& axis)
{
	f32 rad = Radius / 2.0f;
	f32 sinval = sinf(rad);
	w = cosf(rad);
	x = axis.x * sinval;
	y = axis.y * sinval;
	z = axis.z * sinval;
}

inline f32 revQuaternion::GetSqrNorm()
{
	return w * w + x * x + y * y + z * z;
}


inline f32 revQuaternion::GetNorm()
{
	return sqrtf( GetSqrNorm() );
}

inline revVector3 revQuaternion::Vector3TransformCoord(const revVector3& vec)
{
	revQuaternion qur(w, -x, -y, -z), qpos(vec);
	qur.mult(qpos);
	qur.mult(*this);

	return revVector3(qur.x, qur.y, qur.z);
}

#endif