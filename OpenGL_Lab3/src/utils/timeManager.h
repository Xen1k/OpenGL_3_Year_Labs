#pragma once
class Time
{
private:
	static float m_deltaTime;
	static float m_lastTime;
public:
	static void SetNow();
	static void Calculate();
	static float GetDeltaTime();
};

