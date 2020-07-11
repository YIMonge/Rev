#ifndef __REVQUATERNION_INL__
#define __REVQUATERNION_INL__

inline revQuaternion::revQuaternion( ) :
x(0.0f),
y(0.0f),
z(0.0f),
w(0.0f)
{
}

inline revQuaternion::revQuaternion(float w, float x, float y, float z) :
x(x),
y(y),
z(z),
w(w)
{
}


inline revQuaternion::revQuaternion(float radian, const revVector3& axis)
{
	CreateRotation(radian, axis);
}

inline revQuaternion::revQuaternion(const revMatrix44 m)
{
	f32 tr = m._11 + m._22 + m._33;
	if (tr > 0) {
		f32 s = sqrtf(tr + 1.0f) * 2.0f;
		w = 0.25f * s;
		x = (m._32 - m._23) / s;
		y = (m._13 - m._31) / s;
		z = (m._21 - m._22) / s;
	}
	else if ((m._11 > m._22) && (m._11 > m._33)) {
		f32 s = sqrtf(m._11 - m._22 - m._33 + 1.0f) * 2.0f;
		w = (m._32 - m._23) / s;
		x = 0.25f * s;
		y = (m._12 + m._21) / s;
		z = (m._13 + m._31) / s;
	}
	else if (m._22 > m._33) {
		f32 s = sqrtf(-m._11 + m._22 - m._33 + 1.0f) * 2.0f;
		w = (m._13 - m._31) / s;
		x = (m._12 + m._21) / s;
		y = 0.25f * s;
		z = (m._23 + m._32) / s;
	}
	else {
		f32 s = sqrtf(-m._11 - m._22 + m._33 + 1.0f) * 2.0f;
		w = (m._21 - m._12) / s;
		x = (m._13 + m._31) / s;
		y = (m._23 + m._32) / s;
		z = 0.25f * s;
	}
}


inline revQuaternion::revQuaternion(const revVector3& radians)
{
	revQuaternion x, y, z;
	x.CreateRotation(radians.x, revVector3::RIGHT);
	y.CreateRotation(radians.y, revVector3::UP);
	z.CreateRotation(radians.z, revVector3::FOWARD);

	*this = x * y * z;
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

inline revVector3 revQuaternion::Vector3TransformCoord(const revVector3& vec) const
{
	revQuaternion qur(w, -x, -y, -z), qpos(0.0f, vec.x, vec.y, vec.z);
	qur.Mul(qpos);
	qur.Mul(*this);

	return revVector3(qur.x, qur.y, qur.z);
}

#endif