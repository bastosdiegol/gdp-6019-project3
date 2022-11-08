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
		if (m_vRobots[i]->getHealth() > 0) {
			m_vRobots[i]->Update(deltaTime);
		}
	}
}

void cRobotFactory::adjustRobotHeightPos(unsigned int robotId, float y) {
	DEBUG_PRINT("cRobotFactory::adjustRobotHeightPos(%d, %f)\n", robotId, y);
	for (int i = 0; i < m_vRobots.size(); i++) {
		if (m_vRobots[i]->getID() == robotId) {
			m_vRobots[i]->setHeight(y);
		}
	}
}

iRobot* cRobotFactory::getRobot(int index) {
	return m_vRobots[index];
}

void cRobotFactory::setNewRandomPosition(iRobot* robot) {
	// We gonna set Y later comparing with the closest triangle
	robot->setPosition(RandFloat(-128.0f, 128.0f),
					   0.0f,
					   RandFloat(-128.0f, 128.0f));
}

iRobot* cRobotFactory::findNearestRobot(iRobot* robot) {
	int indexClosestRobot = -1;
	float smallestDistance = 128 * 1.414; // Diagonal of the Arena - just a big value
	float newDistance;
	for (int i = 0; i < m_vRobots.size(); i++) {
		if (m_vRobots[i]->getID() != robot->getID()) {
			newDistance = glm::distance(robot->getPosition().getGlmVec3(),
										m_vRobots[0]->getPosition().getGlmVec3());
			if (newDistance < smallestDistance) {
				// TODO: Conditional for Weapon Type here
				// Laser and Bullet do something
				// Bomb do another thing
				Vector3 targetToRobot = m_vRobots[i]->getPosition() -
										robot->getPosition();
				targetToRobot.Normalize();
				if (hasLineOfSight(robot, m_vRobots[i], targetToRobot)) {
					newDistance = smallestDistance;
					indexClosestRobot = i;
				}
			}
		}
	}
	if (indexClosestRobot == -1)
		return nullptr;
	else
		return m_vRobots[indexClosestRobot];
}

void cRobotFactory::setTerrain(cModel* terrain) {
	this->m_terrain = terrain;
}

bool cRobotFactory::hasLineOfSight(iRobot* robot, iRobot* target, Vector3 direction) {
	for (int terrainIndex = 0; terrainIndex < m_terrain->numberOfTriangles; terrainIndex++) {
		// TODO: Now we should iterate through the terrain
	}
	return false;
}

void cRobotFactory::caculatePlaneTrianglesCenter(cModel* terrainModel) {
	m_vPlaneTrianglesCenter = new std::vector<glm::vec3>();
	// Initial reserve to contain all the centers for the terrain
	//g_vTerrainTrianglesCenter->reserve(terrainModel->numberOfTriangles);
	glm::vec3 triangleCenter;

	// We gonna iterate through each triangle of the model
	for (int i = 0; i < terrainModel->numberOfIndices; i) {
		// Checks if this triangle is a plane (all 3 vertices has same Y)
		if ((terrainModel->pVertices[terrainModel->pIndices[i + 0]].y ==
			terrainModel->pVertices[terrainModel->pIndices[i + 1]].y) &&
			(terrainModel->pVertices[terrainModel->pIndices[i + 0]].y ==
				terrainModel->pVertices[terrainModel->pIndices[i + 2]].y)) {

			// Center Triangle Terrain
			// = (CenterX, CenterZ)
			// CenterX = (V1x + V2x + V3x) / 3
			triangleCenter.x = (terrainModel->pVertices[terrainModel->pIndices[i + 0]].x +
				terrainModel->pVertices[terrainModel->pIndices[i + 1]].x +
				terrainModel->pVertices[terrainModel->pIndices[i + 2]].x) / 3;
			// CenterZ = (V1z + V2z + V3z) / 3
			triangleCenter.z = (terrainModel->pVertices[terrainModel->pIndices[i + 0]].z +
				terrainModel->pVertices[terrainModel->pIndices[i + 1]].z +
				terrainModel->pVertices[terrainModel->pIndices[i + 2]].z) / 3;
			// For the height we gonna consider the first vertice Y
			triangleCenter.y = terrainModel->pVertices[terrainModel->pIndices[i]].y;

			m_vPlaneTrianglesCenter->push_back(triangleCenter);
		}
		i += 3;
	}
}

// Utility function for a random range of two floats
float RandFloat(float min, float max) {
	//DEBUG_PRINT("RandFloat(%f, %f)\n", min, max);
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}