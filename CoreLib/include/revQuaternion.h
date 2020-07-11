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
	revQuaternion(float radian, const revVector3& axis);
	revQuaternion(const revVector3& radians);
	revQuaternion(float w, float x, float y, float z);
	revQuaternion(const revMatrix44 matrix);

	void Identity();
	void Mul(const revQuaternion& quat);
	void MulScalar(float s);
	void DivScalar(float s);

	void CreateRotation(float radian, float xval, float yval, float zval);
	void CreateRotation(float radian, const revVector3& axis);


	revMatrix44 CreateRotationMatrix();

	float GetSqrNorm(void);
	float GetNorm(void);

	revVector3	Vector3TransformCoord(const revVector3& vec) const;
	static	revVector3	Vector3TransformCoord(const revVector3& vec, const revQuaternion& quat);

	revQuaternion operator + (const revQuaternion& q);
	revQuaternion operator - (const revQuaternion& q);

	revQuaternion operator * (const revQuaternion& q) const 
	{
		revQuaternion ret;
		ret = *this;
		ret.Mul(q);
		return ret;
	}
	revQuaternion operator * (float fs);
	revQuaternion operator / (float fs);


};

#include "revQuaternion.inl"

#endif