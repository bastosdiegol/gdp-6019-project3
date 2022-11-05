#pragma once
#include "iRobot.h"
#include <vector>

class cRobotFactory {
protected:
	cRobotFactory();
	~cRobotFactory();

	static cRobotFactory*	m_robotFactory;

public:
	cRobotFactory(cRobotFactory& other) = delete;
	void operator=(const cRobotFactory&) = delete;

	static cRobotFactory* GetInstance(void);
	iRobot* buildARobot(void);

private:
	std::vector<iRobot*> m_vRobots;
};