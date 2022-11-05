#pragma once
#include "Vector3.h"
#include <string>

class iRobot {
public:
	Vector3		m_position;
	float		m_health;
	std::string m_name;

	virtual ~iRobot() {};

	virtual void Update(double deltaTime) = 0;
};

