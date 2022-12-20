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
#define MIN_COORD 0
#define MAX_COORD 64

extern cProjectManager* g_ProjectManager;
extern PhysicsSystem* g_PhysicsSystem;
extern GLFWwindow* window;
extern glm::vec3* g_cameraEye;
extern glm::vec3* g_cameraTarget;
extern double g_MouseStaticPosX;
extern double g_MouseStaticPosY;
extern Vector3 g_forwardVector;
extern Vector3 g_rightVector;
extern bool g_isClicked;

double m_PrevMouseX;
double m_PrevMouseY;
double m_CurrMouseX;
double m_CurrMouseY;
double m_MouseDownX;
double m_MouseDownY;

float m_HorizontalAngle;

glm::vec3 g_destinations[5];
Vector3 g_forces[5];
float g_targetDT[5]{0,0,0,0,0};
const float MAX_VEL_ALLOWED = 0.5f;
const float MAX_TIME_ALLOWED = 3.0f;

void PhysicsProjThreeStartingUp();
void PhysicsProjThreeNewGame();
void PhysicsProjThreeRunning();
void PhysicsProjThreeShutdown();
float GenFloat(float min, float max);
void setStaticPhysObjectAABB(std::string name);
bool accelerateTowards(PhysicsObject* obj, const glm::vec3 finalDestination);
bool arrivedAt(PhysicsObject* obj, const glm::vec3 finalDestination);

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
		// Gets a random destination
		g_destinations[i - 1] = glm::vec3(GenFloat(MIN_COORD, MAX_COORD),
										  physObj->GetPosition().y,
										  GenFloat(MIN_COORD, MAX_COORD));
		// Applies a random force to a random X and Z
		accelerateTowards(physObj, g_destinations[i-1]);
		// Saves the current force
		g_forces[i - 1] = physObj->GetForce();
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
	int targetIndex = 0;
	for (int i = 0; i < g_PhysicsSystem->m_PhysicsObjects.size(); i++) {
		if (g_PhysicsSystem->m_PhysicsObjects[i]->parentMesh->m_parentModel->meshName == "TargetBall") {
			// Checks if the target arrived at their destination
			// OR stopped moving (hit a wall)
			// OR broke the time limit for current target
			if (arrivedAt(g_PhysicsSystem->m_PhysicsObjects[i], g_destinations[targetIndex]) ||
				(g_PhysicsSystem->m_PhysicsObjects[i]->GetVelocity().x == 0 &&
				 g_PhysicsSystem->m_PhysicsObjects[i]->GetVelocity().z == 0) ||
				g_targetDT[targetIndex] > MAX_TIME_ALLOWED) {
				
				// Gets a random destination
				g_destinations[targetIndex] = glm::vec3(GenFloat(MIN_COORD, MAX_COORD),
														g_PhysicsSystem->m_PhysicsObjects[i]->GetPosition().y,
														GenFloat(MIN_COORD, MAX_COORD));
				// Applies a random force to a new random X and Z
				accelerateTowards(g_PhysicsSystem->m_PhysicsObjects[i], 
								  g_destinations[targetIndex]);
				// Saves the current force
				g_forces[targetIndex] = g_PhysicsSystem->m_PhysicsObjects[i]->GetForce();
				g_targetDT[targetIndex] = 0.0f;
			} else {
				// Gets current velocity
				Vector3 curVelocity = g_PhysicsSystem->m_PhysicsObjects[i]->GetVelocity();
				bool velocityCheck = false;
				if (curVelocity.x >= MAX_VEL_ALLOWED) {
					curVelocity.x = MAX_VEL_ALLOWED;
					velocityCheck = true;
				}
				if (curVelocity.z >= MAX_VEL_ALLOWED) {
					curVelocity.z = MAX_VEL_ALLOWED;
					velocityCheck = true;
				}
				// Verifies if the Velocity reached its max allowed
				if (velocityCheck) {
					// Limits the velocity
					g_PhysicsSystem->m_PhysicsObjects[i]->SetVelocity(curVelocity);
					// Resets the acceleration
					g_PhysicsSystem->m_PhysicsObjects[i]->SetAcceleration(0.0f);
				} else {
					// Increments velocity
					g_PhysicsSystem->m_PhysicsObjects[i]->ApplyForce(g_forces[targetIndex]);
				}
			}
			g_targetDT[targetIndex] += 0.01f;
			targetIndex++;
		}		

		g_PhysicsSystem->m_PhysicsObjects[i]->parentMesh->m_position = g_PhysicsSystem->m_PhysicsObjects[i]->GetPosition().GetGLM();
	}

	m_PrevMouseX = m_CurrMouseX;
	m_PrevMouseY = m_CurrMouseY;
	glfwGetCursorPos(window, &m_CurrMouseX, &m_CurrMouseY);
	//DEBUG_PRINT("Cursor Pos: x y (%f %f) \n", m_PrevMouseX, m_PrevMouseY);
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

	

	if (deltaMouseX != 0 || deltaMouseY != 0) {
		g_cameraTarget->x += sin(m_HorizontalAngle);
		g_cameraTarget->z += cos(m_HorizontalAngle);
		g_cameraTarget->y -= deltaMouseY * rotateSpeed;
	}
	g_forwardVector = Vector3(g_cameraTarget->x, 0.0f, g_cameraTarget->z);
	g_rightVector = Vector3(glm::cross(g_forwardVector.GetGLM(), glm::vec3(0, 1, 0)));

	if (g_isClicked) {
		DEBUG_PRINT("LMB Clicked\n");
		int width = 1200;
		int height = 800;

		// 1. Cursor Position on the Screen Fixed Middle Screen
		glm::vec3 cursorPositionOnScreenSpace(
			600,				// X is fine from left to right
			400,	// Since Y is origin at the top, and positive as it goes down the screen
			// we need to fix it like this.
			1.f
		);

		// 2. Viewport: Window Information
		glm::vec4 viewport = glm::vec4(0, 0, width, height);


		// 3 Projection Matrix
		glm::mat4 projectionMatrix = glm::perspective(
			glm::radians(45.0f),			// Field of View
			(float)width / (float)height,	// Aspect Ratio
			0.1f,							// zNear plane
			1.0f							// zFar plane
		);

		glm::mat4 viewMatrix = glm::lookAt(
			*g_cameraEye,					// Position of the Camera
			*g_cameraEye + *g_cameraTarget,	// Target view point
			glm::vec3(0, 1, 0)				// Up direction
		);

		cursorPositionOnScreenSpace.x = width / 2;
		cursorPositionOnScreenSpace.y = height / 2;

		// Calculate our position in world space
		glm::vec3 pointInWorldSpace = glm::unProject(
			cursorPositionOnScreenSpace,
			viewMatrix,
			projectionMatrix,
			viewport
		);


		// Using the point in World space and the Camera Position
		// We can calculate a direction to use for a Ray

		// This debug info should tell us our ray is facing the wrong way.
		//CreateBall(m_CameraPosition, .2f);
		//CreateBall(pointInWorldSpace, .1f);


		// This should be the fix:
		// Make pointInWorldSpace a direction instead
		glm::vec3 direction = pointInWorldSpace - *g_cameraEye;



		Ray ray(*g_cameraEye, direction);


		PhysicsObject* hitObject = nullptr;

		DEBUG_PRINT("CameraPosition: %.2f, %.2f, %.2f\nDirection: %.2f, %.2f, %.2f\n",
			g_cameraEye->x, g_cameraEye->y, g_cameraEye->z,
			g_cameraTarget->x, g_cameraTarget->y, g_cameraTarget->z
		);



		//if (g_PhysicsSystem->RayCastFirstFound(ray, &hitObject)) {
		//	hitObject->ApplyForce(Vector3(0.0f, 2000.0f, 0.0f));
		//	hitObject->parentMesh->m_bIsVisible = false;
		//}

		if (g_PhysicsSystem->RayCastClosest(ray, &hitObject)) {
			hitObject->ApplyForce(Vector3(0.0f, 2000.0f, 0.0f));
		}
		g_isClicked = false;
	}
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

bool accelerateTowards(PhysicsObject* obj, const glm::vec3 finalDestination) {
	// Checks if obj arrived at the spot
	if (glm::distance(obj->GetPosition().GetGLM(), finalDestination) < 0.2f) {
		return false;
	} else {
		// If not, gets the direction and keep moving
		glm::vec3 direction = finalDestination - obj->GetPosition().GetGLM();
		direction = glm::normalize(direction);
		obj->ApplyForce(direction);
		return true;
	}
}

bool arrivedAt(PhysicsObject* obj, const glm::vec3 finalDestination) {
	if (glm::distance(obj->GetPosition().GetGLM(), finalDestination) < 0.2f) {
		return true;
	}
	return false;
}
