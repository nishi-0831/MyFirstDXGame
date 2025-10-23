#include "Fbx.h"
#include "Direct3D.h"
#include "Quad.h"
#include "Camera.h"
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

Fbx::Fbx()
	: pVertexBuffer_{nullptr}
	, pIndexBuffer_{nullptr}
	, pConstantBuffer_{nullptr}
	, vertexCount_{-1}
	, polygonCount_{-1}
{
}

Fbx::~Fbx()
{
}

HRESULT Fbx::Load(std::string fileName)
{
	fs::path currPath = fs::current_path();
	std::string subDir("Assets");
	fs::path basePath = currPath;

	currPath = currPath / subDir;
	assert(fs::exists(currPath));
	
	fs::current_path(currPath);

	//�}�l�[�W���𐶐�
	FbxManager* pFbxManager = FbxManager::Create();

	//�C���|�[�^�[�𐶐�
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	bool result = fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());
	assert(result);
	
	//�V�[���I�u�W�F�N�g��FBX�t�@�C���̏��𗬂�����
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	result = fbxImporter->Import(pFbxScene);
	assert(result);

	fbxImporter->Destroy();

	//���b�V�������擾
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();


	vertexCount_ = mesh->GetControlPointsCount();	//���_�̐�

	polygonCount_ = mesh->GetPolygonCount();	//�|���S���̐�

	materialCount_ = pNode->GetMaterialCount(); //�}�e���A���̌�
	pMaterialList_ = new MATERIAL[materialCount_];


	//// �J�����g�f�B���N�g�����o���Ă���
	//wchar_t defaultCurrentDir[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	//// �f�B���N�g���������擾
	//wchar_t dir[MAX_PATH];
	//wchar_t file[MAX_PATH];
	//_wsplitpath_s(defaultCurrentDir,
	//	nullptr, 0,
	//	dir, MAX_PATH, 
	//	file, MAX_PATH,
	//	nullptr, 0);

	//std::wstring newDir = std::wstring(dir) + std::wstring(file);
	//// �J�����g�f�B���N�g���ύX
	//SetCurrentDirectory(newDir.c_str());

	InitVertex(mesh);
	InitIndex(mesh);
	InitConstantBuffer();
	InitMaterial(pNode);

	// �J�����g�f�B���N�g�������ɖ߂�
	//SetCurrentDirectory(defaultCurrentDir);
	fs::current_path(basePath);

	//�}�l�[�W�����
	pFbxManager->Destroy();
	return S_OK;
}

