#pragma once
#include <FModManager.h>
#include "cProjectManager.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

#define SOUND_FILE "media_proj_2_sounds.xml"

extern cProjectManager* g_ProjectManager;
extern FModManager* g_FModManager;
extern GLFWwindow* window;

void mediaProjectTwoStartingUp();
void mediaProjectTwoNewGame();
void mediaProjectTwoRunning();
void mediaProjectTwoShutdown();
bool attachSoundToMesh(std::string meshName, std::string soundName);

void MediaProjectTwoGameLoop() {
	switch (g_ProjectManager->m_GameLoopState) {
	case GameState::STARTING_UP:
		mediaProjectTwoStartingUp();
		break;
	case GameState::NEW_GAME:
		mediaProjectTwoNewGame();
		break;
	case GameState::RUNNING:
		mediaProjectTwoRunning();
		break;
	case GameState::SHUTING_DOWN:
		mediaProjectTwoShutdown();
		break;
	}
}

void mediaProjectTwoStartingUp() {
	// Load all sounds from the XML File
	g_FModManager->loadSoundsFromFile(SOUND_FILE);
	// Attaching sounds to meshes
	attachSoundToMesh("Dragon Rib", "Darkness");
	attachSoundToMesh("Brazier 01", "Fire");
	attachSoundToMesh("Brazier 02", "Fire");
	attachSoundToMesh("Barrel",	"Drips");
	attachSoundToMesh("Window Wall", "Wind");

	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void mediaProjectTwoNewGame() {
	g_ProjectManager->m_selectedScene->m_cameraEye = glm::vec3(0.0f, 10.0f, -46.0f);
	g_ProjectManager->m_selectedScene->m_cameraTarget = glm::vec3(0.0f, 5.0f, 0.0f);
	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void mediaProjectTwoRunning(){
	g_FModManager->tick(g_ProjectManager->m_selectedScene->m_cameraEye);
}

void mediaProjectTwoShutdown(){
	glfwSetWindowShouldClose(window, true);
}


bool attachSoundToMesh(std::string meshName, std::string soundName) {
	// Sets the sound for the dragon bones mesh
	cMeshObject* meshObj;
	// Grabs the Dragon Rib mesh
	std::map<std::string, cMeshObject*>::iterator itMesh = g_ProjectManager->m_selectedScene->m_mMeshes.find(meshName);
	if (itMesh != g_ProjectManager->m_selectedScene->m_mMeshes.end()) {
		meshObj = itMesh->second;
	} else {
		DEBUG_PRINT("Mesh %s not found!", meshName.c_str());
		return false;
	}
	// Grabs the Sound
	Sound* pSound = g_FModManager->getSound(soundName);
	// Play the Sound
	g_FModManager->playSound(soundName, meshObj->m_position, 0.01f);
	// Attach the Sound Channel to the Mesh
	meshObj->attached_sound = pSound->m_channel;

	return true;
}