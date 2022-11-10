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

iRobot* cRobotFactory::BuildARobot(iWeapon* weapon) {
	DEBUG_PRINT("cRobotFactory::buildARobot()\n");	
	iRobot* newRobot = new cRobot();
	// We could receive the min and max X for reference by lets go hardcoded for while
	// We gonna set Y later comparing with the closest triangle
	newRobot->setPosition(RandFloat(-128.0f, 128.0f), 0.0f, RandFloat(-128.0f, 128.0f));
	newRobot->setWeapon(weapon);
	this->m_vRobots.push_back(newRobot);
	this->m_mMapOfTargets.try_emplace(newRobot->getID(), -1); // Adds the robot with no target
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
	float smallestDistance = 256 * 1.414; // Diagonal of the Arena - just a big value
	float newDistance;
	for (int i = 0; i < m_vRobots.size(); i++) {
		if (m_vRobots[i]->getID() != robot->getID()) {
			DEBUG_PRINT("	New Target Robot[%d] Position(%.1f,%.1f,%.1f) found.\n", m_vRobots[i]->getID(), m_vRobots[i]->getPosition().x
				, m_vRobots[i]->getPosition().y, m_vRobots[i]->getPosition().z);
			DEBUG_PRINT("		... Comparing Distance [Origin, Target] = %f\n", glm::distance(robot->getPosition().getGlmVec3(),
				m_vRobots[i]->getPosition().getGlmVec3()));
			DEBUG_PRINT("		... Comparing Distance [Target, Origin] = %f\n", glm::distance(m_vRobots[i]->getPosition().getGlmVec3(),
				robot->getPosition().getGlmVec3()));
			newDistance = glm::distance(robot->getPosition().getGlmVec3(),
										m_vRobots[i]->getPosition().getGlmVec3());
			if (newDistance < smallestDistance) {
				// TODO: Conditional for Weapon Type here
				// Laser and Bullet do something
				// Bomb do another thing
				DEBUG_PRINT("			New small distance ... From Robot [%d] To Robot[%d] Distance to it: %f.\n", robot->getID(), m_vRobots[i]->getID(), newDistance);
				Vector3 targetToRobot = m_vRobots[i]->getPosition() - robot->getPosition();
				targetToRobot.Normalize();
				DEBUG_PRINT("			Normal Vector to Target Robot[%d]: ", m_vRobots[i]->getID());
				targetToRobot.vOut(); DEBUG_PRINT("\n");
				if (hasLineOfSight(robot, m_vRobots[i], targetToRobot.getGlmVec3())) {
					smallestDistance = newDistance;
					indexClosestRobot = i;
					DEBUG_PRINT("				Has LOS!\n");
				}else

					DEBUG_PRINT("				LOS check FAILED!\n");
			}
		}
	}
	if (indexClosestRobot == -1) {
		std::map<int, int>::iterator it = m_mMapOfTargets.find(robot->getID());
		if (it != m_mMapOfTargets.end())
			it->second = -1; // Set's no target for this robot
		return nullptr;
	}
	else {
		std::map<int, int>::iterator it = m_mMapOfTargets.find(robot->getID());
		if (it != m_mMapOfTargets.end())
			it->second = m_vRobots[indexClosestRobot]->getID(); // Set's new target for this robot
		return m_vRobots[indexClosestRobot];
	}
}

void cRobotFactory::setTerrain(cModel* terrain) {
	this->m_terrain = terrain;
}

