#pragma once
#include <string>
#include <vector>

#include "cScene.h"

#define PROJECT_SAVE_FILE "graphicLibrary.xml"

class cProjectManager {	
private:
	std::vector<std::string>	m_vScenes;
	cScene*						m_selectedScene;

	/// <summary>
	/// Loads the PROJECT_SAVE_FILE and stores all scenes available to be loaded
	/// </summary>
	/// <returns>TRUE if loading was successful</returns>
	bool LoadSaveFile();
public:
	/// <summary>
	/// Default Constructor - also Loads Save File
	/// </summary>
	cProjectManager();
	// TODO: Clean the Vector of Scenes
	~cProjectManager();

	/// <summary>
	/// Loads a scene contained on the Vector of Scenes
	/// Sets the new scene on m_selectedScene pointer
	/// </summary>
	/// <param name="name">Name of the scene to be loaded</param>
	/// <returns>True if loading was successful</returns>
	bool LoadScene(std::string name);
	bool SaveScene(std::string name);
};

