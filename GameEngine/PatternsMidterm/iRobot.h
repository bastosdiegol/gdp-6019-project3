#pragma once
#include "Vector3.h"
#include <string>

class iRobot {
public:
	Vector3		m_position;
	int			m_health;
	std::string m_name;

	virtual ~iRobot() {};

	virtual void Update(double deltaTime) = 0;
	virtual void ChangeSpawnLocation() = 0;
};

