#pragma once
#include "cProjectManager.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

extern cProjectManager* g_ProjectManager;
extern GLFWwindow* window;
extern glm::vec3* g_cameraEye;
extern glm::vec3* g_cameraTarget;

void finalExamStartingUp();
void finalExamNewGame();
void finalExamRunning();
void finalExamShutdown();
bool moveTowards(glm::vec3& position, glm::vec3 destination);
bool turnRight(cMeshObject* a, int loopCount);
void flickerLights();

// Beholders pointers
cMeshObject* patrols[3] = {nullptr, nullptr, nullptr};
int closestTargets[3] = { -1, -1, -1 };
int beholderCurRoom[3] = { 4, 3, 5 };
// All coordinates that the beholders will be pathing
glm::vec3 destinations[3][4] = { {glm::vec3(-93.0f, 1.6f, -44.0f), glm::vec3(-84.0f, 1.6f, -44.0f), glm::vec3(-84.0f, 1.6f, -48.0f), glm::vec3(-93.0f, 1.6f, -48.0f)},
								 {glm::vec3(-58.0f, 1.6f, -54.5f), glm::vec3(-44.0f, 1.6f, -54.5f), glm::vec3(-44.0f, 1.6f, -62.0f), glm::vec3(-58.0f, 1.6f, -62.0f)},
								 {glm::vec3(-53.0f, 1.6f, -44.0f), glm::vec3(-44.0f, 1.6f, -44.0f), glm::vec3(-44.0f, 1.6f, -48.0f), glm::vec3(-53.0f, 1.6f, -48.0f)} };
// Rooms entry and exit points
glm::vec3 room1to2 = glm::vec3(-74.2f, 1.6f, -58.7f);
glm::vec3 room1to4 = glm::vec3(-63.5f, 1.6f, -58.7f);
glm::vec3 room2to1 = glm::vec3(-78.5f, 1.6f, -58.7f);
glm::vec3 room2to3 = glm::vec3(-88.7f, 1.6f, -53.5f);
glm::vec3 room3to2 = glm::vec3(-88.7f, 1.6f, -49.0f);
glm::vec3 room4to1 = glm::vec3(-59.0f, 1.6f, -58.7f);
glm::vec3 room4to5 = glm::vec3(-48.7f, 1.6f, -53.5f);
glm::vec3 room5to4 = glm::vec3(-48.7f, 1.6f, -49.0f);
// Loops index to syncronize where the beholders are looking towards
int loopPatrols[3] = {1, 0, 2};
int beholderIndex = -1;
std::vector<cMeshObject*> v_brazierFlameObjects;
std::vector<cLight*> v_brazierLights;
unsigned int frame = 0;
bool targetsSet = false;

void FinalExamGameLoop() {
	switch (g_ProjectManager->m_GameLoopState) {
	case GameState::STARTING_UP:
		finalExamStartingUp();
		break;
	case GameState::NEW_GAME:
		finalExamNewGame();
		break;
	case GameState::RUNNING:
		finalExamRunning();
		break;
	case GameState::SHUTING_DOWN:
		finalExamShutdown();
		break;
	}
}

