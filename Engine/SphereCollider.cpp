#include "SphereCollider.h"
#include <DirectXMath.h>

using namespace DirectX;

//float GetSizeSq(const XMFLOAT3& vec)
//{
//	return (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
//}



SphereCollider::SphereCollider(float radius)
	: radius_{radius}
	, pTransform_{nullptr}
{
}

bool SphereCollider::IsHit(const SphereCollider& _other) const
{
	XMVECTOR pos = XMLoadFloat3(&pTransform_->position);
	DirectX::XMVECTOR otherPos = XMLoadFloat3(&_other.pTransform_->position);
	
	XMVECTOR vector = pos - otherPos;
	//float distance = XMVector3Length(pos - otherPos).m128_f32[0];
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
