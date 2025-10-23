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
	/*PlayScene* playScene = new PlayScene(this);
	playScene->Initialize();

	childList_.push_back(playScene);*/
	Instantiate<PlayScene>(this);
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
