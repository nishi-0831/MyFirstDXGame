#include "Transform.h"

using namespace DirectX;

Transform::Transform() : 
    pParent_{nullptr}
{
    position = XMFLOAT3(0, 0, 0);
    rotate = XMFLOAT3(0, 0, 0);
    scale = XMFLOAT3(1, 1, 1);
    matTranslate_ = XMMatrixIdentity();
    matRotate_ = XMMatrixIdentity();
    matScale_ = XMMatrixIdentity();
}

Transform::~Transform()
{
}

void Transform::Calculation()
{
    matTranslate_ = XMMatrixTranslation(position.x, position.y, position.z);

    XMMATRIX rotateZ,rotateX, rotateY;
    rotateZ = XMMatrixRotationZ(XMConvertToRadians(rotate.z));
    rotateX = XMMatrixRotationX(XMConvertToRadians(rotate.x));
    rotateY = XMMatrixRotationY(XMConvertToRadians(rotate.y));

    matRotate_ = rotateZ * rotateX * rotateY;

    matScale_ = XMMatrixScaling(scale.x, scale.y, scale.z);
}

XMMATRIX Transform::GetWorldMatrix()
{
    if (pParent_) 
    {
        Calculation();
        return matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
    }
    
    return matScale_ * matRotate_ * matTranslate_;
}

DirectX::XMMATRIX Transform::GetWorldRotMatrix()
{
    if (pParent_)
    {
        Calculation();
        return matRotate_ * pParent_->GetWorldRotMatrix();
    }
    return matRotate_;
}

DirectX::XMMATRIX Transform::GetNormalMatrix()
{
    return  DirectX::XMMatrixTranspose( GetWorldRotMatrix() * DirectX::XMMatrixInverse(nullptr,matScale_));
}
