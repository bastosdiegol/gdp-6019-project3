#include <pugixml/pugixml.hpp>
#include <iostream>

#include "cProjectManager.h"
#include "cMeshObject.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cProjectManager::cProjectManager() {
	DEBUG_PRINT("cProjectManager::cProjectManager()\n");
	if (!LoadSaveFile()) {
		std::cout << "Project Save File named: #" << PROJECT_SAVE_FILE << " could not be loaded. Ending the application." << std::endl;
		exit(-1);
	}
	m_VAOManager = new cVAOManager();
	m_lightManager = new cLightManager();
	isNewScene = false;
	m_selectedScene = nullptr;
	m_selectedMesh = nullptr;
}

cProjectManager::~cProjectManager() {
	DEBUG_PRINT("cProjectManager::~cProjectManager()\n");
	std::map<std::string, cScene*>::iterator itScenes;
	for (itScenes = this->m_mScenes.begin();
		 itScenes != this->m_mScenes.end();
		 itScenes++)
		delete itScenes->second;
	delete m_VAOManager;
	delete m_lightManager;
}

bool cProjectManager::LoadScene(std::string name) {
	DEBUG_PRINT("cProjectManager::LoadScene(%s)\n", name.c_str());
	// I don't know if I really want to unload the scenes
	//this->UnloadScene();
	
	// Unselect previous selected mesh and light
	this->m_selectedMesh = nullptr;
	this->m_selectedLight = nullptr;

	// Create a document object
	pugi::xml_document graphicsLibrary;
	// Load the XML file into the object
	pugi::xml_parse_result result = graphicsLibrary.load_file(PROJECT_SAVE_FILE);
	if (!result) {
		std::cout << "ProjectManager error: Failed to load file named #" << PROJECT_SAVE_FILE << std::endl;
		return false;
	}
	DEBUG_PRINT("Successfully loaded file named #%s\n", PROJECT_SAVE_FILE);

	// Gets all nodes of sound inside the soundlibrary
	pugi::xml_node scenes = graphicsLibrary.child("graphicsLibrary");

	DEBUG_PRINT("<graphicsLibrary>\n");
	// Iterates through each graphicsLibrary node
	for (pugi::xml_node sceneNode = scenes.child("scene");
		sceneNode;
		sceneNode = sceneNode.next_sibling("scene")) {
		// Checks if current sceneNode is the selected Node
		if (std::strcmp(sceneNode.attribute("title").value(), name.c_str()) == 0) {

			// Tries to find if the scene was already loaded
			std::map<std::string, cScene*>::iterator itScene = m_mScenes.find(name);
			if (itScene != m_mScenes.end()) {
				// Checks if the scene was already loaded
				if (itScene->second != nullptr) {
					this->m_selectedScene = itScene->second;
					DEBUG_PRINT("Scene loaded previously ... Skipping new loading...\n");
					// Sets new Scene bool
					isNewScene = true;
					return true; // Scene previously loaded FOUND
				}
			}

			// Creates a pointer to the new Scene
			cScene* newScene = new cScene(name);
			// Sets Scene Camera Eye
			newScene->m_cameraEye	 = glm::vec3(sceneNode.attribute("camEyeX").as_float(),
												 sceneNode.attribute("camEyeY").as_float(),
												 sceneNode.attribute("camEyeZ").as_float());
			// Sets Scene Camera Target
			newScene->m_cameraTarget = glm::vec3(sceneNode.attribute("camTarX").as_float(),
												 sceneNode.attribute("camTarY").as_float(),
												 sceneNode.attribute("camTarZ").as_float());
			// Sets the new scene as selected one
			this->m_selectedScene = newScene;

			DEBUG_PRINT("<scene title=\"%s\">\n", name.c_str());
			// Iterates through each Model node inside the selected Scene Node
			for (pugi::xml_node modelNode = sceneNode.child("model");
				modelNode;
				modelNode = modelNode.next_sibling("model")) {
				DEBUG_PRINT("	<model path=\"%s\" title=\"%s\" \\> \n", modelNode.attribute("path").value()
																	   , modelNode.attribute("title").value());
				// Checks if parameters provided by XML are safe
				if ((std::strcmp(modelNode.attribute("path").value(), "") == 0
					|| std::strcmp(modelNode.attribute("title").value(), "") == 0)) {
					continue;
				}
				cModel* newModel;
				// Checks if Model was already loaded by another scene
				newModel = m_VAOManager->findModel(modelNode.attribute("title").value());
				if (newModel == nullptr) {
					// Creates a new model and add its Meshes to the MeshObject List of the Scene
					newModel = m_VAOManager->PrepareNewModel(modelNode.attribute("title").value(), modelNode.attribute("path").value());
					m_VAOManager->LoadModelIntoVAO(newModel);
				} else {
					DEBUG_PRINT("Model previously loaded found ... Skipping the creation a new model ...\n");
				}
				// Iterates through each mesh using the same model
				for (pugi::xml_node meshNode = modelNode.child("mesh");
					meshNode;
					meshNode = meshNode.next_sibling("mesh")) {
					DEBUG_PRINT("		<mesh id=\"%s\" \\> \n", meshNode.attribute("id").value());
					cMeshObject* newMeshObj = new cMeshObject(newModel);
					newMeshObj->m_meshName = meshNode.attribute("id").value();
					// Now lets load the rest of info about the mesh
					// Reads Position
					pugi::xml_node meshInfo = meshNode.child("position");
					newMeshObj->m_position = glm::vec3(meshInfo.attribute("x").as_float(),
													   meshInfo.attribute("y").as_float(),
													   meshInfo.attribute("z").as_float());
					meshInfo = meshInfo.next_sibling();
					// Reads Rotation
					newMeshObj->m_rotation = glm::vec3(meshInfo.attribute("x").as_float(),
													   meshInfo.attribute("y").as_float(),
													   meshInfo.attribute("z").as_float());
					meshInfo = meshInfo.next_sibling();
					// Reads UseRGB?
					newMeshObj->m_bUse_RGBA_colour = meshInfo.attribute("value").as_bool();
					meshInfo = meshInfo.next_sibling();
					// Reads Colour
					newMeshObj->m_RGBA_colour = glm::vec4(meshInfo.attribute("r").as_float(),
														  meshInfo.attribute("g").as_float(),
														  meshInfo.attribute("b").as_float(),
														  1);
					meshInfo = meshInfo.next_sibling();
					// Reads Scale
					newMeshObj->m_scale = meshInfo.attribute("value").as_float();
					meshInfo = meshInfo.next_sibling();
					// Reads isWireframe
					newMeshObj->m_isWireframe = meshInfo.attribute("value").as_bool();
					meshInfo = meshInfo.next_sibling();
					// Reads doNotLight
					newMeshObj->m_bDoNotLight = meshInfo.attribute("value").as_bool();
					meshInfo = meshInfo.next_sibling();
					// Reads isVisible
					newMeshObj->m_bIsVisible = meshInfo.attribute("value").as_bool();

					// Adds the newly created Mesh to the Scene Map of Meshes
					newScene->m_mMeshes.try_emplace(newMeshObj->m_meshName, newMeshObj);
				}
			}
			// Iterates through each Light Node
			for (pugi::xml_node lightNode = sceneNode.child("light");
				lightNode;
				lightNode = lightNode.next_sibling("light")) {
				// Creates new Instance of the Light
				cLight* newLight = new cLight();
				// Gets Light Friendly Name / ID
				newLight->m_friendlyName = lightNode.attribute("id").value();
				// Reads light info and store them
				pugi::xml_node lightInfo = lightNode.child("position");
				// Reads Position
				newLight->m_position.x = lightInfo.attribute("x").as_float();
				newLight->m_position.y = lightInfo.attribute("y").as_float();
				newLight->m_position.z = lightInfo.attribute("z").as_float();
				lightInfo = lightInfo.next_sibling();
				// Reads Diffuse
				newLight->m_diffuse.x = lightInfo.attribute("x").as_float();
				newLight->m_diffuse.y = lightInfo.attribute("y").as_float();
				newLight->m_diffuse.z = lightInfo.attribute("z").as_float();
				lightInfo = lightInfo.next_sibling();
				// Reads Specullar
				newLight->m_specular.x = lightInfo.attribute("r").as_float();
				newLight->m_specular.y = lightInfo.attribute("g").as_float();
				newLight->m_specular.z = lightInfo.attribute("b").as_float();
				newLight->m_specular.w = lightInfo.attribute("w").as_float();
				lightInfo = lightInfo.next_sibling();
				// Reads Attenuation
				newLight->m_attenuation.x = lightInfo.attribute("x").as_float();
				newLight->m_attenuation.y = lightInfo.attribute("y").as_float();
				newLight->m_attenuation.z = lightInfo.attribute("z").as_float();
				newLight->m_attenuation.w = lightInfo.attribute("w").as_float();
				lightInfo = lightInfo.next_sibling();
				// Reads Direction
				newLight->m_direction.x = lightInfo.attribute("x").as_float();
				newLight->m_direction.y = lightInfo.attribute("y").as_float();
				newLight->m_direction.z = lightInfo.attribute("z").as_float();
				newLight->m_direction.w = lightInfo.attribute("w").as_float();
				lightInfo = lightInfo.next_sibling();
				// Reads Param1
				newLight->m_param1.x = lightInfo.attribute("x").as_float();
				newLight->m_param1.y = lightInfo.attribute("y").as_float();
				newLight->m_param1.z = lightInfo.attribute("z").as_float();
				newLight->m_param1.w = lightInfo.attribute("w").as_float();
				lightInfo = lightInfo.next_sibling();
				// Reads Param2
				newLight->m_param2.x = lightInfo.attribute("x").as_float();
				newLight->m_param2.y = lightInfo.attribute("y").as_float();
				newLight->m_param2.z = lightInfo.attribute("z").as_float();
				newLight->m_param2.w = lightInfo.attribute("w").as_float();

				// Adds the newly created Light to the Scene Map of Lights
				newScene->m_mLights.try_emplace(newLight->m_friendlyName, newLight);
			}
			// Load UniformLocations for new Scene and set them on each light that's going to be used
			if(newScene->m_mLights.size() > 0)
				m_lightManager->LoadLightUniformLocations(m_VAOManager->m_shaderID, &newScene->m_mLights);
			// Adds the scene to the Map of scenes
			itScene = m_mScenes.find(name);
			if (itScene != m_mScenes.end()) {
				// Checks if the scene was already loaded
				itScene->second = newScene;
			} else {
				this->m_mScenes.try_emplace(name, newScene);
			}
		}
	}
	// Sets new Scene bool
	isNewScene = true;
	return true;
}

