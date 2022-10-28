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
}

bool cProjectManager::LoadScene(std::string name) {
	DEBUG_PRINT("cProjectManager::LoadScene(%s)\n", name.c_str());

	// I don't know if I really want to unload the scenes
	//this->UnloadScene();

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
			// Iterates through each node inside the selected Scene Node
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
					pugi::xml_node meshInfo = meshNode.child("position");
					newMeshObj->m_position = glm::vec3(meshInfo.attribute("x").as_float(),
													   meshInfo.attribute("y").as_float(),
													   meshInfo.attribute("z").as_float());
					meshInfo = meshInfo.next_sibling();
					newMeshObj->m_rotation = glm::vec3(meshInfo.attribute("x").as_float(),
													   meshInfo.attribute("y").as_float(),
													   meshInfo.attribute("z").as_float());
					meshInfo = meshInfo.next_sibling();
					newMeshObj->m_bUse_RGBA_colour = meshInfo.attribute("value").as_bool();
					meshInfo = meshInfo.next_sibling();
					newMeshObj->m_RGBA_colour = glm::vec4(meshInfo.attribute("r").as_float(),
														  meshInfo.attribute("g").as_float(),
														  meshInfo.attribute("b").as_float(),
														  1);
					meshInfo = meshInfo.next_sibling();
					newMeshObj->m_scale = meshInfo.attribute("value").as_bool();
					meshInfo = meshInfo.next_sibling();
					newMeshObj->m_isWireframe = meshInfo.attribute("value").as_bool();
					meshInfo = meshInfo.next_sibling();
					newMeshObj->m_bIsVisible = meshInfo.attribute("value").as_bool();

					newScene->m_vMeshes.try_emplace(newMeshObj->m_meshName, newMeshObj);
				}
			}
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
	return true;
}

void cProjectManager::UnloadScene() {
	DEBUG_PRINT("cProjectManager::UnloadScene()\n");
	// Cheks if theres no selected scene
	if (this->m_selectedScene == nullptr)
		return;

	std::map<std::string, cMeshObject*>::iterator itMeshes;
	for (itMeshes = this->m_selectedScene->m_vMeshes.begin();
		 itMeshes != this->m_selectedScene->m_vMeshes.end();
		 itMeshes++) {
		itMeshes->second->~cMeshObject();
		delete itMeshes->second;
	}

	this->m_selectedMesh = nullptr;
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
