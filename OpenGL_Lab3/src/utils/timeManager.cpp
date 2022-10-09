#include "timeManager.h"
#include <glfw3.h>

float Time::m_lastTime = 0;
float Time::m_deltaTime = 0;

void Time::SetNow()
{
	Time::m_lastTime = glfwGetTime();
}

void Time::Calculate()
{
	Time::m_deltaTime = glfwGetTime() - Time::m_lastTime;
}

float Time::GetDeltaTime()
{
	return Time::m_deltaTime;
}
