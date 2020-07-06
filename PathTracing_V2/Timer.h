#pragma once
#include <chrono>
class Timer
{
public:
	Timer();
	static void Init();
	static double GetMilisecondsElapsed();
	static bool Start();
	static bool Stop();
	static void Restart();
	static std::chrono::time_point<std::chrono::steady_clock> start;
	static std::chrono::time_point<std::chrono::steady_clock> stop;
	static bool isRunning;
	static double deltaTime;
};

