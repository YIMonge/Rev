#include "revTransform.h"

revTransform::revTransform(revTransform* parent) :
scale(revVector3(1.0f, 1.0f, 1.0f)),
rotation(revVector3(0.0f, 0.0f, 0.0f)),
position(revVector3(0.0f, 0.0f, 0.0f)),
dirty(false)
{
	this->parent = parent;
	UpdateMatrix();
}

revTransform::~revTransform()
{
}

void revTransform::UpdateMatrix(const revMatrix44& parentMatrix)
{
	revMatrix44 s, r, t;
	s.Scaling(scale);
	r.RotationXYZ(rotation);
	t.Translation(position);
	local = s * r * t;
	if (parent != nullptr) world = parent->GetWorldMatrix() * local;
	else world = local;
	dirty = false;
}