bool cRobotFactory::hasLineOfSight(iRobot* robot, iRobot* target, glm::vec3 direction) {
	// Initial Position of the LOS

	DEBUG_PRINT("				Checking LOS ... Robot[%d] to Target Robot[%d].\n", robot->getID(), target->getID());
	glm::vec3 curPosition = robot->getPosition().getGlmVec3() + glm::vec3(0.0f, 5.0f, 0.0f); // Height off-set for weapon
	float dt = 0.1f;
	glm::vec3 movementPerStep = direction * 5.0f;
	float age = 6.0f;
	int indexClosestTriangle;
	float distanceToTarget = glm::distance(curPosition, target->getPosition().getGlmVec3());
	do {
		if (distanceToTarget < ENEMY_RADIUS) {
			DEBUG_PRINT("				LOS ok! Enemy can be hit.");
			return true;
		}
		indexClosestTriangle = calculateClosestTerrainTriangle(curPosition, m_vTerrainTrianglesCenter);
		if (m_vTerrainTrianglesCenter->at(indexClosestTriangle).y > curPosition.y) {
			DEBUG_PRINT("				closestTriangle.y(%.1f) > curPosition.y(%.1f) !\n", m_vTerrainTrianglesCenter->at(indexClosestTriangle).y, curPosition.y);
			DEBUG_PRINT("				distanceToTarget(%.1f), age(%f)!\n", distanceToTarget, age);
			return false;
		}
		curPosition += movementPerStep;
		DEBUG_PRINT("				distanceToTarget(%f)!\n", distanceToTarget);
		distanceToTarget = glm::distance(curPosition, target->getPosition().getGlmVec3());
		age -= dt;
	} while ( distanceToTarget > 0 && age > 0);
	if (age < 0) {
		DEBUG_PRINT("				Checking died of old age(%f)!\n", age);
		return false;
	}
	DEBUG_PRINT("				distanceToTarget(%.1f) > age(%f)!\n", distanceToTarget, age);
	return true;
}

void cRobotFactory::caculateTrianglesCenter(cModel* terrainModel) {
	m_vPlaneTrianglesCenter = new std::vector<glm::vec3>();
	glm::vec3 triangleCenter;
	// We gonna iterate through each triangle of the model
	// And just save plane (no height difference) triangles
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

	// Initial reserve to containing all triangles centers part of the terrain
	m_vTerrainTrianglesCenter = new std::vector<glm::vec3>();
	m_vTerrainTrianglesCenter->reserve(terrainModel->numberOfTriangles);
	// We gonna iterate through each triangle of the model
	for (int i = 0; i < terrainModel->numberOfIndices; i) {
			// Center Triangle Terrain
			// = (CenterX, CenterZ)
			// CenterX = (V1x + V2x + V3x) / 3
			triangleCenter.x = (terrainModel->pVertices[terrainModel->pIndices[i + 0]].x +
								terrainModel->pVertices[terrainModel->pIndices[i + 1]].x +
								terrainModel->pVertices[terrainModel->pIndices[i + 2]].x) / 3;
			/*triangleCenter.y = (terrainModel->pVertices[terrainModel->pIndices[i + 0]].y +
								terrainModel->pVertices[terrainModel->pIndices[i + 1]].y +
								terrainModel->pVertices[terrainModel->pIndices[i + 2]].y) / 3;*/
			// For the height we gonna consider the first vertice Y
			triangleCenter.y = terrainModel->pVertices[terrainModel->pIndices[i]].y;
			triangleCenter.z = (terrainModel->pVertices[terrainModel->pIndices[i + 0]].z +
								terrainModel->pVertices[terrainModel->pIndices[i + 1]].z +
								terrainModel->pVertices[terrainModel->pIndices[i + 2]].z) / 3;

			m_vTerrainTrianglesCenter->push_back(triangleCenter);
		i += 3;
	}
}

int cRobotFactory::calculateClosestTerrainTriangle(glm::vec3 position, std::vector<glm::vec3>* triVector) {
	float minDistance = glm::distance(triVector->at(0), position);
	float curDistance;
	int faceIndex = 0; // Stores the index of the closest triangle
	for (int i = 0; i < triVector->size(); i++) {
		// gets the distance of the current face
		curDistance = glm::distance(triVector->at(i), position);
		// Checks if the current face is the closest face
		if (curDistance < minDistance) {
			minDistance = curDistance;
			faceIndex = i;
		}
	}
	return faceIndex;
}

// Utility function for a random range of two floats
float RandFloat(float min, float max) {
	//DEBUG_PRINT("RandFloat(%f, %f)\n", min, max);
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}