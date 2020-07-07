#ifndef __REVTRANSFORM_H__
#define __REVTRANSFORM_H__

#include "revMath.h"
#include "revArray.h"

class revTransform
{
public:
	revTransform(revTransform* parent = nullptr);
	virtual ~revTransform();

	const revVector3& GetScale() const { return scale; }
	const revVector3& GetPosition() const { return position; }
	const revQuaternion& GetRoation() const { return rotation; }

	void SetScale(const revVector3& scale) 
	{ 
		this->scale = scale; 
		UpdateMatrix();
	}
	void SetPosition(const revVector3& position) 
	{ 
		this->position = position; 
		UpdateMatrix();
	}
	void SetRotation(const revQuaternion& rotation) 
	{ 
		this->rotation = rotation; 
		UpdateMatrix();
	}

	const revMatrix44& GetWorldMatrix() const
	{
		return world;
	}
	const revMatrix44& GetLocalMatrix() const
	{
		return local;
	}

private:
	void UpdateMatrix();

	revTransform* parent;
	revArray<revTransform*> children;
	revVector3 scale;
	revVector3 position;
	revQuaternion rotation;	

	revMatrix44 world;
	revMatrix44 local;
};

#endif
