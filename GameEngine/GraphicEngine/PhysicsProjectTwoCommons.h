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
	// Grabs Terrain the Mesh
	std::map<std::string, cMeshObject*>::iterator itMesh
		= g_ProjectManager->m_selectedScene->m_mMeshes.find("Terrain");
	// Checks if it exist
	if (itMesh != g_ProjectManager->m_selectedScene->m_mMeshes.end()) {
		// Creates the AABB structure for the Terrain
		float min[3] = { itMesh->second->m_parentModel->min_x,
						 itMesh->second->m_parentModel->min_y,
						 itMesh->second->m_parentModel->min_z };
		float max[3] = { itMesh->second->m_parentModel->max_x,
						 itMesh->second->m_parentModel->max_y,
						 itMesh->second->m_parentModel->max_z };
		iShape* groundAABB = new AABB(min, max);
		// Adds the AABB to the Physics System
		PhysicsObject* physObj = g_PhysicsSystem->CreatePhysicsObject(itMesh->second, groundAABB);
		physObj->SetMass(-1.0f);
		// Creates the Hash from all triangles of the AABB for Collision
		collmanager.addModelToAABBEnviroment(g_PhysicsSystem, itMesh->second->m_parentModel, itMesh->second->m_position);

		// Grabs the Glider Mesh
		itMesh = g_ProjectManager->m_selectedScene->m_mMeshes.find("Glider");
		if (itMesh != g_ProjectManager->m_selectedScene->m_mMeshes.end()) {
			Vector3 pos = itMesh->second->m_position;
			// Creates the initial shape for the Plane
			iShape* ball = new Sphere(Point(0.0f, 0.0f, 0.0f), itMesh->second->m_scale);
			// Adds the plane shape to the physics system
			g_PhysicsSystem->CreatePhysicsObject(itMesh->second, ball);
		}
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
	// Update Step
	g_PhysicsSystem->UpdateStep(0.01f);
	// Update all Meshs to the current
	for (int i = 0; i < g_PhysicsSystem->m_PhysicsObjects.size(); i++) {
		g_PhysicsSystem->m_PhysicsObjects[i]->parentMesh->m_position = g_PhysicsSystem->m_PhysicsObjects[i]->GetPosition().GetGLM();
	}
}

void physicsProjectTwoShutdown() {
	return;
}