void cProjectManager::UnloadScene() {
	DEBUG_PRINT("cProjectManager::UnloadScene()\n");
	// Cheks if theres no selected scene
	if (this->m_selectedScene == nullptr)
		return;

	std::map<std::string, cMeshObject*>::iterator itMeshes;
	for (itMeshes = this->m_selectedScene->m_mMeshes.begin();
		 itMeshes != this->m_selectedScene->m_mMeshes.end();
		 itMeshes++) {
		itMeshes->second->~cMeshObject();
		delete itMeshes->second;
	}

	this->m_selectedMesh = nullptr;
}

bool cProjectManager::SaveSelectedScene() {
	// Checks if there's a selected scene
	if (m_selectedScene == nullptr) {
		return false;
	}
	DEBUG_PRINT("cProjectManager::SaveSelectedScene()\n");
	// Create a document object
	pugi::xml_document graphicsLibrary;
	// Load the XML file into the object
	pugi::xml_parse_result result = graphicsLibrary.load_file(PROJECT_SAVE_FILE);
	if (!result) {
		std::cout << "ProjectManager error: Failed to load file named #" << PROJECT_SAVE_FILE << std::endl;
		return false;
	}
	DEBUG_PRINT("Successfully loaded file named #%s\n", PROJECT_SAVE_FILE);

	// Gets all nodes of sound inside the soundlibrary
	pugi::xml_node scenes = graphicsLibrary.child("graphicsLibrary");
	// Iterates through each graphicsLibrary node to find Selected Scene
	for (pugi::xml_node sceneNode = scenes.child("scene");
						sceneNode;
						sceneNode = sceneNode.next_sibling("scene")) {
		// Checks if current sceneNode is the selected Node
		if (std::strcmp(sceneNode.attribute("title").value(), m_selectedScene->m_name.c_str()) == 0) {
			// Found It - Sets camera attributes
			sceneNode.attribute("camEyeX").set_value(m_selectedScene->m_cameraEye.x);
			sceneNode.attribute("camEyeY").set_value(m_selectedScene->m_cameraEye.y);
			sceneNode.attribute("camEyeZ").set_value(m_selectedScene->m_cameraEye.z);
			sceneNode.attribute("camTarX").set_value(m_selectedScene->m_cameraTarget.x);
			sceneNode.attribute("camTarY").set_value(m_selectedScene->m_cameraTarget.y);
			sceneNode.attribute("camTarZ").set_value(m_selectedScene->m_cameraTarget.z);
			// Now Sets the Mesh attributes
			pugi::xml_node modelNode = sceneNode.child("model");
			// Iterates through each Model of the scene
			for (pugi::xml_node modelNode = sceneNode.child("model");
								modelNode;
								modelNode = modelNode.next_sibling("model")) {
				// Iterates through each Mesh of this Model
				for (pugi::xml_node meshNode = modelNode.child("mesh");
									meshNode;
									meshNode = meshNode.next_sibling("mesh")) {
					std::map<std::string, cMeshObject*>::iterator itMesh = m_selectedScene->m_mMeshes.find(meshNode.attribute("id").as_string());
					// Checks if found the Selected Scene Obj
					if (itMesh == m_selectedScene->m_mMeshes.end()) {
						DEBUG_PRINT("Tried to find a Mesh to save and got nullptr. Mesh %s not saved.", meshNode.attribute("id").as_string());
						continue;
					}
					// Gets first Mesh Child Node - Position
					pugi::xml_node meshInfoNode = *meshNode.children().begin();
					// Sets Position
					meshInfoNode.attribute("x").set_value(itMesh->second->m_position.x);
					meshInfoNode.attribute("y").set_value(itMesh->second->m_position.y);
					meshInfoNode.attribute("z").set_value(itMesh->second->m_position.z);
					meshInfoNode = meshInfoNode.next_sibling();
					// Sets Rotation
					meshInfoNode.attribute("x").set_value(itMesh->second->m_rotation.x);
					meshInfoNode.attribute("y").set_value(itMesh->second->m_rotation.y);
					meshInfoNode.attribute("z").set_value(itMesh->second->m_rotation.z);
					meshInfoNode = meshInfoNode.next_sibling();
					// Sets Use Color
					meshInfoNode.attribute("value").set_value(itMesh->second->m_bUse_RGBA_colour);
					meshInfoNode = meshInfoNode.next_sibling();
					// Sets Color
					meshInfoNode.attribute("r").set_value(itMesh->second->m_RGBA_colour.r);
					meshInfoNode.attribute("g").set_value(itMesh->second->m_RGBA_colour.g);
					meshInfoNode.attribute("b").set_value(itMesh->second->m_RGBA_colour.b);
					meshInfoNode.attribute("a").set_value(itMesh->second->m_RGBA_colour.a);
					meshInfoNode = meshInfoNode.next_sibling();
					// Sets Scale
					meshInfoNode.attribute("value").set_value(itMesh->second->m_scale);
					meshInfoNode = meshInfoNode.next_sibling();
					// Sets Wireframe
					meshInfoNode.attribute("value").set_value(itMesh->second->m_isWireframe);
					meshInfoNode = meshInfoNode.next_sibling();
					// Sets DoNotLight
					meshInfoNode.attribute("value").set_value(itMesh->second->m_bDoNotLight);
					meshInfoNode = meshInfoNode.next_sibling();
					// Sets Visible
					meshInfoNode.attribute("value").set_value(itMesh->second->m_bIsVisible);
				}
			}
		}
	}
	graphicsLibrary.save_file(PROJECT_SAVE_FILE);
	return true;
}

