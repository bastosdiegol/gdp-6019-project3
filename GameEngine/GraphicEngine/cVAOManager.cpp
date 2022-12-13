#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include "cVAOManager.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cVAOManager::cVAOManager(GLuint shaderID) {
	DEBUG_PRINT("cVAOManager::cVAOManager()\n");
	this->m_plyReader = new cPlyFileReader();
	this->m_shaderID = shaderID;
	// Loads the universal light model
	this->m_lightModel = LoadLightModel();
	LoadModelIntoVAO(this->m_lightModel);
}

cVAOManager::~cVAOManager() {
	DEBUG_PRINT("cVAOManager::~cVAOManager()\n");
	// Free Ply Reader Class
	delete m_plyReader;
	std::map<std::string, cModel*>::iterator itModel;
	// Free Each Model Class
	for (itModel = m_mapModels.begin(); itModel != m_mapModels.end(); itModel++)
		delete itModel->second;
	// Free Light Model
	delete m_lightModel;
}

cModel* cVAOManager::PrepareNewModel(std::string friendlyName, std::string filePath) {
	DEBUG_PRINT("cVAOManager::PrepareNewModel(%s, %s)\n", friendlyName.c_str(), filePath.c_str());
	// Checks if Model is already loaded
	std::map<std::string, cModel*>::iterator itModel = m_mapModels.find(friendlyName);
	if (itModel != m_mapModels.end()) {
		DEBUG_PRINT("Model already loaded ... Returning its class ...\n");
		return itModel->second;
	}
	cModel* newModel = new cModel();
	newModel->meshName = friendlyName;
	// Reads the Ply file
	// Checks if the reading was ok
	if (m_plyReader->loadMeshFromFile(filePath) == false) {
		return nullptr;
	}
	// Sets number of Vertices
	newModel->numberOfVertices = m_plyReader->m_numberOfVertices;
	// Creates the struct which will host all vertices
	newModel->pVertices = new sVertex[newModel->numberOfVertices];
	// Sets up initial min and max values
	newModel->min_x = newModel->max_x = m_plyReader->pTheModelArray[0].x;
	newModel->min_y = newModel->max_y = m_plyReader->pTheModelArray[0].y;
	newModel->min_z = newModel->max_z = m_plyReader->pTheModelArray[0].z;
	// Now copy the information from the PLY to the model class
	for (unsigned int index = 0; index != m_plyReader->m_numberOfVertices; index++) {

		newModel->pVertices[index].x = m_plyReader->pTheModelArray[index].x;
		newModel->pVertices[index].y = m_plyReader->pTheModelArray[index].y;
		newModel->pVertices[index].z = m_plyReader->pTheModelArray[index].z;

		if (m_plyReader->pTheModelArray[index].x < newModel->min_x) newModel->min_x = m_plyReader->pTheModelArray[index].x;
		if (m_plyReader->pTheModelArray[index].x > newModel->max_x) newModel->max_x = m_plyReader->pTheModelArray[index].x;
		if (m_plyReader->pTheModelArray[index].y < newModel->min_y) newModel->min_y = m_plyReader->pTheModelArray[index].y;
		if (m_plyReader->pTheModelArray[index].y > newModel->max_y) newModel->max_y = m_plyReader->pTheModelArray[index].y;
		if (m_plyReader->pTheModelArray[index].z < newModel->min_z) newModel->min_z = m_plyReader->pTheModelArray[index].z;
		if (m_plyReader->pTheModelArray[index].z > newModel->max_z) newModel->max_z = m_plyReader->pTheModelArray[index].z;

		newModel->pVertices[index].r = m_plyReader->pTheModelArray[index].red;
		newModel->pVertices[index].g = m_plyReader->pTheModelArray[index].green;
		newModel->pVertices[index].b = m_plyReader->pTheModelArray[index].blue;
		newModel->pVertices[index].a = m_plyReader->pTheModelArray[index].alpha;

		newModel->pVertices[index].nx = m_plyReader->pTheModelArray[index].nx;
		newModel->pVertices[index].ny = m_plyReader->pTheModelArray[index].ny;
		newModel->pVertices[index].nz = m_plyReader->pTheModelArray[index].nz;

		newModel->pVertices[index].u0 = m_plyReader->pTheModelArray[index].texture_u;
		newModel->pVertices[index].v0 = m_plyReader->pTheModelArray[index].texture_v;

	}
	// Sets Number of Triangles
	newModel->numberOfTriangles = m_plyReader->m_numberOfTriangles;
	// Final Number of Indices to be drawn
	newModel->numberOfIndices = newModel->numberOfTriangles * 3;
	// Creates the struct which will host all vertices of each triangle
	newModel->pIndices = new unsigned int[newModel->numberOfIndices];

	unsigned int vertex_element_index_index = 0;

	for (unsigned int triangleIndex = 0; triangleIndex < newModel->numberOfTriangles; triangleIndex++) {
		newModel->pIndices[vertex_element_index_index + 0] = m_plyReader->pTheModelTriangleArray[triangleIndex].vertexIndices[0];
		newModel->pIndices[vertex_element_index_index + 1] = m_plyReader->pTheModelTriangleArray[triangleIndex].vertexIndices[1];
		newModel->pIndices[vertex_element_index_index + 2] = m_plyReader->pTheModelTriangleArray[triangleIndex].vertexIndices[2];

		// Each +1 of the triangle index moves the "vertex element index" by 3
		// (3 vertices per triangle)
		vertex_element_index_index += 3;
	}

	if (m_plyReader->m_numberOfVertices > 0) {
		delete[] m_plyReader->pTheModelArray;
		m_plyReader->m_numberOfVertices = -1;
	}
	if (m_plyReader->m_numberOfTriangles > 0) {
		delete[] m_plyReader->pTheModelTriangleArray;
		m_plyReader->m_numberOfTriangles = -1;
	}

	this->m_mapModels.try_emplace(newModel->meshName, newModel);

	return newModel;
}

