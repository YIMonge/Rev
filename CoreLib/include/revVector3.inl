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


inline void revVector3::Add( const revVector3 &v )
{
	x += v.x;
	y += v.y;
	z += v.z;
}

inline revVector3 revVector3::Add( const revVector3 &lhs, const revVector3 &rhs )
{
	return revVector3( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline void revVector3::Sub( const revVector3 &v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}


inline revVector3 revVector3::Sub( const revVector3 &lhs, const revVector3 &rhs )
{
	return revVector3( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}



inline void revVector3::MultiScalar(float s)
{
	x *= s;
	y *= s;
	z *= s;
}


inline void revVector3::DivScalar(float s)
{
	x /= s;
	y /= s;
	z /= s;
}


inline float revVector3::dot( const revVector3 &v ) const
{
	return x * v.x + y * v.y + z * v.z;
}


inline revVector3 revVector3::cross( const revVector3 &v )  const
{
	return revVector3( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
}

inline float revVector3::getNorm()  const
{
	return sqrtf( x * x + y * y + z * z );
}

inline float revVector3::getSqrNorm()  const
{
	return x * x + y * y + z * z;
}

inline void revVector3::normalize()
{
	float N = getNorm();

	x /= N;
	y /= N;
	z /= N;
}

inline revVector3 revVector3::operator + (const revVector3& lhs)
{
	return revVector3(x + lhs.x, y + lhs.y, z + lhs.z);
}

inline revVector3& revVector3::operator += (const revVector3& lhs)
{
	Add(lhs);
	return *this;
}

inline revVector3 revVector3::operator - (const revVector3& lhs)
{
	return revVector3(x - lhs.x, y - lhs.y, z - lhs.z);
}

inline revVector3& revVector3::operator -= (const revVector3& lhs)
{
	Sub(lhs);
	return *this;
}

inline float& revVector3::operator [] (int id)
{
	return data[id];
}

inline revVector3 revVector3::operator * (float s)
{
	return revVector3(x * s, y * s, z * s);
}

inline revVector3& revVector3::operator *= (float s)
{
	MultiScalar(s);
	return *this;
}

inline revVector3 revVector3::operator / (float s)
{
	return revVector3(x / s, y / s, z / s);
}

inline revVector3& revVector3::operator /= (float s)
{
	DivScalar(s);
	return *this;
}

inline revVector3 operator +  (const revVector3& lhs, const revVector3& rhs)
{
	return revVector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline revVector3 operator -  (const revVector3& lhs, const revVector3& rhs)
{
	return revVector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

inline revVector3 operator *  (float s, const revVector3& lhs)
{
	return revVector3(lhs.x * s, lhs.y * s, lhs.z * s);
}

inline revVector3 operator *  (const revVector3& lhs, float s)
{
	return revVector3(lhs.x * s, lhs.y * s, lhs.z * s);
}

inline revVector3 operator /  (const revVector3& lhs, float s)
{
	return revVector3(lhs.x / s, lhs.y / s, lhs.z / s);
}

#endif