#pragma once
#include <string>
#include <map>

#include "cMeshObject.h"

class cScene {
protected:
	std::string				 m_name;
public:
	std::map<std::string, cMeshObject*> m_vMeshes;

	cScene(std::string name);
	// TODO: Clean the Vector of MeshOjects
	~cScene();
};

