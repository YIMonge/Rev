#ifndef __REVVECTOR3_H__
#define __REVVECTOR3_H__

#include "revTypedef.h"
#include "revVector4.h"

class revVector3
{
public:
	static const revVector3 UP;
	static const revVector3 RIGHT;
	static const revVector3 FOWARD;
	static const revVector3 ZERO;


	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float data[3];
	};

public:
	revVector3();
	revVector3(float a, float b, float c);
	revVector3(const revVector3& lhs);
	revVector3(const revVector4& a);

			revVector3		operator +  (const revVector3& rhs) const;
			revVector3&		operator += (const revVector3& rhs);
			revVector3		operator -  (const revVector3& rhs) const;
			revVector3&		operator -= (const revVector3& rhs);
			revVector3		operator *  (f32 s) const;
			revVector3&		operator *= (f32 s);
			revVector3		operator /  (f32 s) const;
			revVector3&		operator /= (f32 s);
			f32&			operator [] (int id);

	friend	revVector3		operator *  (f32 s, const revVector3& lhs);
	friend	revVector3		operator /  (f32 s, const revVector3& lhs);
	
	void Normalize()
	{
		*this /= Magnitude(*this);
	}

	static f32 Dot(const revVector3& lhs, const revVector3& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	static revVector3 Cross(const revVector3& lhs, const revVector3& rhs)
	{
		return revVector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
	}

	static revVector3 Normalize(const revVector3& v)
	{
		return v / Magnitude(v);
	}

	static f32 Magnitude(const revVector3& v)
	{
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	static f32 SqrMagnitude(const revVector3& v)
	{
		return Dot(v, v);
	}

	static revVector3 Zero()
	{
		return revVector3(0.0f, 0.0f, 0.0f);
	}

};

#include "revVector3.inl"
#endif