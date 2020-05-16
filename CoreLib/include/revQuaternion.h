#ifndef __REVQUATERNION_H__
#define __REVQUATERNION_H__

#include "revVector3.h"
#include "revVector4.h"
#include "revMatrix44.h"

class revQuaternion
{
public:
	static const revQuaternion ZERO;

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
	revQuaternion();
	revQuaternion(f32 wval, f32 xval, f32 yval, f32 zval);
	revQuaternion(const revVector3& revVector3);
	revQuaternion(const revVector4& revVector4);

	void Identity();

	void Add(const revQuaternion& quat);
	void Sub(const revQuaternion& quat);
	void mult(const revQuaternion& quat);
	void MultiScalar(f32 s);
	void DivScalar(f32 s);

	void CreateRotation(f32 Radius, f32 xval, f32 yval, f32 zval);
	void CreateRotation(f32 Radius, revVector3& axis);


	revMatrix44 CreateRotationMatrix();

	f32 GetSqrNorm(void);
	f32 GetNorm(void);

	revVector3	Vector3TransformCoord(const revVector3& vec);
	static	revVector3	Vector3TransformCoord(const revVector3& vec, const revQuaternion& quat);

	revQuaternion operator + (const revQuaternion& q);
	revQuaternion operator - (const revQuaternion& q);

	revQuaternion operator * (const revQuaternion& q);
	revQuaternion operator * (f32 fs);
	revQuaternion operator / (f32 fs);
};

#include "revQuaternion.inl"

#endif