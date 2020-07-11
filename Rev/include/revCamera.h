#ifndef __REVCAMERA_H__
#define __REVCAMERA_H__

#include "revMath.h"
#include "revTransform.h"
#include "revGraphicsCommandList.h"

class revCamera
{
public:
	revCamera(revTransform* parent = nullptr) :
		fov(MathUtil::ToRadian(45.0f)),
		aspectRatio(1.6f),
		nearZ(0.01f),
		farZ(10000.0f)
	{
		transform.SetParent(parent);
	}
	virtual ~revCamera(){}

	const revVector3& GetUpVector()
	{
		revVector3 up = revVector3::UP;
		return transform.GetLocalMatrix().Vector3TransformNormal(up);
	}

	void LookAt(const revVector3& target)
	{
		transform.LookAt(target);
	}

	revTransform& GetTransform()
	{
		return transform;
	}

	const revTransform& GetTransform() const
	{
		return transform;
	}

	revMatrix44 GetProjectionMatrix() const 
	{
		revMatrix44 proj;
		proj.CreatePerspectiveMatrixLH(fov, aspectRatio, nearZ, farZ);
		return proj;
	}

	revMatrix44 GetViewMatrix() const
	{
		revMatrix44 view;
		view.CreateLookTo(transform.GetPosition(), transform.GetFoward(), transform.GetUp());
		return view;
	}

private:
	revTransform transform;
	f32 fov;
	f32 aspectRatio;
	f32 nearZ;
	f32 farZ;
};

#endif

