#ifndef __REVVECTOR2_H__
#define __REVVECTOR2_H__

#include "revTypedef.h"
#include "revMath.h"

class revVector2
{
public:
	union
	{
		struct
		{
			f32 x;
			f32 y;
		};
		f32 data[2];
	};

public:
	revVector2();
	revVector2(const revVector2& src);
	revVector2(revVector2&& src) noexcept;
	revVector2(f32 a, f32 b);	

			void	Add( const revVector2 &v );
			void	Sub( const revVector2 &v  );
			void	MultiScalar( f32 s );
			void	DivScalar( f32 s );

			f32	dot( const revVector2 &v );

			f32	getNorm();
			f32	getSqrNorm();

			void	normalize();
};

#include "revVector2.inl"
#endif