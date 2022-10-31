#pragma once
#include <vector>
#include <string>

#include "cLight.h"

class cLightManager {
public:
	cLightManager();

	static const unsigned int NUMBER_OF_LIGHTS_IM_GONNA_USE = 3;

	std::vector<cLight> vecTheLights;

	void LoadLightUniformLocations(unsigned int shaderID);

	void CopyLightInformationToShader(unsigned int shaderID);

};

