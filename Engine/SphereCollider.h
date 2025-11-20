#pragma once
#include "Transform.h"
class SphereCollider
{
public:
	SphereCollider(float radius);
	void SetTransform(Transform* pTransform) { pTransform_ = pTransform; }
	void SetOffset(DirectX::XMVECTOR offset);
	float GetRadius() { return radius_; }
	bool IsHit(const SphereCollider& _other) const;
private:
	float radius_;
	DirectX::XMVECTOR offset_;
	Transform* pTransform_;
};