cModel* cVAOManager::LoadLightModel() {
	DEBUG_PRINT("cVAOManager::LoadLightModel()\n");
	cModel* newModel = new cModel();
	newModel->meshName = "Light";
	// Reads the Ply file
	// Checks if the reading was ok
	if (m_plyReader->loadMeshFromFile(LIGHT_MODEL_PATH) == false) {
		return nullptr;
	}
	// Sets number of Vertices
	newModel->numberOfVertices = m_plyReader->m_numberOfVertices;
	// Creates the struct which will host all vertices
	newModel->pVertices = new sVertex[newModel->numberOfVertices];
	// Now copy the information from the PLY to the model class
	for (unsigned int index = 0; index != m_plyReader->m_numberOfVertices; index++) {

		newModel->pVertices[index].x = m_plyReader->pTheModelArray[index].x;
		newModel->pVertices[index].y = m_plyReader->pTheModelArray[index].y;
		newModel->pVertices[index].z = m_plyReader->pTheModelArray[index].z;

		newModel->pVertices[index].r = m_plyReader->pTheModelArray[index].red;
		newModel->pVertices[index].g = m_plyReader->pTheModelArray[index].green;
		newModel->pVertices[index].b = m_plyReader->pTheModelArray[index].blue;

		newModel->pVertices[index].nx = m_plyReader->pTheModelArray[index].nx;
		newModel->pVertices[index].ny = m_plyReader->pTheModelArray[index].ny;
		newModel->pVertices[index].nz = m_plyReader->pTheModelArray[index].nz;

	}
	// Sets Number of Triangles
	newModel->numberOfTriangles = m_plyReader->m_numberOfTriangles;
	// Final Number of Indices to be drawn
	newModel->numberOfIndices = newModel->numberOfTriangles * 3;
	// Creates the struct which will host all vertices of each triangle
	newModel->pIndices = new unsigned int[newModel->numberOfIndices];

	unsigned int vertex_element_index_index = 0;

	for (unsigned int triangleIndex = 0; triangleIndex < newModel->numberOfTriangles; triangleIndex++) {
		newModel->pIndices[vertex_element_index_index + 0] = m_plyReader->pTheModelTriangleArray[triangleIndex].vertexIndices[0];
		newModel->pIndices[vertex_element_index_index + 1] = m_plyReader->pTheModelTriangleArray[triangleIndex].vertexIndices[1];
		newModel->pIndices[vertex_element_index_index + 2] = m_plyReader->pTheModelTriangleArray[triangleIndex].vertexIndices[2];

		// Each +1 of the triangle index moves the "vertex element index" by 3
		// (3 vertices per triangle)
		vertex_element_index_index += 3;
	}

	if (m_plyReader->m_numberOfVertices > 0) {
		delete[] m_plyReader->pTheModelArray;
		m_plyReader->m_numberOfVertices = -1;
	}
	if (m_plyReader->m_numberOfTriangles > 0) {
		delete[] m_plyReader->pTheModelTriangleArray;
		m_plyReader->m_numberOfTriangles = -1;
	}

	return newModel;
}

