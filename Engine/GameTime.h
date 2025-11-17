#pragma once
#include <iostream>
#include <chrono>



class  GameTime
{
public:
	static void Initialize();
	static void Refresh();
	/// <summary>
	/// 秒単位のデルタタイムを返す
	/// </summary>
	/// <returns></returns>
	static float DeltaTime();
private:
	static GameTime* pInstance_;
	std::chrono::high_resolution_clock::time_point prevTime_;
	std::chrono::high_resolution_clock::time_point currTime_;
	float deltaTime_;
};