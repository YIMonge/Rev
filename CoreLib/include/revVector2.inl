#ifndef __REVVECTOR2_INL__
#define __REVVECTOR2_INL__
#include <math.h>

inline revVector2::revVector2() 
: x(0)
, y(0)
{
}

inline revVector2::revVector2(const revVector2& src) 
: x(src.x)
, y(src.y)
{	
}

inline revVector2::revVector2(revVector2&& src) noexcept
: x(src.x)
, y(src.y)
{
}


inline revVector2::revVector2( f32 a, f32 b ) 
: x(a)
, y(b) 
{
}

inline void revVector2::Add( const revVector2 &v )
{
	x += v.x;
	y += v.y;
}


inline void revVector2::Sub( const revVector2 &v )
{
	x -= v.x;
	y -= v.y;
}

inline void revVector2::MultiScalar(f32 s)
{
	x *= s;
	y *= s;
}


inline void revVector2::DivScalar(f32 s)
{
	x /= s;
	y /= s;
}


inline f32 revVector2::dot( const revVector2 &v )
{
	return x * v.x + y * v.y;
}


inline f32 revVector2::getNorm()
{
	return sqrtf( x * x + y * y );
}


inline f32 revVector2::getSqrNorm()
{
	return x * x + y * y;
}


inline void revVector2::normalize()
{
	f32 N  =  1.0f /getNorm();
	x *= N;
	y *= N;
}

#endif