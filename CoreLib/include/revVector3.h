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
			f32 x;
			f32 y;
			f32 z;
		};
		f32 data[3];
	};

public:
	revVector3();
	revVector3(f32 a, f32 b, f32 c);
	revVector3(const revVector3& lhs);
	revVector3(const revVector4& a);

			void			Add(const revVector3 &v);
	static	revVector3		Add(const revVector3 &lhs, const revVector3 &rhs);
			void			Sub(const revVector3 &v);
	static	revVector3		Sub(const revVector3 &lhs, const revVector3 &rhs);
			void			MultiScalar(f32 s);
			void			DivScalar(f32 s);

			f32			dot(const revVector3 &v) const;
			revVector3		cross(const revVector3 &v) const;

			f32			getNorm(void) const;
			f32			getSqrNorm(void) const;

			void			normalize(void);

			revVector3		operator +  (const revVector3& lhs);
			revVector3&		operator += (const revVector3& lhs);
			revVector3		operator -  (const revVector3& lhs);
			revVector3&		operator -= (const revVector3& lhs);
			revVector3		operator *  (f32 s);
			revVector3&		operator *= (f32 s);
			revVector3		operator /  (f32 s);
			revVector3&		operator /= (f32 s);
			f32&			operator [] (int id);

	friend	revVector3		operator +  (const revVector3& lhs, const revVector3& rhs);
	friend	revVector3		operator -  (const revVector3& lhs, const revVector3& rhs);
	friend	revVector3		operator *  (f32 s, const revVector3& lhs);
	friend	revVector3		operator *  (const revVector3& lhs, f32 s);
	friend	revVector3		operator /  (f32 s, const revVector3& lhs);
};

#include "revVector3.inl"
#endif