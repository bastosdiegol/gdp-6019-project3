#include "cRobotFactory.h"
#include "cRobot.h"
#include <glm/geometric.hpp>

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
	DEBUG_PRINT("cRobotFactory::cRobotFactory()\n");
}

cRobotFactory::~cRobotFactory() {
	DEBUG_PRINT("cRobotFactory::~cRobotFactory()\n");
	for (int i = 0; i < m_vRobots.size(); i++) {
		m_vRobots[i]->~iRobot();
	}
}

cRobotFactory* cRobotFactory::GetInstance() {
	DEBUG_PRINT("cRobotFactory::GetInstance()\n");
	if (m_robotFactory == nullptr) {
		m_robotFactory = new cRobotFactory();
	}
	return m_robotFactory;
}

iRobot* cRobotFactory::BuildARobot() {
	DEBUG_PRINT("cRobotFactory::buildARobot()\n");	
	iRobot* newRobot = new cRobot();
	// We could receive the min and max X for reference by lets go hardcoded for while
	static_cast<cRobot*>(newRobot)->m_position.x = RandFloat(-128.0f, 128.0f);
	// We gonna set Y later comparing with the closest triangle
	static_cast<cRobot*>(newRobot)->m_position.y = 0.0f;
	static_cast<cRobot*>(newRobot)->m_position.z = RandFloat(-128.0f, 128.0f);
	this->m_vRobots.push_back(newRobot);
	return newRobot;	 
}

void cRobotFactory::Update(double deltaTime) {
	DEBUG_PRINT("cRobotFactory::Update(%f)\n", deltaTime);
	for (int i = 0; i < m_vRobots.size(); i++) {
		if (static_cast<cRobot*>(m_vRobots[i])->m_health > 0) {
			m_vRobots[i]->Update(deltaTime);
		}
	}
}

void cRobotFactory::adjustRobotHeightPos(unsigned int robotId, float y) {
	DEBUG_PRINT("cRobotFactory::adjustRobotHeightPos(%d, %f)\n", robotId, y);
	for (int i = 0; i < m_vRobots.size(); i++) {
		if (m_vRobots[i]->getID() == robotId) {
			static_cast<cRobot*>(m_vRobots[i])->m_position.y = y;
		}
	}
}

iRobot* cRobotFactory::getRobot(int index) {
	return m_vRobots[index];
}

void cRobotFactory::setNewRandomPosition(iRobot* robot) {
	static_cast<cRobot*>(robot)->m_position.x = RandFloat(-128.0f, 128.0f);
	// We gonna set Y later comparing with the closest triangle
	static_cast<cRobot*>(robot)->m_position.y = 0.0f;
	static_cast<cRobot*>(robot)->m_position.z = RandFloat(-128.0f, 128.0f);
}

iRobot* cRobotFactory::findNearestRobot(iRobot* robot) {
	int indexClosestRobot = -1;
	float smallestDistance = 128 * 1.414; // Diagonal of the Arena - just a big value
	float newDistance;
	for (int i = 0; i < m_vRobots.size(); i++) {
		if (m_vRobots[i]->getID() != robot->getID()) {
			newDistance = glm::distance(static_cast<cRobot*>(robot)->m_position.getGlmVec3(),
										static_cast<cRobot*>(m_vRobots[0])->m_position.getGlmVec3());
			if (newDistance < smallestDistance) {
				// TODO: Conditional for Weapon Type here
				// Laser and Bullet do something
				// Bomb do another thing
				Vector3 targetToRobot = static_cast<cRobot*>(m_vRobots[i])->m_position -
										static_cast<cRobot*>(robot)->m_position;
				targetToRobot.Normalize();
				// Now we should iterate through the terrain
				// But the terrain is on the Graphics Project
				// =(
				newDistance = smallestDistance;
				indexClosestRobot = i;
			}
		}
	}
	if (indexClosestRobot == -1)
		return nullptr;
	else
		return m_vRobots[indexClosestRobot];
}

// Utility function for a random range of two floats
float RandFloat(float min, float max) {
	DEBUG_PRINT("RandFloat(%f, %f)\n", min, max);
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}
