#include "SphereCollider.h"
#include <DirectXMath.h>

using namespace DirectX;

SphereCollider::SphereCollider(float radius)
	: radius_{radius}
	, offset_{DirectX::XMVectorSet(0,0,0,0)}
	, pTransform_{nullptr}
{
}

void SphereCollider::SetOffset(DirectX::XMVECTOR offset)
{
	offset_ = offset;
}

bool SphereCollider::IsHit(const SphereCollider& _other) const
{
	XMVECTOR pos = XMLoadFloat3(&pTransform_->position) + offset_;
	DirectX::XMVECTOR otherPos = XMLoadFloat3(&_other.pTransform_->position);
	
	XMVECTOR vector = pos - otherPos;
	float distanceSq = XMVector3LengthSq(pos - otherPos).m128_f32[0];

	float hitThreshold = radius_ + _other.radius_;
	float hitThresholdSq = hitThreshold * hitThreshold;

	if (distanceSq < hitThresholdSq)
	{
		return true;
	}
	else
	{
		return false;
	}
}
