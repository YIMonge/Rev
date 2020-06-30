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
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
		float data[16];
	};

public:
	revMatrix44();
	revMatrix44(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44);
	revMatrix44(float afNum[16]);
	revMatrix44(const revMatrix44& m);
	~revMatrix44();


public:
			void		Mul( const revMatrix44& m );
	static	revMatrix44	Mul(const revMatrix44& lhs, const revMatrix44& rhs);

			void		MulScalar( float s );
			void		DivScalar( float s );

			void		Identity();
            revMatrix44	Inverse();

			void		Scaling( const revVector3& vec );
            void		Scaling( float x, float y, float z );
            void		RotationX( float radian );
            void		RotationY( float radian );
            void		RotationZ( float radian );
            void		RotationXYZ( const revVector3& vec );
            void		RotationXYZ( float x, float y, float z );
            void		RotationZYX( const revVector3& vec );
            void		RotationZYX( float x, float y, float z );
            void		Translation( const revVector3& vec );
    
            void		Translation( float x, float y, float z );
			void		CreateLookAtrevMatrixRH(const revVector3& eye, const revVector3& lookat, const revVector3& upvec);
			void		CreatePerspectiverevMatrixRH(float fov_radian, float aspect, float near, float far);
			void		CreateViewPortMatrixRH(int Width, int height, int Near, int Far);

			void		CreateLookAtrevMatrixLH(const revVector3& eye, const revVector3& lookat, const revVector3& upvec);
			void		CreatePerspectiveMatrixLH(float fov_radian, float aspect, float near, float far);

			void		Transpose();

			void		Set(float m11, float m12, float m13, float m14,
							float m21, float m22, float m23, float m24,
							float m31, float m32, float m33, float m34,
							float m41, float m42, float m43, float m44);
			void		Set(revVector4 a, revVector4 b, revVector4 c, revVector4 d);
			void		Set(float* afNum);

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