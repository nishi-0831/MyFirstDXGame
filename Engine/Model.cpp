#include "Model.h"
#include <vector>
namespace Model
{
	std::vector<ModelData*> modelDatas;
}
int Model::Load(std::string fileName)
{
	ModelData* data = new ModelData();
	bool isExist = false;
	//開いたファイル一覧から同じファイル名のものが無いか探す
	for (int i = 0; i < modelDatas.size(); i++)
	{
		if (modelDatas[i] != nullptr && modelDatas[i]->fileName_ == fileName)
		{
			data->pFbx_ = modelDatas[i]->pFbx_;
			data->fileName_ = fileName;
			isExist = true;
			break;
		}
	}

	//新たにファイルを開く
	if (isExist == false)
	{
		Fbx* fbx = new Fbx();
		HRESULT hResult = fbx->Load(fileName);
		if (FAILED(hResult))
		{
			SAFE_DELETE(fbx);
			SAFE_DELETE(data);
			return -1;
		}
		data->pFbx_ = fbx;
		data->fileName_ = fileName;
	}

	//使ってない番号が無いか探す
	for (int i = 0; i < modelDatas.size(); i++)
	{
		if (modelDatas[i] == nullptr)
		{
			modelDatas[i] = data;
			return i;
		}
	}

	// 新たに追加
	modelDatas.push_back(data);
	return static_cast<int>(modelDatas.size() - 1);
}

void Model::SetTransform(int hModel, const Transform& transform)
{
	ModelData* modelData = modelDatas[hModel];
	if (hModel < 0 || hModel >= modelDatas.size() || modelData == nullptr)
	{
		return;
	}

	modelData->transform_ = transform;
}

void Model::Draw(int hModel)
{
	ModelData* modelData = modelDatas[hModel];
	if (hModel < 0 || hModel >= modelDatas.size() || modelData == nullptr)
	{
		return;
	}

	modelData->pFbx_->Draw(modelData->transform_);
}

void Model::Release()
{
	bool isReffred = false;
	for (int i = 0; i < modelDatas.size(); i++)
	{
		isReffred = false;
		for (int j = i + 1; j < modelDatas.size(); j++)
		{
			if (modelDatas[i]->pFbx_ == modelDatas[j]->pFbx_)
			{
				isReffred = true;
				break;
			}
		}
		if (isReffred == false)
		{
			SAFE_DELETE(modelDatas[i]->pFbx_);
		}
		SAFE_DELETE(modelDatas[i]);
	}
	modelDatas.clear();
}
