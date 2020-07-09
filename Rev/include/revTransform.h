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
	const revVector3& GetRoation() const { return rotation; }

	void SetParent(revTransform* parent)
	{
		this->parent = parent;
		dirty = true;
	}
	void SetScale(const revVector3& scale) 
	{ 
		this->scale = scale; 
		dirty = true;
	}
	void SetPosition(const revVector3& position) 
	{ 
		this->position = position; 
		dirty = true;
	}
	void SetRotation(const revVector3& rotation) 
	{ 
		this->rotation = rotation; 
		dirty = true;
	}

	const revMatrix44& GetWorldMatrix()
	{
		if (dirty) UpdateMatrix();
		return world;
	}
	const revMatrix44& GetLocalMatrix()
	{
		if (dirty) UpdateMatrix();
		return local;
	}

	void UpdateMatrix(const revMatrix44& parentMatrix = revMatrix44::Identity);

private:

	revTransform* parent;
	revVector3 scale;
	revVector3 position;
	revVector3 rotation;

	revMatrix44 world;
	revMatrix44 local;
	bool dirty;
};

#endif
