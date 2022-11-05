#include "cRobotFactory.h"
#include "cRobot.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cRobotFactory* cRobotFactory::m_robotFactory = nullptr;

cRobotFactory::cRobotFactory() {
	DEBUG_PRINT("cRobotFactory::cRobotFactory()");
}

cRobotFactory::~cRobotFactory() {
	DEBUG_PRINT("cRobotFactory::~cRobotFactory()");
	delete m_robotFactory;
}

cRobotFactory* cRobotFactory::GetInstance() {
	DEBUG_PRINT("cRobotFactory::GetInstance()");
	if (m_robotFactory == nullptr) {
		m_robotFactory = new cRobotFactory();
	}
	return m_robotFactory;
}

iRobot* cRobotFactory::buildARobot() {
	DEBUG_PRINT("cRobotFactory::buildARobot()");	
	iRobot* newRobot = new cRobot();
	this->m_vRobots.push_back(newRobot);
	return newRobot;	 
}
