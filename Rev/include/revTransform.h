#ifndef __REVTRANSFORM_H__
#define __REVTRANSFORM_H__

#include "revMath.h"
#include "revArray.h"

class revTransform
{
public:
	revTransform(revTransform* parent = nullptr);
	virtual ~revTransform();

	revTransform* GetParent() const { return parent; }
	const revVector3& GetScale() const { return scale; }
	const revVector3& GetPosition() const { return position; }
	const revQuaternion& GetRotaion() const { return rotation; }

	revVector3 GetFoward() const 
	{
		return rotation.Vector3TransformCoord(revVector3::FOWARD);
	}

	revVector3 GetRight() const
	{
		return rotation.Vector3TransformCoord(revVector3::RIGHT);
	}

	revVector3 GetUp() const
	{
		return rotation.Vector3TransformCoord(revVector3::UP);
	}


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
	void SetRotation(const revQuaternion& rotation) 
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

	void UpdateMatrix(const revMatrix44& parentMatrix = revMatrix44::IDENTITY);
	void LookAt(const revVector3& target, const revVector3& up = revVector3::UP);

	struct CBuffer
	{
		revMatrix44 view;
		revMatrix44 projection;
		revMatrix44 world;
		revMatrix44 wvp;
	};

private:

	revTransform* parent;
	revVector3 scale;
	revVector3 position;
	revQuaternion rotation;

	revMatrix44 world;
	revMatrix44 local;
	bool dirty;
};

#endif
