#ifndef __REVVECTOR3_INL__
#define __REVVECTOR3_INL__
#include <math.h>

inline revVector3::revVector3() :
x(0), 
y(0),
z(0)
{
}


inline revVector3::revVector3( float a, float b, float c) :
x(a),
y(b),
z(c) 
{
}

inline revVector3::revVector3( const revVector3& lhs ) : 
x(lhs.x), 
y(lhs.y), 
z(lhs.z) 
{
}

inline revVector3::revVector3( const revVector4& a ) : 
x(a.x), 
y(a.y), 
z(a.z) 
{
}


inline revVector3 revVector3::operator + (const revVector3& lhs) const 
{
	return revVector3(x + lhs.x, y + lhs.y, z + lhs.z);
}

inline revVector3& revVector3::operator += (const revVector3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

inline revVector3 revVector3::operator - (const revVector3& rhs) const
{
	return revVector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

inline revVector3& revVector3::operator -= (const revVector3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

inline f32& revVector3::operator [] (int id)
{
	return data[id];
}

inline revVector3 revVector3::operator * (f32 s) const
{
	return revVector3(x * s, y * s, z * s);
}

inline revVector3& revVector3::operator *= (float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

inline revVector3 revVector3::operator / (f32 s) const 
{
	return revVector3(x / s, y / s, z / s);
}

inline revVector3& revVector3::operator /= (float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

inline revVector3 operator *  (f32 s, const revVector3& lhs)
{
	return revVector3(lhs.x * s, lhs.y * s, lhs.z * s);
}

inline revVector3 operator / (f32 s, const revVector3& lhs)
{
	return revVector3(lhs.x / s, lhs.y / s, lhs.z / s);
}

#endif