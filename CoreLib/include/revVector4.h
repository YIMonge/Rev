#ifndef __REVVECTOR4_H__
#define __REVVECTOR4_H__

#include "revVector3.h"

class revVector4
{
public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		float data[4];
	};

public:
	revVector4();
	revVector4( float a, float b, float c, float d );
	revVector4( const revVector4& a );

			void			Add( const revVector4 &v );
			void			Sub( const revVector4 &v );
			void			MultiScalar( float s );
			void			DivScalar( float s );

			float			getNorm();
			float			getSqrNorm();

			void			normalize();

			revVector4		operator +  (const revVector4& a);
			revVector4&		operator += (const revVector4& a);
			revVector4		operator -  (const revVector4& a);
			revVector4&		operator -= (const revVector4& a);
			revVector4		operator *  (float s);
			revVector4&		operator *= (float s);
			revVector4		operator /  (float s);
			revVector4&		operator /= (float s);
			float&			operator [] (int id);

	friend	revVector4		operator -  (const revVector4& a, const revVector4& b);
};

#include "revVector4.inl"

#endif