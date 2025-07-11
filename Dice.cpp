#include "Dice.h"
#include "Quad.h"
#include <DirectXMath.h>


using namespace DirectX;

namespace
{
    int row = 2;
    int column = 4;
    static float rotY = 0.0f;
    
    
}

HRESULT Dice::Initialze()
{
    pos_ = XMVectorSet(0.0f, 0.0f, 2.0f,0.0f);
    radius_ = 1.0f;
    quads_.resize(6);
    for(int i = 0 ; i< quads_.size();i++)
    {
        quads_[i] = new Quad();
        int y = i / column;
        int x = i % column;
        quads_[i]->Initialze(y,x);
        //quads_[i]->pos_
    }
    rot_ = XMVectorSet(90.0f, 0.0f, 0.0f, 0.0f);
    
#if 1
    //前後
    quads_[0]->pos_ = XMVectorSet(0.0f, 0.0f, -radius_ * 1.0f,0.0f);
    quads_[1]->pos_ = XMVectorSet(0.0f, 0.0f, radius_ * 1.0f,0.0f);
    //,0.0f上下
    quads_[2]->pos_ = XMVectorSet(0.0f, -radius_ * 1.0f, 0.0f,0.0f);
    quads_[3]->pos_ = XMVectorSet(0.0f, radius_ * 1.0f, 0.0f,0.0f);
    //,0.0f左右
    quads_[4]->pos_ = XMVectorSet(-radius_ * 1.0f, 0.0f, 0.0f,0.0f);
    quads_[5]->pos_ = XMVectorSet(radius_ * 1.0f, 0.0f, 0.0f,0.0f);
#endif
    
    //前
    /*XMVECTOR rots[6]
        =
    {
        XMVectorSet
    };*/
    float r = 90.0f;
    XMVECTOR rots[2] =
    {
        XMVectorSet(r,r,r,r),
        XMVectorSet(-r,-r,-r,-r),
    };

    XMVECTOR mask[3] =
    {
        XMVectorSet(0.0f,0.0f,1.0f,0.0f),
        XMVectorSet(0.0f,1.0f,0.0f,0.0f),
        XMVectorSet(1.0f,0.0f,0.0f,0.0f),
    };
    
    

    quads_[0]->rot_ = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    quads_[1]->rot_ = XMVectorSet(0.0f, 180.0f, 0.0f, 0.0f);
    //quads_[0]->rot_ = rots[0] * mask[2];
    //quads_[1]->rot_ = rots[1] * mask[2];

    //,0.0f上下
    quads_[2]->rot_ = XMVectorSet(-90.0f, 0.0f, 0.0f, 0.0f);
    quads_[3]->rot_ = XMVectorSet(90.0f, 0.0f, 0.0f, 0.0f);
    //quads_[2]->rot_ = rots[0] * mask[2];
    //quads_[3]->rot_ = rots[1] * mask[2];
    //,0.0f左右
    quads_[4]->rot_ = XMVectorSet(0.0f, 90.0f, 0.0f, 0.0f);
    quads_[5]->rot_ = XMVectorSet(0.0f, -90.0f, 0.0f, 0.0f);
    
    
    return E_NOTIMPL;
}

void Dice::Draw()
{
    XMFLOAT3 rotate_;
    XMStoreFloat3(&rotate_, rot_);
    static XMFLOAT3 rotMovement = XMFLOAT3(0, 0, 0);
    float movement = 0.1f;
    if (GetKeyState(VK_UP))
    {
        rotMovement.x += movement;
    }
    if (GetKeyState(VK_DOWN))
    {
        rotMovement.x -= movement;

    }
    if (GetKeyState(VK_RIGHT))
    {
        rotMovement.y += movement;

    }
    if (GetKeyState(VK_LEFT))
    {
        rotMovement.y -= movement;
    }
    if (GetKeyState(VK_LBUTTON))
    {
        rotMovement.z += movement;
    }
    if (GetKeyState(VK_RBUTTON))
    {
        rotMovement.z -= movement;
    }

    XMMATRIX rotMat;
    
    rotY += 0.03;
    
    XMMATRIX rotateX, rotateY, rotateZ;
    rotateX = XMMatrixRotationX(XMConvertToRadians(rotate_.x + rotMovement.x));
    rotateY = XMMatrixRotationY(XMConvertToRadians(rotate_.y + rotMovement.y));
    rotateZ = XMMatrixRotationZ(XMConvertToRadians(rotate_.z + rotMovement.z));


    rotMat = rotateZ * rotateX * rotateY;
    XMFLOAT3 pos;
    XMStoreFloat3(&pos, pos_);
    XMMATRIX transMat = XMMatrixTranslation(pos.x, pos.y, pos.z);
    XMMATRIX scaleMat = XMMatrixScaling(1, 1, 1);
    XMMATRIX worldMat = scaleMat* rotMat * transMat;

    for (auto quad : quads_)
    {
        quad->Draw(worldMat);
    }
   
    //quads_[0]->Draw();
    //前後のはy軸180
    //quads_[0]->rot_ = XMVectorSet(0.0f, 0.0f, 00.0f, 0.0f);
}