void Fbx::InitVertex(fbxsdk::FbxMesh* mesh)
{
	//���_��������z��
	VERTEX* vertices = new VERTEX[vertexCount_];

	//�S�|���S��
	for (int poly = 0; poly < polygonCount_; poly++)
	{
		//3���_��
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//���ׂ钸�_�̔ԍ�
			int index = mesh->GetPolygonVertex(poly, vertex);

			//���_�̈ʒu
			FbxVector4 pos = mesh->GetControlPointAt(index);
			vertices[index].position = DirectX::XMVectorSet(
				(float)pos[0],
				(float)pos[1],
				-(float)pos[2],
				0.0f);

			FbxVector4 normal;
			mesh->GetPolygonVertexNormal(poly, vertex, normal);
			vertices[index].normal = DirectX::XMVectorSet(

				(float)normal[0],
				(float)normal[1],
				-(float)normal[2],
				0.0f);
			

			//���_��UV
			//���_��UV
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			if (pUV)
			{

				//���_��UV
				FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();

				int UVNum = pUV->GetDirectArray().GetCount();
				int indexNum = pUV->GetIndexArray().GetCount();
				int size = UVNum > indexNum ? UVNum : indexNum;

				FbxLayerElement::EMappingMode mappingMode = pUV->GetMappingMode();
				FbxLayerElement::EReferenceMode refMode = pUV->GetReferenceMode();
				
				if (mappingMode == FbxLayerElement::eByPolygonVertex)
				{
					if (refMode == FbxLayerElement::eDirect)
					{
						FbxVector2 uv = pUV->GetDirectArray().GetAt(index);
						vertices[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);
					}
				}
				else if(mappingMode == FbxLayerElement::eByControlPoint)
				{
					if (refMode == FbxLayerElement::eIndexToDirect)
					{
						int i = pUV->GetIndexArray().GetAt(index);
						FbxVector2 uv = pUV->GetDirectArray().GetAt(i);
						vertices[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);
					}
					else if (refMode == FbxLayerElement::eDirect)
					{
						FbxVector2 uv = pUV->GetDirectArray().GetAt(index);
						vertices[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);
					}
				}
				/*int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
				FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
				vertices[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);*/

				/*FbxStringList uvSetNameList;
				mesh->GetUVSetNames(uvSetNameList);
				const char* uvSetName ="";
				if (uvSetNameList.GetCount() > 0)
				{
					uvSetName = uvSetNameList[0];
				}
				
				FbxVector2  uv{};
				bool unMapped = true;
				if(mesh->GetPolygonVertexUV(poly, index, uvSetName, uv, unMapped))
				{
					vertices[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);
				}*/
			}
		}
	}

	const D3D11_BUFFER_DESC BUFFER_DESC
	{
		.ByteWidth = sizeof(VERTEX) * vertexCount_,
		.Usage = D3D11_USAGE_DEFAULT,  // MEMO: �r���ŏ��������邽�� dynamic
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = 0,  // �r���ŏ��������邩��
		.MiscFlags = 0,
		.StructureByteStride = 0,
	};

	const D3D11_SUBRESOURCE_DATA INITIALIZE_DATA
	{
		.pSysMem = vertices,
		.SysMemPitch = 0,
		.SysMemSlicePitch = 0,
	};

	HRESULT hResult = Direct3D::pDevice->CreateBuffer(&BUFFER_DESC, &INITIALIZE_DATA, &pVertexBuffer_);
	if (FAILED(hResult))
	{
		MessageBox(NULL, L"���_�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
	}
}

void Fbx::InitIndex(fbxsdk::FbxMesh* mesh)
{
	// int�z��łȂ�VERTEX�z��(VERTEX*)
	// �|���S����3�̒��_������polygonCount_�~3
	pIndexBuffer_ = new ID3D11Buffer * [materialCount_];

	int* index = new int[polygonCount_ * 3];
	
	for (int i = 0; i < materialCount_; i++)
	{

		int count = 0;
		for (int j = 0; j < polygonCount_; j++)
		{
			FbxLayerElementMaterial* material{ mesh->GetLayer(0)->GetMaterials() };
			int materialId{ material->GetIndexArray().GetAt(j) };

			if (materialId == i)
			{
				for (int k = 0; k < 3; k++)
				{
					index[count + k] = mesh->GetPolygonVertex(j, k);
				}
				count += 3;
			}
		}


		D3D11_BUFFER_DESC bufferDesc
		{
			.ByteWidth = sizeof(DWORD) * count,
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_INDEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = 0,
		};

		D3D11_SUBRESOURCE_DATA subResData
		{
			.pSysMem = index,
			.SysMemPitch = 0,
			.SysMemSlicePitch = 0,
		};

		HRESULT hResult{};

		pMaterialList_[i].indexCount = count;
		hResult = Direct3D::pDevice->CreateBuffer(&bufferDesc, &subResData, &pIndexBuffer_[i]);

		assert(SUCCEEDED(hResult)
			&& "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s");


	}

}

void Fbx::InitConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc
	{
		.ByteWidth = sizeof(CONSTANT_BUFFER),
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
		.MiscFlags = 0,
		.StructureByteStride = 0,
	};

	HRESULT hResult = Direct3D::pDevice->CreateBuffer(&bufferDesc, nullptr, &pConstantBuffer_);

	assert(SUCCEEDED(hResult) &&
		"�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s");
}

void Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
{

	for (int i = 0; i < materialCount_; i++)
	{
		//i�Ԗڂ̃}�e���A�������擾
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);

		//�e�N�X�`�����
		fbxsdk::FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		//fbxsdk::FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::s);

		//�e�N�X�`���̐���
		int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

		//�e�N�X�`������
		if (fileTextureCount > 0)
		{
			FbxFileTexture* pTextureInfo{ lProperty.GetSrcObject<FbxFileTexture>(0) };
			const char* textureFilePath = pTextureInfo->GetRelativeFileName();
			fs::path tPath(textureFilePath);
			if (fs::is_regular_file(tPath))
			{
				int a;
				a = 0;
				a++;
			}
			
			char fileName[_MAX_FNAME];
			char ext[_MAX_EXT];
			_splitpath_s(textureFilePath,
				nullptr, 0,
				nullptr, 0,
				fileName, _MAX_FNAME,
				ext, _MAX_EXT);

			
			//�t�@�C������e�N�X�`�����쐬
			pMaterialList_[i].pTexture = new Texture();
			//std::string subDir("Assets/");
			std::string fileNameStr(fileName);
			std::string extStr(ext);
			auto a =tPath.c_str();
			pMaterialList_[i].pTexture->Load(fileNameStr + extStr);
		}
		//�e�N�X�`������
		else
		{
			// �e�N�X�`���Ȃ��Ȃ�nullptr

			pMaterialList_[i].pTexture = nullptr;

			
			FbxDouble3 color = ((FbxSurfaceLambert*)pMaterial)->Diffuse;
			pMaterialList_[i].diffuse = { (float)color[0],(float)color[1],(float)color[2],1.0f };
		}
	}
}

void Fbx::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_3D);

	///���_�o�b�t�@���C���v�b�g�A�Z���u���ɃZ�b�g
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//�R���X�^���g�o�b�t�@�𒸓_�V�F�[�_�A�s�N�Z���V�F�[�_�ɃZ�b�g
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);

	for (int i = 0; i < materialCount_; i++)
	{
		//�C���f�b�N�X�o�b�t�@���Z�b�g
		stride = sizeof(int);
		offset = 0;
		Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);


		if (pMaterialList_[i].pTexture)
		{
			ID3D11SamplerState* pSampler = pMaterialList_[i].pTexture->GetSampler();
			Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

			ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTexture->GetSRV();
			Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
		}

		XMMATRIX worldMat = transform.GetWorldMatrix();
		D3D11_MAPPED_SUBRESOURCE pdata;
		CONSTANT_BUFFER cb;
		cb.matWVP = XMMatrixTranspose(worldMat * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
		cb.matW =  worldMat;
		cb.matNormalTrans = transform.GetNormalMatrix();
		//cb.matNormalTrans = XMMatrixInverse(nullptr,worldMat);
		if (pMaterialList_[i].pTexture)
		{
			cb.materialFlag = true;
			cb.diffuse = XMFLOAT4(1,1,1,1);
		}
		else
		{
			cb.materialFlag = false;
			cb.diffuse = pMaterialList_[i].diffuse;
			//cb.diffuse = XMFLOAT4(255, 0, 0, 0);
		}

		

		//GPU����̃f�[�^�A�N�Z�X���~�߂�
		//CPU����f�[�^�n������GPU�ɑ҂��Ă��炤
		Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);

		//�f�[�^��n��
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));

		//�������ݏI���������Ă���
		Direct3D::pContext->Unmap(pConstantBuffer_, 0);

		//�`��
		// �`�悷��C���f�b�N�X�̐���n��
		Direct3D::pContext->DrawIndexed(pMaterialList_[i].indexCount, 0, 0);
		//Direct3D::pContext->DrawIndexed(pMaterialList_[i].indexCount * 3, 0, 0);
	}
}

void Fbx::Release()
{
}