void cProjectManager::SetShaderID(GLuint shaderID) {
	this->m_VAOManager->m_shaderID = shaderID;
}

bool cProjectManager::LoadSaveFile() {
	DEBUG_PRINT("cProjectManager::LoadSaveFile()\n");
	// Create a document object
	pugi::xml_document graphicsLibrary;
	// Load the XML file into the object
	pugi::xml_parse_result result = graphicsLibrary.load_file(PROJECT_SAVE_FILE);
	if (!result) {
		std::cout << "ProjectManager error: Failed to load file named #" << PROJECT_SAVE_FILE << std::endl;
		return false;
	}
	DEBUG_PRINT("Successfully loaded file named #%s\n", PROJECT_SAVE_FILE);

	// Gets all nodes of sound inside the soundlibrary
	pugi::xml_node scenes = graphicsLibrary.child("graphicsLibrary");
	//cScene* scene;

	DEBUG_PRINT("<graphicsLibrary>\n");
	// Iterates through each node
	for (pugi::xml_node sceneNode = scenes.child("scene"); 
						sceneNode; 
						sceneNode = sceneNode.next_sibling("scene")) {
		DEBUG_PRINT("	<scene> Title: %s\n",sceneNode.attribute("title").value());
		// Adds the unloaded scene to the map
		m_mScenes.try_emplace(sceneNode.attribute("title").value(), nullptr);
	}

	return true;
}