bool cVAOManager::LoadModelIntoVAO(cModel* modelObj) {
	DEBUG_PRINT("cVAOManager::LoadModelIntoVAO(%s)\n", modelObj->meshName.c_str());

	// Ask OpenGL for a new buffer ID...
	glGenVertexArrays(1, &(modelObj->VAO_ID));
	// Bind this buffer:
	glBindVertexArray(modelObj->VAO_ID);
	glGenBuffers(1, &(modelObj->VertexBufferID));
	glBindBuffer(GL_ARRAY_BUFFER, modelObj->VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(sVertex) * modelObj->numberOfVertices,
		(GLvoid*)modelObj->pVertices,
		GL_STATIC_DRAW);

	// Copy the index buffer into the video card, too
	// Create an index buffer.
	glGenBuffers(1, &(modelObj->IndexBufferID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelObj->IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
		sizeof(unsigned int) * modelObj->numberOfIndices,
		(GLvoid*)modelObj->pIndices,
		GL_STATIC_DRAW);

	// Set the vertex attributes.
	GLint vpos_location = glGetAttribLocation(m_shaderID, "vPosition");
	GLint vcol_location = glGetAttribLocation(m_shaderID, "vColour");
	GLint vNormal_location = glGetAttribLocation(m_shaderID, "vNormal");

	// Set the vertex attributes for this shader
	// Position
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 4,
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)offsetof(sVertex, x));

	// Color
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 4,
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)offsetof(sVertex, r));

	// Normals
	glEnableVertexAttribArray(vNormal_location);
	glVertexAttribPointer(vNormal_location, 4,
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)offsetof(sVertex, nx));

	//in vec4 vUVx2;			
	GLint vUVx2_location = glGetAttribLocation(m_shaderID, "vUVx2");
	glEnableVertexAttribArray(vUVx2_location);
	glVertexAttribPointer(vUVx2_location,
		4, GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex),						// Stride	(number of bytes)
		(void*)offsetof(sVertex, u0));		// Offset the member variable

	//in vec4 vTangent;			
	GLint vTangent_location = glGetAttribLocation(m_shaderID, "vTangent");
	glEnableVertexAttribArray(vTangent_location);
	glVertexAttribPointer(vTangent_location,
		4, GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex),						// Stride	(number of bytes)
		(void*)offsetof(sVertex, tx));		// Offset the member variable

	//in vec4 vBiNormal;		
	GLint vBiNormal_location = glGetAttribLocation(m_shaderID, "vBiNormal");
	glEnableVertexAttribArray(vBiNormal_location);
	glVertexAttribPointer(vBiNormal_location,
		4, GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex),						// Stride	(number of bytes)
		(void*)offsetof(sVertex, bx));		// Offset the member variable

	//in vec4 vBoneID;			
	GLint vBoneID_location = glGetAttribLocation(m_shaderID, "vBoneID");
	glEnableVertexAttribArray(vBoneID_location);
	glVertexAttribPointer(vBoneID_location,
		4, GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex),						// Stride	(number of bytes)
		(void*)offsetof(sVertex, vBoneID[0]));		// Offset the member variable

	//in vec4 vBoneWeight;		
	GLint vBoneWeight_location = glGetAttribLocation(m_shaderID, "vBoneWeight");
	glEnableVertexAttribArray(vBoneWeight_location);
	glVertexAttribPointer(vBoneWeight_location,
		4, GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex),						// Stride	(number of bytes)
		(void*)offsetof(sVertex, vBoneWeight[0]));		// Offset the member variable

	// Now that all the parts are set up, set the VAO to zero
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vNormal_location);
	glDisableVertexAttribArray(vUVx2_location);
	glDisableVertexAttribArray(vTangent_location);
	glDisableVertexAttribArray(vBiNormal_location);
	glDisableVertexAttribArray(vBoneID_location);
	glDisableVertexAttribArray(vBoneWeight_location);

	return true;
}

cModel* cVAOManager::findModel(std::string name) {
	std::map<std::string, cModel*>::iterator itModel = m_mapModels.find(name);
	if (itModel == m_mapModels.end())
		return nullptr;
	else
		return itModel->second;
}