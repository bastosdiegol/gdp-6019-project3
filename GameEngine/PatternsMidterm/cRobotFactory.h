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
	/// <summary>
	/// Sets a new height for a robot
	/// </summary>
	/// <param name="robotId">ID of the robot who will have height adjusted</param>
	/// <param name="y">Float value for new y axis</param>
	void adjustRobotHeightPos(unsigned int robotId, float y);
	/// <summary>
	/// Returns the Robot present in the array
	/// </summary>
	/// <param name="index">index of the robot on the array</param>
	/// <returns>A instance of a Robot</returns>
	iRobot* getRobot(int index);
	/// <summary>
	/// Receives a robot and set a new random position on the terrain
	/// </summary>
	/// <param name="robot">Pointer to the robot who will get a new position</param>
	void setNewRandomPosition(iRobot* robot);

private:
	std::vector<iRobot*> m_vRobots;
};

// Utility function for a random range of two floats
float RandFloat(float min, float max);