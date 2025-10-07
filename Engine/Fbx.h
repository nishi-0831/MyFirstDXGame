#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include "Texture.h"
#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

class Fbx
{
	/*struct CONSTANT_BUFFER
	{
		DirectX::XMMATRIX	matWVP;
		DirectX::XMMATRIX	matNormal;
	};*/
	struct FBX_CONSTANT_BUFFER
	{
		DirectX::XMMATRIX	matWVP;
		DirectX::XMMATRIX	matNormal;
		DirectX::XMFLOAT4 diffuse;
		BOOL materialFlag;
		//float padding[3];
	};
	struct MATERIAL
	{
		Texture* pTexture;
		int indexCount;
		DirectX::XMFLOAT4 diffuse;
	};
	struct VERTEX
	{
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR uv;
		DirectX::XMVECTOR normal;
	};

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_; //マテリアルの個数

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	MATERIAL* pMaterialList_;

public:

	Fbx();
	HRESULT Load(std::string fileName);
	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);
	void    Draw(Transform& transform);
	void    Release();
};