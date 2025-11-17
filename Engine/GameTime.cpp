#include "GameTime.h"

using namespace std::chrono;

GameTime* GameTime::pInstance_{ nullptr };

void GameTime::Initialize()
{
    if (pInstance_ == nullptr)
    {
        pInstance_ = new GameTime();
    }
    pInstance_->prevTime_ = high_resolution_clock::now();
    pInstance_->currTime_ = pInstance_->prevTime_;
}

void GameTime::Refresh()
{
    pInstance_->currTime_ = high_resolution_clock::now();
    long long deltaTimeLL = duration_cast<std::chrono::milliseconds>(pInstance_->currTime_ - pInstance_->prevTime_).count();
    pInstance_->deltaTime_ = static_cast<float>(deltaTimeLL) / 1000.0f;
    pInstance_->prevTime_ = pInstance_->currTime_;
}

float GameTime::DeltaTime()
{
    return pInstance_->deltaTime_;
}
