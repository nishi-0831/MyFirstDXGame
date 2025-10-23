#include "RootJob.h"

RootJob::RootJob(GameObject* pParent)
	: GameObject(pParent, "RootJob")
{
}

RootJob::~RootJob()
{
}

void RootJob::Initialize()
{
	//Instantiate<PlayScene>(this);
	 Instantiate<SceneManager>(this); 
}

void RootJob::Update()
{
}

void RootJob::Draw()
{
}

void RootJob::Release()
{
}
