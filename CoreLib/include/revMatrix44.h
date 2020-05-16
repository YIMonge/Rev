#ifndef __revMatrix44_H__
#define __revMatrix44_H__

#include "revVector3.h"
#include "revVector4.h"

class revMatrix44
{
public:
	union
	{
		struct
		{
			f32 _11, _12, _13, _14;
			f32 _21, _22, _23, _24;
			f32 _31, _32, _33, _34;
			f32 _41, _42, _43, _44;
		};
		f32 m[4][4];
		f32 data[16];
	};

public:
	revMatrix44();
	revMatrix44(
		f32 m11, f32 m12, f32 m13, f32 m14,
		f32 m21, f32 m22, f32 m23, f32 m24,
		f32 m31, f32 m32, f32 m33, f32 m34,
		f32 m41, f32 m42, f32 m43, f32 m44);
	revMatrix44(f32 afNum[16]);
	revMatrix44(const revMatrix44& m);
	~revMatrix44();


public:
			void		Multi( const revMatrix44& m );
	static	revMatrix44	Multi(const revMatrix44& lhs, const revMatrix44& rhs);

			void		MultiScalar( f32 s );
			void		DivScalar( f32 s );

			void		Identity();
            revMatrix44	Inverse();

			void		Scaling( const revVector3& vec );
            void		Scaling( f32 x, f32 y, f32 z );
            void		RotationX( f32 radian );
            void		RotationY( f32 radian );
            void		RotationZ( f32 radian );
            void		RotationXYZ( const revVector3& vec );
            void		RotationXYZ( f32 x, f32 y, f32 z );
            void		RotationZYX( const revVector3& vec );
            void		RotationZYX( f32 x, f32 y, f32 z );
            void		Translation( const revVector3& vec );
    
            void		Translation( f32 x, f32 y, f32 z );
			void		CreateLookAtrevMatrixRH(const revVector3& eye, const revVector3& lookat, const revVector3& upvec);
			void		CreatePerspectiverevMatrixRH(f32 fov_radian, f32 aspect, f32 near, f32 far);
			void		CreateViewPortrevMatrixRH(int Width, int height, int Near, int Far);

			void		CreateLookAtrevMatrixLH(const revVector3& eye, const revVector3& lookat, const revVector3& upvec);
			void		CreatePerspectiverevMatrixLH(f32 fov_radian, f32 aspect, f32 near, f32 far);

			void		Transpose();

			void		Set(f32 m11, f32 m12, f32 m13, f32 m14,
							f32 m21, f32 m22, f32 m23, f32 m24,
							f32 m31, f32 m32, f32 m33, f32 m34,
							f32 m41, f32 m42, f32 m43, f32 m44);
			void		Set(revVector4 a, revVector4 b, revVector4 c, revVector4 d);
			void		Set(f32* afNum);

			revVector4	Vector3Transform(const revVector3& vec);
	static	revVector4	Vector3Transform(const revVector3& vec, const revMatrix44& matrix);
			revVector3	Vector3TransformCoord(const revVector3& vec);
	static	revVector3	Vector3TransformCoord(const revVector3& vec, const revMatrix44& matrix);
			revVector3	Vector3TransformNormal(const revVector3& vec);
	static	revVector3	Vector3TransformNormal(const revVector3& vec, const revMatrix44& matrix);
			revVector4	Vector4Transform(const revVector4& vec) const;
	static	revVector4	Vector4Transform(const revVector4& vec, const revMatrix44& matrix);


			revVector3	operator * (const revVector3& vec);
			revMatrix44	operator * (revMatrix44& matrix);
			revVector4	operator * (const revVector4& vec);
};

typedef revMatrix44 revMatrix44;

#include "revMatrix44.inl"

#endif