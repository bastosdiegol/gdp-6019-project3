#include "cProjectManager.h"
#include <pugixml/pugixml.hpp>
#include <iostream>

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
}

bool cProjectManager::LoadScene(std::string name) {
	DEBUG_PRINT("cProjectManager::LoadScene(%s)\n", name.c_str());

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
				// Adds new model to the Model List
				// TODO: Calls VAO Manager to Create a new Model
				// TODO: Change it from String Vector to an Model Vector
				newScene->m_vModels.push_back(modelNode.attribute("title").value());
			}
		}
	}	

	return true;
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
