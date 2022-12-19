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

#define MIN_FORCE -3
#define MAX_FORCE 3

extern cProjectManager* g_ProjectManager;
extern PhysicsSystem* g_PhysicsSystem;
extern GLFWwindow* window;
extern glm::vec3* g_cameraEye;
extern glm::vec3* g_cameraTarget;
extern double g_MouseStaticPosX;
extern double g_MouseStaticPosY;
extern Vector3 g_forwardVector;
extern Vector3 g_rightVector;

double m_PrevMouseX;
double m_PrevMouseY;
double m_CurrMouseX;
double m_CurrMouseY;
double m_MouseDownX;
double m_MouseDownY;

float m_HorizontalAngle;

void PhysicsProjThreeStartingUp();
void PhysicsProjThreeNewGame();
void PhysicsProjThreeRunning();
void PhysicsProjThreeShutdown();
float GenFloat(float min, float max);
void setStaticPhysObjectAABB(std::string name);

void PhysicsProjThreeGameLoop() {
	switch (g_ProjectManager->m_GameLoopState) {
	case GameState::STARTING_UP:
		PhysicsProjThreeStartingUp();
		break;
	case GameState::NEW_GAME:
		PhysicsProjThreeNewGame();
		break;
	case GameState::RUNNING:
		PhysicsProjThreeRunning();
		break;
	case GameState::SHUTING_DOWN:
		PhysicsProjThreeShutdown();
		break;
	}
}

void PhysicsProjThreeStartingUp() {
	cMeshObject*	mesh	= nullptr;
	iShape*			aabb	= nullptr;
	iShape*			ball	= nullptr;
	PhysicsObject*	physObj	= nullptr;
	std::string		meshName;

	// Adds all static meshes to the PhysicsSystem
	setStaticPhysObjectAABB("Plane");
	setStaticPhysObjectAABB("Cone");
	setStaticPhysObjectAABB("Cube");
	setStaticPhysObjectAABB("Cylinder");
	setStaticPhysObjectAABB("LargeBox");
	mesh = g_ProjectManager->m_selectedScene->m_mMeshes.find("Sphere")->second;
	ball = new Sphere(Point(7.5f, 2.5f, 25.3757f), 1.5f);
	physObj = g_PhysicsSystem->CreatePhysicsObject(mesh, ball);
	physObj->SetMass(-1.0f);
	// Now add all the walls
	for (int i = 1; i <= 4; i++) {
		setStaticPhysObjectAABB("Wall" + std::to_string(i));
	}

	// Adds all moving targets to the physics system
	for (int i = 1; i <= 5; i++) {
		mesh = g_ProjectManager->m_selectedScene->m_mMeshes.find("Ball"+std::to_string(i))->second;
		//mesh->m_position = glm::vec3(i*10,
		//							 2.0f,
		//							 GenFloat(1.0f, 63.0f));
		// Creates the shape for the targets
		ball = new Sphere(Point(0.0f, 0.0f, 0.0f), mesh->m_scale.x);
		// Adds the sphere to the Physics System
		physObj = g_PhysicsSystem->CreatePhysicsObject(mesh, ball);
		// Applies a random force to a random X and Z
		physObj->ApplyForce(Vector3(GenFloat(MIN_FORCE, MAX_FORCE),
									0.0f,
									GenFloat(MIN_FORCE, MAX_FORCE)));
	}
	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void PhysicsProjThreeNewGame() {
	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void PhysicsProjThreeRunning() {
	// Update Step
	g_PhysicsSystem->UpdateStep(0.1f);
	// Update all Meshs to the current step
	for (int i = 0; i < g_PhysicsSystem->m_PhysicsObjects.size(); i++) {
		// Checks if the target stopped moving (hit a wall)
		if (g_PhysicsSystem->m_PhysicsObjects[i]->GetVelocity().x == 0 && 
			g_PhysicsSystem->m_PhysicsObjects[i]->GetVelocity().z == 0) {
			// Applies a random force to a random X and Z
			g_PhysicsSystem->m_PhysicsObjects[i]->ApplyForce(Vector3(GenFloat(MIN_FORCE, MAX_FORCE),
																	 0.0f,
																	 GenFloat(MIN_FORCE, MAX_FORCE)));
		}
		g_PhysicsSystem->m_PhysicsObjects[i]->parentMesh->m_position = g_PhysicsSystem->m_PhysicsObjects[i]->GetPosition().GetGLM();
	}

	m_PrevMouseX = m_CurrMouseX;
	m_PrevMouseY = m_CurrMouseY;
	glfwGetCursorPos(window, &m_CurrMouseX, &m_CurrMouseY);
	DEBUG_PRINT("Cursor Pos: x y (%f %f) \n", m_PrevMouseX, m_PrevMouseY);
	double deltaMouseX = 0;
	double deltaMouseY = 0;

	//g_MouseStaticPosX = m_CurrMouseX;
	//g_MouseStaticPosY = m_CurrMouseY;
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//glfwSetCursorPos(window, g_MouseStaticPosX, g_MouseStaticPosY);

	deltaMouseX = m_CurrMouseX - m_PrevMouseX;
	deltaMouseY = m_CurrMouseY - m_PrevMouseY;

	const float rotateSpeed = 0.01f;
	m_HorizontalAngle -= deltaMouseX * rotateSpeed;

	//DEBUG_PRINT("Camera Target: x y z (%f %f %f) \n", g_cameraTarget->x, g_cameraTarget->y, g_cameraTarget->z);

	if (deltaMouseX != 0 || deltaMouseY != 0) {
		g_cameraTarget->x += sin(m_HorizontalAngle);
		g_cameraTarget->z += cos(m_HorizontalAngle);
		g_cameraTarget->y -= deltaMouseY * rotateSpeed;
	}
	g_forwardVector = Vector3(g_cameraTarget->x, 0.0f, g_cameraTarget->z);
	g_rightVector = Vector3(glm::cross(g_forwardVector.GetGLM(), glm::vec3(0, 1, 0)));
}

void PhysicsProjThreeShutdown() {
	glfwSetWindowShouldClose(window, true);
}

// Utility function for a random range of two floats
float GenFloat(float min, float max) {
	if (max == min)
		return 0.0f;

	int diff = (max - min) * 1000;
	return min + (rand() % diff) / 1000.0f;
}

void setStaticPhysObjectAABB(std::string name) {
	cMeshObject* pmesh;
	iShape* paabb = nullptr;
	PhysicsObject* pphysObj = nullptr;
	// Cone AABB
	pmesh = g_ProjectManager->m_selectedScene->m_mMeshes.find(name)->second;
	// Creates the AABB structure for the Terrain
	float min[3] = { pmesh->m_parentModel->min_x,
					 pmesh->m_parentModel->min_y,
					 pmesh->m_parentModel->min_z };
	float max[3] = { pmesh->m_parentModel->max_x,
					 pmesh->m_parentModel->max_y,
					 pmesh->m_parentModel->max_z };
	paabb = new AABB(min, max);
	// Adds the AABB to the Physics System
	pphysObj = g_PhysicsSystem->CreatePhysicsObject(pmesh, paabb);
	pphysObj->SetMass(-1.0f);
}