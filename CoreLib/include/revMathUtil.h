#ifndef __REVMATHUTIL_H__
#define __REVMATHUTIL_H__

#include <math.h>
#include <float.h>

#include "revVector3.h"
#include "revVector4.h"
#include "revMatrix44.h"

static const float PI = 3.1415926535f;
static const float EPSILON = ((float)1e-10);

typedef struct {
	uint32 x, y;
}revIndex2;

typedef struct {
	union {
		struct {
			uint32 x, y, z;
		};
		uint32 data[3];
	};
}revIndex3;

typedef struct {
	union {
		struct {
			uint32 x, y, z, w;
		};
		uint32 data[4];
	};
}revIndex4;


namespace MathUtil
{

	inline float Dot(const revVector3& lhs, const revVector3& rhs)
	{
		return lhs.x * rhs.x + lhs.y + rhs.y + lhs.z * rhs.z;
	}


	inline float ToRadian(float degree)
	{
		return degree * PI / 180.0f;
	}

	inline revVector3 ToRadian(const revVector3& degree)
	{
		return revVector3(degree.x * PI / 180.0f, degree.y * PI / 180.0f, degree.z * PI / 180.0f);
	}

	inline float ToDegree(float radian)
	{
		return radian / PI * 180.0f;
	}

	inline bool CompareFloats(float a, float b)
	{
		if (a >= b + EPSILON || a <= b - EPSILON) {
			return false;
		}
		return true;
	}

	template<class T>
	inline T Clamp(T n, T fmin, T fmax)
	{
		if (n < fmin) {
			return fmin;
		}
		if (n > fmax) {
			return fmax;
		}
		return n;
	}

	inline float DistanceSqr(const revVector3& lhs, const revVector3& rhs)
	{
		float x = lhs.x - rhs.x;
		float y = lhs.y - rhs.y;
		float z = lhs.z - rhs.z;
		return x * x + y * y + z * z;
	}

	inline float DistanceSqrFromOrigin(const revVector3& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}


	inline float Gausian2D(float value, float sigma)
	{
		return expf(-(value * value) / (2.0f * sigma * sigma));
	}

	inline float Gausian2D(float value, float sigma, float C)
	{
		return C * expf(-(value * value) / (2.0f * sigma * sigma));
	}

	inline float Gausian3D(float distance, float dir_radian, float sigma)
	{
		float xValue = distance * cosf(dir_radian);
		float yValue = distance * sinf(dir_radian);
		float dispersion = 2 * sigma * sigma;
		return expf(-(xValue * xValue) / dispersion) * expf(-(yValue * yValue) / dispersion);
	}

	inline float Gausian3D(float distance, float dir_radian, float sigma, float C)
	{
		float xValue = distance * cosf(dir_radian);
		float yValue = distance * sinf(dir_radian);
		float dispersion = 2 * sigma * sigma;
		return C * expf(-(xValue * xValue) / dispersion) * expf(-(yValue * yValue) / dispersion);
	}

	inline float Gausian3D(const revVector3& Center, const revVector3& Point, float sigma)
	{
		revVector3 Length = Center - Point;
		float dispersion = 2 * sigma * sigma;
		return expf(-(Length.x * Length.x) / dispersion) * expf(-(Length.z * Length.z) / dispersion);
	}

	inline float Gausian3D(const revVector3& Center, const revVector3& Point, float sigma, float C)
	{
		revVector3 Length = Center - Point;
		float dispersion = 2 * sigma * sigma;
		return C * expf(-(Length.x * Length.x) / dispersion) * expf(-(Length.z * Length.z) / dispersion);
	}

	template<class TYPE>
	inline TYPE GetBitCount(TYPE value)
	{
		TYPE count = 0;
		for (TYPE i = 0; i < sizeof(TYPE) * 8; ++i) {
			if (value & (0x01 << i)) {
				++count;
			}
		}
		return count;
	}

	inline bool CmpFloat(float a, float b)
	{
		return fabs(a - b) < FLT_EPSILON;
	}
};



#endif