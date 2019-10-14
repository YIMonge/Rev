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
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
		};
		float m[3][3];
		float data[9];
	};

public:
	revMatrix33();
	revMatrix33(
		float m11, float m12, float m13,
		float m21, float m22, float m23,
		float m31, float m32, float m33);

public:
			void			Add( const revMatrix33 &m );
			void			Sub( const revMatrix33 &m );
			void			Multi( const revMatrix33 &m );
	static	revMatrix33		Multi(const revMatrix33& lhs, const revMatrix33& rhs);

			void			MultiScalar( float s );
			void			DivScalar( float s );

			void			Identity();
			revMatrix33		Inverse();


			void			Scaling( const revVector2& vec );
			void			Scaling( float x, float y);
			void			Rotation( float radian );
			void			Transpose();

			revVector2		Vector2Transform(const revVector2& vec);
	static	revVector2		Vector2Transform(const revVector2& vec, const revMatrix33& matrix);
};


#endif
