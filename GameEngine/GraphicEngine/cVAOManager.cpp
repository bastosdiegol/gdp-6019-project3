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

cVAOManager::cVAOManager() {
	DEBUG_PRINT("cVAOManager::cVAOManager()\n");
	this->m_plyReader = new cPlyFileReader();
}

cVAOManager::~cVAOManager() {
	DEBUG_PRINT("cVAOManager::~cVAOManager()\n");
	// Free Ply Reader Class
	delete m_plyReader;
	std::map<std::string, cModel*>::iterator itModel;
	// Free Each Model Class
	for (itModel = m_mapModels.begin(); itModel != m_mapModels.end(); itModel++)
		delete itModel->second;
}

bool cVAOManager::PrepareNewModel(std::string friendlyName, std::string filePath) {
	DEBUG_PRINT("cVAOManager::PrepareNewModel(%s, %s)\n", friendlyName.c_str(), filePath.c_str());
	cModel* newModel = new cModel();
	newModel->meshName = friendlyName;
	// Reads the Ply file
	m_plyReader->loadMeshFromFile(filePath);
	// Creates the struct which will host all vertices
	newModel->pVertices = new sVertex[newModel->numberOfVertices];
	// Now copy the information from the PLY to the model class
	for (unsigned int index = 0; index != newModel->numberOfVertices; index++) {

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
	// Final Number of Indices to be drawn
	newModel->numberOfIndices = newModel->numberOfTriangles * 3;
	// Creates the struct which will host all vertices of each triangle
	newModel->pIndices = new unsigned int[newModel->numberOfIndices];

	unsigned int vertex_element_index_index = 0;

	for (unsigned int triangleIndex = 0; triangleIndex != newModel->numberOfTriangles; triangleIndex++) {
		newModel->pIndices[vertex_element_index_index + 0] = m_plyReader->pTheModelTriangleArray[triangleIndex].vertexIndices[0];
		newModel->pIndices[vertex_element_index_index + 1] = m_plyReader->pTheModelTriangleArray[triangleIndex].vertexIndices[1];
		newModel->pIndices[vertex_element_index_index + 2] = m_plyReader->pTheModelTriangleArray[triangleIndex].vertexIndices[2];

		// Each +1 of the triangle index moves the "vertex element index" by 3
		// (3 vertices per triangle)
		vertex_element_index_index += 3;
	}

	delete[] m_plyReader->pTheModelArray;
	delete[] m_plyReader->pTheModelTriangleArray;

	this->m_mapModels.try_emplace(newModel->meshName, newModel);

	return true;
}

bool cVAOManager::LoadModelIntoVAO(
	cModel* drawInfo,
	unsigned int shaderProgramID) {
	DEBUG_PRINT("cVAOManager::LoadModelIntoVAO(%s, %d)\n", drawInfo->meshName.c_str(), shaderProgramID);

	// Ask OpenGL for a new buffer ID...
	glGenVertexArrays(1, &(drawInfo->VAO_ID));
	// Bind this buffer:
	glBindVertexArray(drawInfo->VAO_ID);
	glGenBuffers(1, &(drawInfo->VertexBufferID));
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo->VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(sVertex) * drawInfo->numberOfVertices,
		(GLvoid*)drawInfo->pVertices,
		GL_STATIC_DRAW);

	// Copy the index buffer into the video card, too
	// Create an index buffer.
	glGenBuffers(1, &(drawInfo->IndexBufferID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo->IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
		sizeof(unsigned int) * drawInfo->numberOfIndices,
		(GLvoid*)drawInfo->pIndices,
		GL_STATIC_DRAW);

	// Set the vertex attributes.
	GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPos");
	GLint vcol_location = glGetAttribLocation(shaderProgramID, "vCol");
	GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");

	// Set the vertex attributes for this shader
	// Position
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3,
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)offsetof(sVertex, x));
	// Color
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3,
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)offsetof(sVertex, r));
	// Normals
	glEnableVertexAttribArray(vNormal_location);
	glVertexAttribPointer(vNormal_location, 3,
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)offsetof(sVertex, nx));

	// Now that all the parts are set up, set the VAO to zero
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vNormal_location);

	// Store the draw information into the map
	this->m_mapModels[drawInfo->meshName] = drawInfo;

	return true;
}

bool cVAOManager::FindDrawInfoByModelName(
		std::string filename,
		cModel* drawInfo) {
	DEBUG_PRINT("cVAOManager::FindDrawInfoByModelName(%s)\n", filename.c_str());

	std::map<std::string, cModel*>::iterator
		itDrawInfo = this->m_mapModels.find( filename );

	// Checks if it's the end of the map
	if ( itDrawInfo == this->m_mapModels.end() ) {
		return false;
	}
	// Found it
	drawInfo = itDrawInfo->second;
	return true;
}