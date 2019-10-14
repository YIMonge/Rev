#ifndef __REVVECTOR4_INL__
#define __REVVECTOR4_INL__
#include <math.h>

inline revVector4::revVector4() :
x(0),
y(0),
z(0),
w(0)
{
}


inline revVector4::revVector4( float a, float b, float c, float d) : 
x(a),
y(b),
z(c),
w(d)
{
}

inline revVector4::revVector4( const revVector4& a ) :
x(a.x),
y(a.y),
z(a.z),
w(a.w)
{
}

inline void revVector4::Add( const revVector4 &v )
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}


inline void revVector4::Sub( const revVector4 &v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

inline void revVector4::MultiScalar(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

inline void revVector4::DivScalar(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
}



inline float revVector4::getNorm()
{
	return sqrtf( x * x + y * y + z * z );
}


inline float revVector4::getSqrNorm()
{
	return x * x + y * y + z * z;
}


inline void revVector4::normalize()
{
	float N = getSqrNorm();

	x /= N;
	y /= N;
	z /= N;
}

inline revVector4 revVector4::operator + (const revVector4& a)
{
	return revVector4(x + a.x, y + a.y, z + a.z, w + a.w);
}

inline revVector4& revVector4::operator += (const revVector4& a)
{
	Add(a);
	return *this;
}

inline revVector4 revVector4::operator - (const revVector4& a)
{
	return revVector4(x - a.x, y - a.y, z - a.z, w - a.w);
}

inline revVector4& revVector4::operator -= (const revVector4& a)
{
	Sub(a);
	return *this;
}

inline float& revVector4::operator [] (int id)
{
	return data[id];
}

inline revVector4	operator -  (const revVector4& a, const revVector4& b)
{
	return revVector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline revVector4 revVector4::operator * (float s)
{
	return revVector4(x * s, y * s, z * s, w * s);
}

inline revVector4& revVector4::operator *= (float s)
{
	MultiScalar(s);
	return *this;
}

inline revVector4 revVector4::operator / (float s)
{
	return revVector4(x / s, y / s, z / s, w / s);
}

inline revVector4& revVector4::operator /= (float s)
{
	DivScalar(s);
	return *this;
}

#endif