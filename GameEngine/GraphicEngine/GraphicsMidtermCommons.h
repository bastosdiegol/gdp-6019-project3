#pragma once
#include <glm/vec3.hpp>

#include "cProjectManager.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

#define normalFactor 1000.0f

extern cProjectManager* g_ProjectManager;

// Initial Positions
glm::vec3 g_skyShipInitialPos;
glm::vec4 g_skyShipLight1InitialPos;
glm::vec4 g_skyShipLight2InitialPos;
// Current Positions
glm::vec3 g_skyShipPos;
glm::vec4 g_skyShipLight1Pos;
glm::vec4 g_skyShipLight2Pos;
// Destination
glm::vec3 g_skyShipDestination;
glm::vec4 g_skyShipLight1Destination;
glm::vec4 g_skyShipLight2Destination;
// Movement Steps
glm::vec3 g_skyShipMovementStep;
glm::vec4 g_skyShipLight1MovementStep;
glm::vec4 g_skyShipLight2MovementStep;

void graphicsMidTermNightGameLoop() {

	if (g_ProjectManager->m_GameLoopState == GameState::STARTING_UP) {
		// Grabs the Mesh
		std::map<std::string, cMeshObject*>::iterator itMesh
			= g_ProjectManager->m_selectedScene->m_mMeshes.find("Sky Ship");
		// Checks if it exist
		if (itMesh != g_ProjectManager->m_selectedScene->m_mMeshes.end()) {
			// Stores Initial Position
			g_skyShipInitialPos = itMesh->second->m_position;
			// Sets current position
			g_skyShipPos = g_skyShipInitialPos;
			// Gets the Destination
			g_skyShipDestination = g_skyShipInitialPos + glm::vec3(0.0f, 0.0f, 100.0f);
			// Gets the Direction to Destination
			g_skyShipMovementStep = g_skyShipDestination - g_skyShipInitialPos;
			// Normalize the Direction
			g_skyShipMovementStep = g_skyShipMovementStep / normalFactor;
		} else {
			DEBUG_PRINT("Sky Ship not found!\n");
			exit(-1);
		}

		// Grabs the Mesh
		std::map<std::string, cLight*>::iterator itLight
			= g_ProjectManager->m_selectedScene->m_mLights.find("Light Ship Lantern1");
		// Checks if it exist
		if (itLight != g_ProjectManager->m_selectedScene->m_mLights.end()) {
			// Stores Initial Position
			g_skyShipLight1InitialPos = itLight->second->m_position;
			// Sets current position
			g_skyShipLight1Pos = g_skyShipLight1InitialPos;
			// Gets the Destination
			g_skyShipLight1Destination = g_skyShipLight1InitialPos + glm::vec4(0.0f, 0.0f, 100.0f, 0.0f);
			// Gets the Direction to Destination
			g_skyShipLight1MovementStep = g_skyShipLight1Destination - g_skyShipLight1InitialPos;
			// Normalize the Direction
			g_skyShipLight1MovementStep = g_skyShipLight1MovementStep / normalFactor;
		} else {
			DEBUG_PRINT("Sky Ship Light 1 not found!\n");
			exit(-1);
		}

		// Grabs the Mesh
		itLight = g_ProjectManager->m_selectedScene->m_mLights.find("Light Ship Lantern2");
		// Checks if it exist
		if (itLight != g_ProjectManager->m_selectedScene->m_mLights.end()) {
			// Stores Initial Position
			g_skyShipLight2InitialPos = itLight->second->m_position;
			// Sets current position
			g_skyShipLight2Pos = g_skyShipLight2InitialPos;
			// Gets the Destination
			g_skyShipLight2Destination = g_skyShipLight2InitialPos + glm::vec4(0.0f, 0.0f, 100.0f, 0.0f);
			// Gets the Direction to Destination
			g_skyShipLight2MovementStep = g_skyShipLight2Destination - g_skyShipLight2InitialPos;
			// Normalize the Direction
			g_skyShipLight2MovementStep = g_skyShipLight2MovementStep / normalFactor;
		} else {
			DEBUG_PRINT("Sky Ship Light 2 not found!\n");
			exit(-1);
		}
		// Game Initialized, Sets new Game State
		g_ProjectManager->m_GameLoopState = GameState::RUNNING;
	}

	if (g_ProjectManager->m_GameLoopState == GameState::RUNNING) {
		// Checks if the Sky Ship position arrived at the end
		if (glm::distance(g_skyShipPos, g_skyShipDestination) > 0.5) {
			// Increments the Sky Ship Position
			g_skyShipPos += g_skyShipMovementStep;
			// Grabs the Mesh
			std::map<std::string, cMeshObject*>::iterator itMesh
				= g_ProjectManager->m_selectedScene->m_mMeshes.find("Sky Ship");
			// Update Mesh Position
			itMesh->second->m_position = g_skyShipPos;
		} else {
			// Arrived at the end - Restart
			g_ProjectManager->m_GameLoopState = GameState::NEW_GAME;
		}
		// Checks if the Sky Ship Light1 position arrived at the end
		if (glm::distance(g_skyShipLight1Pos, g_skyShipLight1Destination) > 0.5) {
			// Increments the Sky Ship Light 1 Position
			g_skyShipLight1Pos += g_skyShipLight1MovementStep;
			// Grabs the Mesh
			std::map<std::string, cLight*>::iterator itLight
				= g_ProjectManager->m_selectedScene->m_mLights.find("Light Ship Lantern1");
			// Update Mesh Position
			itLight->second->m_position += g_skyShipLight1Pos;
		}
		// Checks if the Sky Ship Light2 position arrived at the end
		if (glm::distance(g_skyShipLight2Pos, g_skyShipLight2Destination) > 0.5) {
			// Increments the Sky Ship Light 2 Position
			g_skyShipLight2Pos += g_skyShipLight2MovementStep;
			// Grabs the Mesh
			std::map<std::string, cLight*>::iterator itLight
				= g_ProjectManager->m_selectedScene->m_mLights.find("Light Ship Lantern2");
			// Update Mesh Position
			itLight->second->m_position = g_skyShipLight2Pos;
		}
	}

	if (g_ProjectManager->m_GameLoopState == GameState::NEW_GAME) {
		// Resets the Sky Ship
		std::map<std::string, cMeshObject*>::iterator itMesh
			= g_ProjectManager->m_selectedScene->m_mMeshes.find("Sky Ship");
		itMesh->second->m_position = g_skyShipInitialPos;
		g_skyShipPos = g_skyShipInitialPos;
		// Resets the Sky Ship Light 1
		std::map<std::string, cLight*>::iterator itLight
			= g_ProjectManager->m_selectedScene->m_mLights.find("Light Ship Lantern1");
		itLight->second->m_position = g_skyShipLight1InitialPos;
		g_skyShipLight1Pos = g_skyShipLight1InitialPos;
		// Resets the Sky Ship Light 2
		itLight = g_ProjectManager->m_selectedScene->m_mLights.find("Light Ship Lantern2");
		itLight->second->m_position = g_skyShipLight2InitialPos;
		g_skyShipLight2Pos = g_skyShipLight2InitialPos;

		g_ProjectManager->m_GameLoopState = GameState::RUNNING;
	}

}