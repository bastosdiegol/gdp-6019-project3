#pragma once
#include <string>
#include <map>

#include "cModel.h"
#include "cPlyFileReader.h"

class cVAOManager {
public:
	GLuint m_shaderID;
	// Default Constructor
	cVAOManager();
	// Destructor
	~cVAOManager();
	/// <summary>
	/// After Reading from a Ply File
	/// This Methods organize that Data to be ready to send
	/// To the GPU (Shader)
	/// </summary>
	/// <returns></returns>
	bool PrepareNewModel(std::string friendlyName,
						 std::string filePath);
	/// <summary>
	/// Binds now the Model Data to the Shader
	/// </summary>
	/// <param name="fileName">Friendly Name</param>
	/// <param name="drawInfo">Model Vertices n Triangles</param>
	/// <param name="shaderProgramID">Shader ID</param>
	/// <returns></returns>
	bool LoadModelIntoVAO(cModel* drawInfo);

	bool FindDrawInfoByModelName(std::string filename,
		cModel* drawInfo);

	//std::string getLastError(bool bAndClear = true);

private:
	cPlyFileReader* m_plyReader;
	std::map<std::string, cModel*> m_mapModels;
};

