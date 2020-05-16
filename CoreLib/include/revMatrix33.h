#ifndef __revMatrix33_H__
#define __revMatrix33_H__

#include "revMath.h"

class revMatrix33
{
public:
	union
	{
		struct
		{
			f32 _11, _12, _13;
			f32 _21, _22, _23;
			f32 _31, _32, _33;
		};
		f32 m[3][3];
		f32 data[9];
	};

public:
	revMatrix33();
	revMatrix33(
		f32 m11, f32 m12, f32 m13,
		f32 m21, f32 m22, f32 m23,
		f32 m31, f32 m32, f32 m33);

public:
			void			Add( const revMatrix33 &m );
			void			Sub( const revMatrix33 &m );
			void			Multi( const revMatrix33 &m );
	static	revMatrix33		Multi(const revMatrix33& lhs, const revMatrix33& rhs);

			void			MultiScalar( f32 s );
			void			DivScalar( f32 s );

			void			Identity();
			revMatrix33		Inverse();


			void			Scaling( const revVector2& vec );
			void			Scaling( f32 x, f32 y);
			void			Rotation( f32 radian );
			void			Transpose();

			revVector2		Vector2Transform(const revVector2& vec);
	static	revVector2		Vector2Transform(const revVector2& vec, const revMatrix33& matrix);
};


#endif
