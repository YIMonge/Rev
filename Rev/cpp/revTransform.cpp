#include "revTransform.h"

revTransform::revTransform(revTransform* parent) :
scale(revVector3(1.0f, 1.0f, 1.0f)),
position(revVector3(0.0f, 0.0f, 0.0f))
{
	rotation.Identity();
	this->parent = parent;
	if(parent != nullptr) parent->children.push_back(this);
	UpdateMatrix();
}

revTransform::~revTransform()
{
	if (parent != nullptr) {
		parent->children.erase(std::find(parent->children.begin(), parent->children.end(), this));
	}
}

void revTransform::UpdateMatrix()
{
	revMatrix44 s, r, t;
	s.Scaling(scale);
	r = rotation.CreateRotationMatrix();
	t.Translation(position);
	local = s * r * t;
	if (parent != nullptr) world = parent->GetWorldMatrix() * local;
	else world = local;
}
