#include "Oden.h"

using namespace DirectX;

void Oden::Initialize()
{
	pFbx_ = new Fbx();
	pFbx_->Load("Oden.fbx");

	pTransform_ = new Transform();
	pTransform_->position = XMFLOAT3(0,-1, 0);
    pTransform_->rotate = XMFLOAT3(0, 0, 0);
	pTransform_->scale = XMFLOAT3(1, 1, 1);
}

void Oden::Draw()
{
    XMVECTOR movement = { 0,0,0 };
    XMVECTOR rotation = { 0,0,0 };
    float move = 0.001f;
    float rotMove = 0.05f;
    if (GetKeyState(VK_UP))
    {
        movement.m128_f32[0] += move;
    }
    if (GetKeyState(VK_DOWN))
    {
        movement.m128_f32[0] -= move;
    }
    if (GetKeyState(VK_RIGHT))
    {
        movement.m128_f32[1] += move;
    }
    if (GetKeyState(VK_LEFT))
    {
        movement.m128_f32[1] -= move;
    }
    if (GetKeyState('R'))
    {
        rotation.m128_f32[1] -= rotMove;
    }
    XMVECTOR pos = XMLoadFloat3(&pTransform_->position);
    pos += movement;
    XMVECTOR rot = XMLoadFloat3(&pTransform_->rotate);
    rot += rotation;

    XMStoreFloat3( &pTransform_->position,pos),
    XMStoreFloat3( &pTransform_->rotate,rot),
	pFbx_->Draw(*pTransform_);
}
