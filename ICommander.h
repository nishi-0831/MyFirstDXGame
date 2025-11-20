#pragma once
#include "Event.h"
#include "Engine/GameObject.h"

class IObserver
{
public:
	virtual void OnNotify(GameObject* pGameObj, Event event) = 0;
};