#pragma once
#include <vector>
#include <string>

#define PROJECT_SAVE_FILE "graphicLibrary.xml"

class cProjectManager {	
public:
	std::vector<std::string> m_vScenes;
	std::string				 m_selectedSecene;

	bool LoadSaveFile();
	bool SelectScene(std::string name);
	bool LoadScene(std::string name);
	bool SaveScene(std::string name);
};

