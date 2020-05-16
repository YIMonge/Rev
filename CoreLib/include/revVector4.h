#ifndef __REVVECTOR4_H__
#define __REVVECTOR4_H__

#include "revTypedef.h"
#include "revVector3.h"

class revVector4
{
public:
	union
	{
		struct
		{
			f32 x;
			f32 y;
			f32 z;
			f32 w;
		};
		f32 data[4];
	};

public:
	revVector4();
	revVector4( f32 a, f32 b, f32 c, f32 d );
	revVector4( const revVector4& a );

			void			Add( const revVector4 &v );
			void			Sub( const revVector4 &v );
			void			MultiScalar( f32 s );
			void			DivScalar( f32 s );

			f32			getNorm();
			f32			getSqrNorm();

			void			normalize();

			revVector4		operator +  (const revVector4& a);
			revVector4&		operator += (const revVector4& a);
			revVector4		operator -  (const revVector4& a);
			revVector4&		operator -= (const revVector4& a);
			revVector4		operator *  (f32 s);
			revVector4&		operator *= (f32 s);
			revVector4		operator /  (f32 s);
			revVector4&		operator /= (f32 s);
			f32&			operator [] (int id);

	friend	revVector4		operator -  (const revVector4& a, const revVector4& b);
};

#include "revVector4.inl"

#endif