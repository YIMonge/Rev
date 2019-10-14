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
			float x;
			float y;
			float z;
			float w;
		};
		float data[4];
	};

public:
	revQuaternion();
	revQuaternion(float wval, float xval, float yval, float zval);
	revQuaternion(const revVector3& revVector3);
	revQuaternion(const revVector4& revVector4);

	void Identity();

	void Add(const revQuaternion& quat);
	void Sub(const revQuaternion& quat);
	void mult(const revQuaternion& quat);
	void MultiScalar(float s);
	void DivScalar(float s);

	void CreateRotation(float Radius, float xval, float yval, float zval);
	void CreateRotation(float Radius, revVector3& axis);


	revMatrix44 CreateRotationMatrix();

	float GetSqrNorm(void);
	float GetNorm(void);

	revVector3	Vector3TransformCoord(const revVector3& vec);
	static	revVector3	Vector3TransformCoord(const revVector3& vec, const revQuaternion& quat);

	revQuaternion operator + (const revQuaternion& q);
	revQuaternion operator - (const revQuaternion& q);

	revQuaternion operator * (const revQuaternion& q);
	revQuaternion operator * (float fs);
	revQuaternion operator / (float fs);
};

#include "revQuaternion.inl"

#endif