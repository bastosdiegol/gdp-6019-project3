#include "cRobotFactory.h"

#define TOTAL_NUM_OF_ROBOTS 10

int main(int argc, char* argv[]) {
	cRobotFactory* g_robotFactory = cRobotFactory::GetInstance();
	for (int i = 0; i < TOTAL_NUM_OF_ROBOTS; i++) {
		g_robotFactory->BuildARobot();
	}
	delete g_robotFactory;
	return 0;
}