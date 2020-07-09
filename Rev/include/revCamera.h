#ifndef __REVCAMERA_H__
#define __REVCAMERA_H__

#include "revTransform.h"

class revCamera
{
public:
	revCamera(revTransform* parent = nullptr)
	{
		transform.SetParent(parent);
	}
	virtual ~revCamera(){}

	const revVector3& GetUpVector()
	{
		revVector3 up = revVector3::UP;
		return transform.GetLocalMatrix().Vector3TransformNormal(up);
	}

	const revVector3& GetPosition() const
	{
		return transform.GetPosition();
	}
	void SetPosition(const revVector3& position)
	{
		transform.SetPosition(position);
	}


private:
	revTransform transform;
};

#endif

