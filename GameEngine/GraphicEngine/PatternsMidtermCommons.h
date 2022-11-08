#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <cRobotFactory.h>
#include <cRobot.h>
#include "cProjectManager.h"
#include "cModel.h"

#define TOTAL_NUM_OF_ROBOTS 10

extern cProjectManager* g_ProjectManager;

// Patterns MidTerm Global Variables
std::vector<glm::vec3>* g_vTerrainTrianglesCenter;
cRobotFactory* g_robotFactory = cRobotFactory::GetInstance();

// Patterns MidTerm Functions
void adjustRobotHeight(iRobot* robot);
int calculateClosestTerrainTriangle(glm::vec3 position);
void caculateTerrainTrianglesCenter(cModel* terrainModel);
void patternsMidTermGameLoop();

void adjustRobotHeight(iRobot* robot) {
	for (int i = 0; i < g_vTerrainTrianglesCenter->size(); i++) {
	}
}

int calculateClosestTerrainTriangle(glm::vec3 position) {
	float minDistance = glm::distance(g_vTerrainTrianglesCenter->at(0), position);
	float curDistance;
	int faceIndex = 0; // Stores the index of the closest triangle
	for (int i = 0; i < g_vTerrainTrianglesCenter->size(); i++) {
		// gets the distance of the current face
		curDistance = glm::distance(g_vTerrainTrianglesCenter->at(i), position);
		// Checks if the current face is the closest face
		if (curDistance < minDistance) {
			minDistance = curDistance;
			faceIndex = i;
		}
	}
	return faceIndex;
}

void caculateTerrainTrianglesCenter(cModel* terrainModel) {
	g_vTerrainTrianglesCenter = new std::vector<glm::vec3>();
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

			g_vTerrainTrianglesCenter->push_back(triangleCenter);
		}
		i += 3;
	}
}

void patternsMidTermGameLoop() {
	// Checks if there's no instance of Terrain Triangles Center
	// And if the it's the game first loop
	if (g_vTerrainTrianglesCenter == nullptr) {
		caculateTerrainTrianglesCenter(g_ProjectManager->m_selectedScene->m_mMeshes.find("Terrain")->second->m_parentModel);

		iRobot* theRobot;
		int closestFaceIndex;
		for (int i = 0; i < TOTAL_NUM_OF_ROBOTS; i++) {
			theRobot = g_robotFactory->BuildARobot();
			closestFaceIndex = calculateClosestTerrainTriangle(static_cast<cRobot*>(theRobot)->m_position.getGlmVec3());
			static_cast<cRobot*>(theRobot)->m_position.x = g_vTerrainTrianglesCenter->at(closestFaceIndex).x;
			static_cast<cRobot*>(theRobot)->m_position.y = g_vTerrainTrianglesCenter->at(closestFaceIndex).y;
			static_cast<cRobot*>(theRobot)->m_position.z = g_vTerrainTrianglesCenter->at(closestFaceIndex).z;
		}
		// Sets new Game Loop State
		g_ProjectManager->m_GameLoopState = GameState::RUNNING;
	} else if (g_ProjectManager->m_GameLoopState == GameState::STARTING_UP) {
		g_ProjectManager->m_GameLoopState = GameState::NEW_GAME;
	}
	// Checks if it's a new game request
	if (g_ProjectManager->m_GameLoopState == GameState::NEW_GAME) {
		iRobot* theRobot;
		int closestFaceIndex;
		for (int i = 0; i < TOTAL_NUM_OF_ROBOTS; i++) {
			theRobot = g_robotFactory->getRobot(i);
			g_robotFactory->setNewRandomPosition(theRobot);
			closestFaceIndex = calculateClosestTerrainTriangle(static_cast<cRobot*>(theRobot)->m_position.getGlmVec3());
			static_cast<cRobot*>(theRobot)->m_position.x = g_vTerrainTrianglesCenter->at(closestFaceIndex).x;
			static_cast<cRobot*>(theRobot)->m_position.y = g_vTerrainTrianglesCenter->at(closestFaceIndex).y;
			static_cast<cRobot*>(theRobot)->m_position.z = g_vTerrainTrianglesCenter->at(closestFaceIndex).z;
		}
		// Sets new Game Loop State
		g_ProjectManager->m_GameLoopState = GameState::RUNNING;
	}
	// Checks if its the game main loop
	if (g_ProjectManager->m_GameLoopState == GameState::RUNNING) {
		iRobot* theRobot;
		// Time to Update the Mesh Position
		std::map<std::string, cMeshObject*>::iterator itMeshes;
		itMeshes = g_ProjectManager->m_selectedScene->m_mMeshes.begin();
		// Iterates through all meshes
		for (int i = 0; i < TOTAL_NUM_OF_ROBOTS; i++) {
			theRobot = g_robotFactory->getRobot(i);
			itMeshes->second->m_position.x = static_cast<cRobot*>(theRobot)->m_position.x;
			itMeshes->second->m_position.y = static_cast<cRobot*>(theRobot)->m_position.y;
			itMeshes->second->m_position.z = static_cast<cRobot*>(theRobot)->m_position.z;
			itMeshes++;
		}
	}
}