#ifndef __REVMATHUTIL_H__
#define __REVMATHUTIL_H__

#include <math.h>
#include <float.h>

#include "revVector3.h"
#include "revVector4.h"
#include "revMatrix44.h"

static const f32 PI = 3.1415926535f;
static const f32 EPSILON = ((f32)1e-10);

typedef struct{
	int x, y;
}revIndex2;

typedef struct{
	int x, y, z;
}revIndex3;

typedef struct{
	union{
		struct{
			int x, y, z, w;
		};
		int data[4];
	};
}revIndex4;


namespace MathUtil
{

	inline f32 Dot( const revVector3& lhs, const revVector3& rhs)
	{
		return lhs.x * rhs.x + lhs.y + rhs.y + lhs.z * rhs.z;
	}


	inline f32 ToRadian(f32 degree)
	{
		return degree * PI / 180.0f;
	}

	inline revVector3 ToRadian(const revVector3& degree)
	{
		return revVector3(degree.x * PI / 180.0f, degree.y * PI / 180.0f, degree.z * PI / 180.0f);
	}

	inline f32 ToDegree(f32 radian)
	{
		return radian / PI * 180.0f;
	}

	inline bool Comparef32s(f32 a, f32 b)
	{
		if(a >= b + EPSILON || a <= b - EPSILON){
			return false;
		}
		return true;
	}

    template<class T>
	inline T Clamp(T n, T fmin, T fmax)
	{
		if (n < fmin){
			return fmin;
		}
		if (n > fmax){
			return fmax;
		}
		return n;
	}

	inline f32 DistanceSqr(const revVector3& lhs, const revVector3& rhs)
	{
		f32 x = lhs.x - rhs.x;
		f32 y = lhs.y - rhs.y;
		f32 z = lhs.z - rhs.z;
		return x * x + y * y + z * z;
	}

	inline f32 DistanceSqrFromOrigin(const revVector3& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}


	inline f32 Gausian2D(f32 value, f32 sigma)
	{
		return expf(-(value * value) / (2.0f * sigma * sigma));
	}

	inline f32 Gausian2D(f32 value, f32 sigma, f32 C)
	{
		return C * expf(-(value * value) / (2.0f * sigma * sigma));
	}

	inline f32 Gausian3D(f32 distance, f32 dir_radian, f32 sigma)
	{
		f32 xValue = distance * cosf(dir_radian);
		f32 yValue = distance * sinf(dir_radian);
		f32 dispersion = 2 *sigma * sigma;
		return expf(-(xValue * xValue) / dispersion) * expf(-(yValue * yValue) / dispersion);
	}

	inline f32 Gausian3D(f32 distance, f32 dir_radian, f32 sigma, f32 C)
	{
		f32 xValue = distance * cosf(dir_radian);
		f32 yValue = distance * sinf(dir_radian);
		f32 dispersion = 2 *sigma * sigma;
		return C * expf(-(xValue * xValue) / dispersion) * expf(-(yValue * yValue) / dispersion);
	}

	inline f32 Gausian3D(const revVector3& Center, const revVector3& Point, f32 sigma)
	{
		revVector3 Length = Center - Point;
		f32 dispersion = 2 *sigma * sigma;
		return expf(-(Length.x * Length.x) / dispersion) * expf(-(Length.z * Length.z) / dispersion);
	}

	inline f32 Gausian3D(const revVector3& Center, const revVector3& Point, f32 sigma, f32 C)
	{
		revVector3 Length = Center - Point;
		f32 dispersion = 2 *sigma * sigma;
		return C * expf(-(Length.x * Length.x) / dispersion) * expf(-(Length.z * Length.z) / dispersion);
	}

	template<class TYPE>
	inline TYPE GetBitCount(TYPE value)
	{
		TYPE count = 0;
		for(TYPE i = 0; i < sizeof(TYPE) * 8; ++i){
			if(value & (0x01 << i)){
				++count;
			}
		}
		return count;
	}

	inline bool CmpFloat(f32 a, f32 b)
	{
		return fabs(a - b) < FLT_EPSILON;
	}
};



#endif