void finalExamStartingUp() {
	patrols[0] = g_ProjectManager->m_selectedScene->m_mMeshes.find("Patrol Beholder1")->second;
	patrols[1] = g_ProjectManager->m_selectedScene->m_mMeshes.find("Patrol Beholder2")->second;
	patrols[2] = g_ProjectManager->m_selectedScene->m_mMeshes.find("Patrol Beholder3")->second;

	// Sets initial position and angle of each patrol beholder
	patrols[0]->m_position = destinations[0][loopPatrols[0]];
	patrols[1]->m_position = destinations[1][loopPatrols[1]];
	patrols[2]->m_position = destinations[2][loopPatrols[2]];
	patrols[0]->m_rotation.y = 6.2832;
	patrols[1]->m_rotation.y = 4.7124f;
	patrols[2]->m_rotation.y = 7.854f;

	glm::vec4 metallic;
	cMeshObject* torch = nullptr;
	for (int i = 1; i <= 5; i++) {
		torch = g_ProjectManager->m_selectedScene->m_mMeshes.find(std::string("ZZ.Torch"+std::to_string(i)))->second;
		//torch->m_specular_colour_and_power = metallic;
	}

	v_brazierFlameObjects.push_back(g_ProjectManager->m_selectedScene->m_mMeshes.find("Z.Flame1")->second);
	v_brazierLights.push_back(g_ProjectManager->m_selectedScene->m_mLights.find("BrazierLight01")->second);
	v_brazierFlameObjects.push_back(g_ProjectManager->m_selectedScene->m_mMeshes.find("Z.Flame2")->second);
	v_brazierLights.push_back(g_ProjectManager->m_selectedScene->m_mLights.find("BrazierLight02")->second);
	v_brazierFlameObjects.push_back(g_ProjectManager->m_selectedScene->m_mMeshes.find("Z.Flame3")->second);
	v_brazierLights.push_back(g_ProjectManager->m_selectedScene->m_mLights.find("BrazierLight03")->second);
	v_brazierFlameObjects.push_back(g_ProjectManager->m_selectedScene->m_mMeshes.find("Z.Flame4")->second);
	v_brazierLights.push_back(g_ProjectManager->m_selectedScene->m_mLights.find("BrazierLight04")->second);
	v_brazierFlameObjects.push_back(g_ProjectManager->m_selectedScene->m_mMeshes.find("Z.Flame5")->second);
	v_brazierLights.push_back(g_ProjectManager->m_selectedScene->m_mLights.find("BrazierLight05")->second);

	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void finalExamNewGame() {
	// Sets initial position and angle of each patrol beholder
	loopPatrols[0] = 1;
	loopPatrols[1] = 0;
	loopPatrols[2] = 2;
	patrols[0]->m_position = destinations[0][loopPatrols[0]];
	patrols[1]->m_position = destinations[1][loopPatrols[1]];
	patrols[2]->m_position = destinations[2][loopPatrols[2]];
	patrols[0]->m_rotation.y = 6.2832;
	patrols[1]->m_rotation.y = 4.7124f;
	patrols[2]->m_rotation.y = 7.854f;
	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void finalExamRunning() {
	float dist;
	// Camera Follow
	if (beholderIndex != -1) {
		if(glm::distance(*g_cameraEye, patrols[beholderIndex]->m_position) > 7)
			moveTowards(*g_cameraEye, patrols[beholderIndex]->m_position);
		if(g_cameraEye->y < 7.0f)
			g_cameraEye->y = 7.0f;
		*g_cameraTarget = patrols[beholderIndex]->m_position;
	}
	// Iterates each patrol
	for (int i = 0; i < 3; i++) {
		// Verifies which loop is it running
		switch (loopPatrols[i] % 4) {
		// First Move Towards the Destination
		// Then turn 90o before incrementing the loop state
		case 0:
			if (!moveTowards(patrols[i]->m_position, destinations[i][0]))
				if (!turnRight(patrols[i], loopPatrols[i]))
					loopPatrols[i]++;
			break;
		case 1:
			if (!moveTowards(patrols[i]->m_position, destinations[i][1]))
				if (!turnRight(patrols[i], loopPatrols[i]))
					loopPatrols[i]++;
			break;
		case 2:
			if (!moveTowards(patrols[i]->m_position, destinations[i][2]))
				if (!turnRight(patrols[i], loopPatrols[i]))
					loopPatrols[i]++;
			break;
		case 3:
			if (!moveTowards(patrols[i]->m_position, destinations[i][3]))
				if (!turnRight(patrols[i], loopPatrols[i]))
					loopPatrols[i]++;
			break;
		}
	}
	if (frame == UINT_MAX)
		frame = 0;
	if(frame % 30 == 0)
		flickerLights();

	frame++;
}

void finalExamShutdown() {
	glfwSetWindowShouldClose(window, true);
}

bool moveTowards(glm::vec3& position, glm::vec3 finalDestination) {
	// Checks if beholder arrived at the spot
	if (glm::distance(position, finalDestination) < 0.2f) {
		return false;
	} else {
		// If not, gets the direction and keep moving
		glm::vec3 direction = finalDestination - position;
		direction = glm::normalize(direction);
		direction *= 0.05;
		position += direction;
		return true;
	}
}

bool turnRight(cMeshObject* a, int loopCount) {
	// Here we gonna check if each beholder is facing the proper angle
	// before moving to a new destination 
	switch (loopCount % 4) {
	case 0: 
		// Checks if the angle is corret
		if(a->m_rotation.y < 4.7124f){
			// If not increments it and keep turning
			a->m_rotation.y += glm::radians(0.3f);
			return true;
		}
		break;
	case 1:
		if (a->m_rotation.y < 6.2832){
			a->m_rotation.y += glm::radians(0.3f);
			return true;
		}
		break;
	case 2:
		if (a->m_rotation.y < 7.854){
			a->m_rotation.y += glm::radians(0.3f);
			return true;
		}
		break;
	case 3:
		if (a->m_rotation.y < 9.4248){
			a->m_rotation.y += glm::radians(0.3f);
			return true;
		} else {
			// After a complete cycle, resets to initial facing angle
			// No changes visually
			a->m_rotation.y = 3.1416;
		}
		break;
	}
	return false;
}

void flickerLights() {
	for (int i = 0; i < v_brazierFlameObjects.size(); i++) {
		cMeshObject* theFlameObject = v_brazierFlameObjects[i];
		cLight* theLight = v_brazierLights[i];
		if (theLight->m_attenuation.x > 1.0f || theFlameObject->m_scale.x < 0.40f) {
			theLight->m_attenuation.x -= theLight->m_attenuation.x * 0.1f;
			theFlameObject->m_scale += theFlameObject->m_scale * 0.1f;
		}else if(theLight->m_attenuation.x < 0.1f || theFlameObject->m_scale.x > 0.75f) {
			theLight->m_attenuation.x += theLight->m_attenuation.x * 0.1f;
			theFlameObject->m_scale -= theFlameObject->m_scale * 0.1f;
		} else if (rand() % 2 == 0) {
			theLight->m_attenuation.x -= theLight->m_attenuation.x * 0.1f;
			theFlameObject->m_scale += theFlameObject->m_scale * 0.1f;
		} else {
			theLight->m_attenuation.x += theLight->m_attenuation.x * 0.1f;
			theFlameObject->m_scale -= theFlameObject->m_scale * 0.1f;
		}
	}
}

void nextBeholder() {
	if (beholderIndex == -1) {
		beholderIndex = 0;
	} else {
		beholderIndex++;
		if (beholderIndex == 3) {
			beholderIndex = 0;
		}
	}
}

void findClosestTargets() {
	float closestDistance = FLT_MAX;
	for (int i = 0; i < 3; i++) {
		int curTarget = -1;
		for (int j = 0; j < 3; j++) {
			if (i != j) {
				float newDist = glm::distance(patrols[i]->m_position, patrols[j]->m_position);
				if (newDist < closestDistance) {
					newDist = closestDistance;
					curTarget = j;
				}
			}
		}
		closestTargets[i] = curTarget;
	}
	return;
}

void beholdersWar() {
	if (targetsSet == false) {
		findClosestTargets();
		targetsSet = true;
	}
	for (int i = 0; i < 3; i++) {
		float curDist = glm::distance(patrols[i]->m_position, patrols[closestTargets[i]]->m_position);
		if (curDist < 0.5f) {
			//TODO: Kill both beholders
			//TODO: Stop the other beholder
		} else {
			float closestDistance = FLT_MAX;
		}
	}
}