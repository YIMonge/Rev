#ifndef __REVVECTOR3_H__
#define __REVVECTOR3_H__

#include "revVector4.h"

class revVector3
{
public:
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

			void			Add(const revVector3 &v);
	static	revVector3		Add(const revVector3 &lhs, const revVector3 &rhs);
			void			Sub(const revVector3 &v);
	static	revVector3		Sub(const revVector3 &lhs, const revVector3 &rhs);
			void			MultiScalar(float s);
			void			DivScalar(float s);

			float			dot(const revVector3 &v) const;
			revVector3		cross(const revVector3 &v) const;

			float			getNorm(void) const;
			float			getSqrNorm(void) const;

			void			normalize(void);

			revVector3		operator +  (const revVector3& lhs);
			revVector3&		operator += (const revVector3& lhs);
			revVector3		operator -  (const revVector3& lhs);
			revVector3&		operator -= (const revVector3& lhs);
			revVector3		operator *  (float s);
			revVector3&		operator *= (float s);
			revVector3		operator /  (float s);
			revVector3&		operator /= (float s);
			float&			operator [] (int id);

	friend	revVector3		operator +  (const revVector3& lhs, const revVector3& rhs);
	friend	revVector3		operator -  (const revVector3& lhs, const revVector3& rhs);
	friend	revVector3		operator *  (float s, const revVector3& lhs);
	friend	revVector3		operator *  (const revVector3& lhs, float s);
	friend	revVector3		operator /  (float s, const revVector3& lhs);
};

#include "revVector3.inl"
#endif