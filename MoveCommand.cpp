#include "MoveCommand.h"
#include "Enemy.h"
#include "Engine/GameTime.h"
namespace
{
}
MoveCommand::MoveCommand()
	: duration_{5.0f}
	, elapsed_{0.0f}
	, speed_{1.0f}
	, distX_{0.0f}
{
	
}
MoveCommand::MoveCommand(float distX)
	: duration_{ 5.0f }
	, elapsed_{ 0.0f }
	, speed_{ 1.0f }
	, distX_{ distX }
{
}
void MoveCommand::Execute(Enemy* _pPlayer)
{
	using namespace DirectX;
	DirectX::XMVECTOR movement = DirectX::XMVectorScale(DirectX::XMVECTOR{-1, 0, 0, 0}, speed_* GameTime::DeltaTime());
	DirectX::XMVECTOR posV = DirectX::XMLoadFloat3(&_pPlayer->transform_.position);

	XMStoreFloat3(&_pPlayer->transform_.position, movement + posV);

	elapsed_ += GameTime::DeltaTime();
}

bool MoveCommand::Transition(Enemy* _pPlayer)
{
	if (_pPlayer->transform_.position.x <= distX_)
	{
		return true;
	}
	else
	{
		return false;
	}
}
