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

cRobotFactory* g_robotFactory = cRobotFactory::GetInstance();

// Patterns MidTerm Functions
void adjustRobotHeight(iRobot* robot);
int calculateClosestTerrainTriangle(glm::vec3 position);
void patternsMidTermGameLoop();

void adjustRobotHeight(iRobot* robot) {
	for (int i = 0; i < g_robotFactory->m_vPlaneTrianglesCenter->size(); i++) {
	}
}

int calculateClosestTerrainTriangle(glm::vec3 position) {
	float minDistance = glm::distance(g_robotFactory->m_vPlaneTrianglesCenter->at(0), position);
	float curDistance;
	int faceIndex = 0; // Stores the index of the closest triangle
	for (int i = 0; i < g_robotFactory->m_vPlaneTrianglesCenter->size(); i++) {
		// gets the distance of the current face
		curDistance = glm::distance(g_robotFactory->m_vPlaneTrianglesCenter->at(i), position);
		// Checks if the current face is the closest face
		if (curDistance < minDistance) {
			minDistance = curDistance;
			faceIndex = i;
		}
	}
	return faceIndex;
}

void patternsMidTermGameLoop() {
	// Checks if there's no instance of Terrain Triangles Center
	// And if the it's the game first loop
	if (g_robotFactory->m_vPlaneTrianglesCenter == nullptr) {
		cModel* terrain = g_ProjectManager->m_selectedScene->m_mMeshes.find("Terrain")->second->m_parentModel;
		g_robotFactory->caculatePlaneTrianglesCenter(terrain);
		g_robotFactory->setTerrain(terrain);

		iRobot* theRobot;
		int closestFaceIndex;
		for (int i = 0; i < TOTAL_NUM_OF_ROBOTS; i++) {
			theRobot = g_robotFactory->BuildARobot();
			closestFaceIndex = calculateClosestTerrainTriangle(theRobot->getPosition().getGlmVec3());
			glm::vec3 posFaceIndex = g_robotFactory->m_vPlaneTrianglesCenter->at(closestFaceIndex);
			theRobot->setPosition(posFaceIndex.x, posFaceIndex.y, posFaceIndex.z);
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
			closestFaceIndex = calculateClosestTerrainTriangle(theRobot->getPosition().getGlmVec3());
			glm::vec3 posFaceIndex = g_robotFactory->m_vPlaneTrianglesCenter->at(closestFaceIndex);
			theRobot->setPosition(posFaceIndex.x, posFaceIndex.y, posFaceIndex.z);
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
			itMeshes->second->m_position = theRobot->getPosition().getGlmVec3();
			itMeshes++;
		}
	}
}