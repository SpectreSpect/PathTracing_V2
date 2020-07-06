#include "Timer.h"

std::chrono::time_point<std::chrono::steady_clock> Timer::start;
std::chrono::time_point<std::chrono::steady_clock> Timer::stop;
bool Timer::isRunning = false;
double Timer::deltaTime = 0;
Timer::Timer() 
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}
void Timer::Init()
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}
double Timer::GetMilisecondsElapsed()
{
	if (isRunning)
	{
		auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
		return elapsed.count();
	}
	else
	{
		auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
		return elapsed.count();
	}
}
bool Timer::Start()
{
	if (isRunning)
		return false;
	else 
	{
		start = std::chrono::high_resolution_clock::now();
		isRunning = true;
	}
}

bool Timer::Stop()
{
	if (!isRunning)
		return false;
	else
	{
		stop = std::chrono::high_resolution_clock::now();
		isRunning = false;
		return true;
	}

}

void Timer::Restart()
{
	isRunning = true;
	start = std::chrono::high_resolution_clock::now();
}
