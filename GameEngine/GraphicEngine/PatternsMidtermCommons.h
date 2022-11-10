#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <cRobotFactory.h>
#include <cWeaponAssembler.h>
#include <cRobot.h>
#include "cProjectManager.h"
#include "cModel.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

#define TOTAL_NUM_OF_ROBOTS 10

extern cProjectManager* g_ProjectManager;

// Patterns MidTerm Global Variables

cRobotFactory* g_robotFactory = cRobotFactory::GetInstance();
cWeaponAssembler* g_weaponAssembler = cWeaponAssembler::GetInstance();

// Patterns MidTerm Functions
void adjustRobotHeight(iRobot* robot);
void patternsMidTermGameLoop();

void adjustRobotHeight(iRobot* robot) {
	for (int i = 0; i < g_robotFactory->m_vPlaneTrianglesCenter->size(); i++) {
	}
}

void patternsMidTermGameLoop() {
	// Checks if there's no instance of Terrain Triangles Center
	// And if the it's the game first loop
	if (g_robotFactory->m_vPlaneTrianglesCenter == nullptr) {
		cModel* terrain = g_ProjectManager->m_selectedScene->m_mMeshes.find("Terrain")->second->m_parentModel;
		g_robotFactory->caculateTrianglesCenter(terrain);
		g_robotFactory->setTerrain(terrain);

		iRobot* theRobot;
		iWeapon* theWeapon;
		int closestFaceIndex;
		for (int i = 0; i < TOTAL_NUM_OF_ROBOTS; i++) {
			theWeapon = g_weaponAssembler->BuildAWeapon();
			theRobot = g_robotFactory->BuildARobot(theWeapon);
			closestFaceIndex = g_robotFactory->calculateClosestTerrainTriangle(theRobot->getPosition().getGlmVec3(),
																			   g_robotFactory->m_vPlaneTrianglesCenter);
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
			closestFaceIndex = g_robotFactory->calculateClosestTerrainTriangle(theRobot->getPosition().getGlmVec3(),
																			   g_robotFactory->m_vPlaneTrianglesCenter);
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
			DEBUG_PRINT("Robot[%d] Position(%.1f, %.1f, %.1f) looking up for targets.\n", theRobot->getID(), theRobot->getPosition().x
				, theRobot->getPosition().y, theRobot->getPosition().z);
			std::map<int, int>::iterator it = g_robotFactory->m_mMapOfTargets.find(theRobot->getID());
			if (it != g_robotFactory->m_mMapOfTargets.end()) {
				if (it->second != -1) {
					// TODO: FIRE!
					continue;
				}
			}
			iRobot* target = g_robotFactory->findNearestRobot(theRobot);
			if (target == nullptr) {
				DEBUG_PRINT("!!! Robot[%d] found no target.\n", theRobot->getID());
				// TODO: choose another spawn location
			} else {
				DEBUG_PRINT("!!! Robot[%d] found target Robot[%d]\n", theRobot->getID(), target->getID());
				// TODO: FIRE!
			}
			itMeshes->second->m_position = theRobot->getPosition().getGlmVec3();
			itMeshes++;
		}
	}
}