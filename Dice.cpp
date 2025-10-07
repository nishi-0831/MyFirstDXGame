#include "Dice.h"
#include "Quad.h"
#include <DirectXMath.h>
#include "ImGui/imgui.h"
#include "Fbx.h"

using namespace DirectX;

namespace
{
    int row = 2;
    int column = 4;
    static float rotY = 0.0f;
    
    
}

HRESULT Dice::Initialze()
{
    pTransform_ = new Transform();
    pTransform_->position = XMFLOAT3(0.0f, 1.0f, 2.0f);
    radius_ = 1.0f;
    quads_.resize(6);
    for(int i = 0 ; i< quads_.size();i++)
    {
        quads_[i] = new Quad();
        int y = i / column;
        int x = i % column;
        quads_[i]->Initialze(y,x);
        quads_[i]->pTransform_->pParent_ = pTransform_;
        //quads_[i]->pos_
        if ((i + 1) / 2 == 0)
        {
            //quads_[i].
        }
    }
    rot_ = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    
#if 1
    //前後
    //,0.0f上下
    //,0.0f左右
    quads_[0]->pTransform_->position = XMFLOAT3(0.0f, 0.0f, -radius_ * 1.0f);
    quads_[1]->pTransform_->position = XMFLOAT3(0.0f, 0.0f, radius_ * 1.0f);
    quads_[2]->pTransform_->position = XMFLOAT3(0.0f, -radius_ * 1.0f, 0.0f);
    quads_[3]->pTransform_->position = XMFLOAT3(0.0f, radius_ * 1.0f, 0.0f);
    quads_[4]->pTransform_->position = XMFLOAT3(-radius_ * 1.0f, 0.0f, 0.0f);
    quads_[5]->pTransform_->position = XMFLOAT3(radius_ * 1.0f, 0.0f, 0.0f);
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
    
    

    //quads_[0]->rot_ = rots[0] * mask[2];
    //quads_[1]->rot_ = rots[1] * mask[2];

    //,0.0f上下
    quads_[0]->pTransform_->rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
    quads_[1]->pTransform_->rotate = XMFLOAT3(0.0f, 180.0f, 180.0f);
    quads_[2]->pTransform_->rotate = XMFLOAT3(-90.0f, 0.0f, 0.0f);
    quads_[3]->pTransform_->rotate = XMFLOAT3(90.0f, 0.0f, 0.0f);
    quads_[4]->pTransform_->rotate = XMFLOAT3(0.0f, 90.0f, 0.0f);
    quads_[5]->pTransform_->rotate = XMFLOAT3(0.0f, -90.0f, 0.0f);
    //quads_[2]->rot_ = rots[0] * mask[2];
    //quads_[3]->rot_ = rots[1] * mask[2];
    //,0.0f左右
    
    //法線はquads_pos - centerで

    D3D11_BUFFER_DESC cb;
    cb.ByteWidth = sizeof(light);
    cb.Usage = D3D11_USAGE_DYNAMIC;
    cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cb.MiscFlags = 0;
    cb.StructureByteStride = 0;

    HRESULT result = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
    if (FAILED(result))
    {
        MessageBox(nullptr, L"コンスタントバッファの作成に失敗しました", L"エラー", MB_OK);
        return result;
    }

    return E_NOTIMPL;
}

void Dice::Draw()
{
    XMFLOAT3 rotate_;
    XMStoreFloat3(&rotate_, rot_);
    static XMFLOAT3 rotMovement = XMFLOAT3(0, 0, 0);
    static float rotMove = 0.0f;
    rotMove += 0.1f;
    float movement = 0.1f;
    if (GetKeyState(VK_UP))
    {
        rotMovement.x += movement;
        //rotMovement.x = 90.0f;
    }
    if (GetKeyState(VK_DOWN))
    {
        rotMovement.x -= movement;
        //rotMovement.x = -90.0f;

    }
    if (GetKeyState(VK_RIGHT))
    {
        rotMovement.y += movement;
        //rotMovement.y = 90.0f;

    }
    if (GetKeyState(VK_LEFT))
    {
        rotMovement.y -= movement;
        //rotMovement.y = -90.0f;

    }
    if (GetKeyState('Q'))
    {
        rotMovement.z += movement;
        //rotMovement.z = 90.0f;
    }
    if (GetKeyState('E'))
    {
        rotMovement.z -= movement;
        //rotMovement.z = -90.0f;
    }

    XMMATRIX rotMat;
    
    rotY += 0.03;
    
   /* XMMATRIX rotateX, rotateY, rotateZ;
    rotateX = XMMatrixRotationX(XMConvertToRadians(rotate_.x + rotMovement.x));
    rotateY = XMMatrixRotationY(XMConvertToRadians(rotate_.y + rotMovement.y));
    rotateZ = XMMatrixRotationZ(XMConvertToRadians(rotate_.z + rotMovement.z));



    rotMat = rotateZ * rotateX * rotateY;
    XMFLOAT3 pos;
    XMStoreFloat3(&pos, pos_);
    XMMATRIX transMat = XMMatrixTranslation(pos.x, pos.y, pos.z);
    XMMATRIX scaleMat = XMMatrixScaling(1, 2, 5);
    XMMATRIX worldMat = scaleMat* rotMat * transMat;*/

   /* XMMATRIX worldMat = pTransform_->GetWorldMatrix();

    XMMATRIX normalTransMat = XMMatrixTranspose(rotMat);*/
    //XMMATRIX normalTransMat = XMMatrixTranspose(rotMat * XMMatrixInverse(nullptr, scaleMat));

    D3D11_MAPPED_SUBRESOURCE pdata;
    light light;
    light.lightRotMat = XMMatrixRotationY(XMConvertToRadians(rotMove));
    Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);

    memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&light), sizeof(light));
    //書き込み終わったよってする
    Direct3D::pContext->Unmap(pConstantBuffer_, 0);

    //コンスタントバッファを頂点シェーダ、ピクセルシェーダにセット
    //Simple3D.hlslの二番目コンスタントバッファ,lightに渡したいので第一引数に1と指定している。0オリジンだからね
    Direct3D::pContext->VSSetConstantBuffers(1, 1, &pConstantBuffer_);
    Direct3D::pContext->PSSetConstantBuffers(1, 1, &pConstantBuffer_);

    for (auto quad : quads_)
    {
        quad->Draw();
        //quad->pFbx_->Draw()
    }

    

    //quads_[0]->pos_ = XMMATRIX
    //quads_[0]->Draw();
    //前後のはy軸180
    //quads_[0]->rot_ = XMVectorSet(0.0f, 0.0f, 00.0f, 0.0f);
}
