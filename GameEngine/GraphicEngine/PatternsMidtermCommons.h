#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <cRobotFactory.h>
#include <cWeaponAssembler.h>
#include <cParticleSystem.h>
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
#define DT 0.1f

extern cProjectManager* g_ProjectManager;

// Patterns MidTerm Global Variables

cRobotFactory*	  g_robotFactory	= cRobotFactory::GetInstance();
cWeaponAssembler* g_weaponAssembler = cWeaponAssembler::GetInstance();
cParticleSystem*  g_particleSystem	= new cParticleSystem(glm::vec3(0.0f), 10);

void patternsMidTermGameLoop() {
	// Checks if there's no instance of Terrain Triangles Center
	// And if the it's the game first loop
	if (g_robotFactory->m_vPlaneTrianglesCenter == nullptr) {
		cModel* terrain = g_ProjectManager->m_selectedScene->m_mMeshes.find("Terrain")->second->m_parentModel;
		g_robotFactory->caculateTrianglesCenter(terrain);
		g_robotFactory->setTerrain(terrain);

		iRobot* theRobot;
		iWeapon* theWeapon;
		for (int i = 0; i < TOTAL_NUM_OF_ROBOTS; i++) {
			theWeapon = g_weaponAssembler->BuildAWeapon();
			theRobot = g_robotFactory->BuildARobot(theWeapon);
			g_robotFactory->setNewRandomPosition(theRobot);
		}
		// Sets new Game Loop State
		g_ProjectManager->m_GameLoopState = GameState::RUNNING;
	} else if (g_ProjectManager->m_GameLoopState == GameState::STARTING_UP) {
		g_ProjectManager->m_GameLoopState = GameState::NEW_GAME;
	}
	// Checks if it's a new game request
	if (g_ProjectManager->m_GameLoopState == GameState::NEW_GAME) {
		iRobot* theRobot;
		for (int i = 0; i < TOTAL_NUM_OF_ROBOTS; i++) {
			theRobot = g_robotFactory->getRobot(i);
			g_robotFactory->setNewRandomPosition(theRobot);
		}
		// Sets new Game Loop State
		g_ProjectManager->m_GameLoopState = GameState::RUNNING;
	}
	// Checks if its the game main loop
	if (g_ProjectManager->m_GameLoopState == GameState::RUNNING) {
		iRobot* theRobot;
		std::map<std::string, cMeshObject*>::iterator itMeshes;
		itMeshes = g_ProjectManager->m_selectedScene->m_mMeshes.begin();
		// Iterates through all robots
		for (int i = 0; i < TOTAL_NUM_OF_ROBOTS; i++) {
			theRobot = g_robotFactory->getRobot(i);
			// Checks if the Robot has a currently active projectile flying around
			if (theRobot->getFiredProjectile() != nullptr) {
				// Checks if the Projectile is Alive
				// If it has hit the target or not
				if (theRobot->getFiredProjectile()->getAge() > 0) {
					// Checks if the projectile is arounnd the target radius
					if (glm::distance(theRobot->getFiredProjectile()->getPosition(),
						theRobot->getCurTarget()->getPosition().getGlmVec3()) < ENEMY_RADIUS) {
						// TODO: enemy takes damage
						
						// Kills the particle by set age negative
						theRobot->getFiredProjectile()->setAge(-1.0f);
						// Forgets this projectile
						theRobot->setProjectile(nullptr);
					}
				} else {
					// Forgets the projectile pointer since its dead
					theRobot->setProjectile(nullptr);
					// Enable the weapon by reseting the cooldown
					theRobot->getWeapon()->setCooldown();
				}
			} else {
				// Checks if the Weapon has Cooldown to Fire
				// If it has Cooldown Look up for an enemy then fire!
				// Else does nothing
				if (theRobot->getWeapon()->getCooldown() <= 0) {
					DEBUG_PRINT("Robot[%d] Position(%.1f, %.1f, %.1f) looking up for targets.\n", theRobot->getID(), theRobot->getPosition().x
						, theRobot->getPosition().y, theRobot->getPosition().z);
					iRobot* target = g_robotFactory->findNearestRobot(theRobot);
					if (target == nullptr) {
						DEBUG_PRINT("!!! Robot[%d] found no target.\n", theRobot->getID());
						// Forgets any previous target
						theRobot->setCurTarget(nullptr);
						// Changes the robot spawn point
						g_robotFactory->setNewRandomPosition(theRobot);
					} else {
						DEBUG_PRINT("!!! Robot[%d] found target Robot[%d]\n", theRobot->getID(), target->getID());
						// Saves currrent target
						theRobot->setCurTarget(target);
						// Fires toward the target
						cParticle* particle = g_particleSystem->AllocateParticle(theRobot->getPosition().getGlmVec3() + glm::vec3(0.0f, 5.0f, 0.0f),
							(target->getPosition() - theRobot->getPosition()).getGlmVec3(),
							glm::vec3(0.0f),	//acceleration
							3.0f,  //age
							1.0f,	//damping
							1.0f);	//mass
						theRobot->setProjectile(particle);
						//g_robotFactory->fire(theRobot, target);
					}
					// Time to Update the Mesh Position
					itMeshes->second->m_position = theRobot->getPosition().getGlmVec3();
					// Sets mesh color to current robot HP
					if (theRobot->getHealth() > 75.0f ) {
						itMeshes->second->m_RGBA_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
					} else if (theRobot->getHealth() > 50.0f) {
						itMeshes->second->m_RGBA_colour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
					} else if (theRobot->getHealth() > 25.0f) {
						itMeshes->second->m_RGBA_colour = glm::vec4(1.0f, 0.59f, 0.22f, 1.0f); // Orange
					} else if (theRobot->getHealth() > 0.0f) {
						itMeshes->second->m_RGBA_colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
					} else {
						itMeshes->second->m_RGBA_colour = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f); // Gray
					}
					itMeshes++; // Next Mesh
				}
			}
			// Finally We update the Robot
			// This is age the Cooldown allowing to robot to shoot again
			theRobot->Update(DT);
		}

		g_particleSystem->Integrate(DT);
	}
}