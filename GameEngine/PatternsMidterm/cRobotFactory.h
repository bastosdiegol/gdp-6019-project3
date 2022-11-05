#pragma once
#include "iRobot.h"
#include <vector>

class cRobotFactory {
protected:
	cRobotFactory();

	static cRobotFactory*	m_robotFactory;

public:
	~cRobotFactory();
	cRobotFactory(cRobotFactory& other) = delete;
	void operator=(const cRobotFactory&) = delete;

	/// <summary>
	/// Singleton Method for Getting an Instance of this class
	/// </summary>
	/// <returns>Pointer of RobotFactory</returns>
	static cRobotFactory* GetInstance(void);
	/// <summary>
	/// Builds a Single iRobot and adds it to the array of Robots
	/// </summary>
	/// <returns>A pointer to the built Robot</returns>
	iRobot* BuildARobot(void);
	/// <summary>
	/// Calls Update() on all Robots that are alive
	/// </summary>
	void Update(double deltaTime);

private:
	std::vector<iRobot*> m_vRobots;
};

// Utility function for a random range of two floats
float RandFloat(float min, float max);