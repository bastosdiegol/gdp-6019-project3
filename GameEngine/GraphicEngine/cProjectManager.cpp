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
}

cProjectManager::~cProjectManager() {
	DEBUG_PRINT("cProjectManager::~cProjectManager()\n");
	delete m_selectedScene;
	delete m_VAOManager;
}

bool cProjectManager::LoadScene(std::string name) {
	DEBUG_PRINT("cProjectManager::LoadScene(%s)\n", name.c_str());
	this->m_selectedMesh = nullptr;
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
			// Creates a pointer to the new Scene
			cScene* newScene = new cScene(name);
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
				// Creates a new model and add its Meshes to the MeshObject List of the Scene
				cModel* newModel = m_VAOManager->PrepareNewModel(modelNode.attribute("title").value(), modelNode.attribute("path").value());
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
				// TODO: Model != Mesh (To remember)
				// TODO: Change it from String Vector to an MeshObject Vector
			}
		}
	}	

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
		m_vScenes.push_back(sceneNode.attribute("title").value());
	}

	return true;
}
