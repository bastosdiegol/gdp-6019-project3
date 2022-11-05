#include "cRobot.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cRobot::cRobot() {
	DEBUG_PRINT("cRobot::cRobot()\n");
	this->m_health = 100.0f;
}

void cRobot::Update(double deltaTime) {
	DEBUG_PRINT("cRobot::Update(%f)\n", deltaTime);
	delete this;
}
