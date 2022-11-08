#include "cRobot.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

unsigned int cRobot::m_nextID = cRobot::STARTING_ID;

cRobot::cRobot() {
	DEBUG_PRINT("cRobot::cRobot()\n");
	this->m_id = cRobot::m_nextID;
	cRobot::m_nextID += ID_VALUE_INCREMENT;
	this->m_health = 100.0f;
}

cRobot::~cRobot() {
	DEBUG_PRINT("cRobot::~cRobot()\n");
}

void cRobot::Update(double deltaTime) {
	DEBUG_PRINT("cRobot::Update(%f)\n", deltaTime);
	delete this;
}

unsigned int cRobot::getID() {
	return this->m_id;
}

void cRobot::setPosition(float x, float y, float z) {
	this->m_position.x = x;
	this->m_position.y = y;
	this->m_position.z = z;
}

Vector3 cRobot::getPosition() {
	return this->m_position;
}

float cRobot::getHealth() {
	return this->m_health;
}

void cRobot::setHeight(float y) {
	this->m_position.y = y;
}
