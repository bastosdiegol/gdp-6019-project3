#pragma once
#include "cProjectManager.h"
#include <CollisionManager.h>
#include <PhysicsSystem.h>

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

extern cProjectManager* g_ProjectManager;
extern PhysicsSystem* g_PhysicsSystem;

void physicsProjectTwoStartingUp();
void physicsProjectTwoNewGame();
void physicsProjectTwoRunning();
void physicsProjectTwoShutdown();

void PhysicsProjectTwoGameLoop() {
	switch (g_ProjectManager->m_GameLoopState) {
	case GameState::STARTING_UP:
		physicsProjectTwoStartingUp();
		break;
	case GameState::NEW_GAME:
		physicsProjectTwoNewGame();
		break;
	case GameState::RUNNING:
		physicsProjectTwoRunning();
		break;
	case GameState::SHUTING_DOWN:
		physicsProjectTwoShutdown();
		break;
	}
}

void physicsProjectTwoStartingUp() {
	CollisionManager collmanager;
	// Grabs the Mesh
	std::map<std::string, cMeshObject*>::iterator itMesh
		= g_ProjectManager->m_selectedScene->m_mMeshes.find("Terrain");
	// Checks if it exist
	if (itMesh != g_ProjectManager->m_selectedScene->m_mMeshes.end()) {
		// Creates the AABB structure for the Terrain
		collmanager.addModelToAABBEnviroment(g_PhysicsSystem, itMesh->second->m_parentModel, itMesh->second->m_position);
	} else {
		DEBUG_PRINT("Terrain not found!\n");
		exit(-1);
	}
	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void physicsProjectTwoNewGame() {
	return;
}

void physicsProjectTwoRunning() {
	return;
}

void physicsProjectTwoShutdown() {
	return;
}