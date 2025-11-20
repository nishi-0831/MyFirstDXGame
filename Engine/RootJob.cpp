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
