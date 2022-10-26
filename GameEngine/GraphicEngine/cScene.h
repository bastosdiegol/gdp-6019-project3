#pragma once
#include <string>
#include <vector>

class cScene {
protected:
	std::string				 m_name;
public:
	std::vector<std::string> m_vModels;

	cScene(std::string name);
	// TODO: Clean the Vector of MeshOjects
	~cScene();
};

