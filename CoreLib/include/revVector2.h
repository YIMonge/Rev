#ifndef __REVVECTOR2_H__
#define __REVVECTOR2_H__

#include "revMath.h"

class revVector2
{
public:
	union
	{
		struct
		{
			float x;
			float y;
		};
		float data[2];
	};

public:
	revVector2();
	revVector2(const revVector2& src);
	revVector2(float a, float b);	

			void	Add( const revVector2 &v );
			void	Sub( const revVector2 &v  );
			void	MultiScalar( float s );
			void	DivScalar( float s );

			float	dot( const revVector2 &v );

			float	getNorm();
			float	getSqrNorm();

			void	normalize();
};

#include "revVector2.inl"
#endif