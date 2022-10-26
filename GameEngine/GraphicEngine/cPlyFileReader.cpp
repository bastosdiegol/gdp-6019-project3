#include "cPlyFileReader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cPlyFileReader::cPlyFileReader() {
	DEBUG_PRINT("PlyFileReader::PlyFileReader()\n");
	m_numberOfVertices = -1;
	m_numberOfTriangles = -1;
	pTheModelArray = nullptr;
	pTheModelTriangleArray = nullptr;
}

cPlyFileReader::~cPlyFileReader() {
	DEBUG_PRINT("PlyFileReader::~PlyFileReader()\n");
	delete[] pTheModelArray;
	delete[] pTheModelTriangleArray;
}

void cPlyFileReader::loadMeshFromFile(std::string file_location) {
	DEBUG_PRINT("PlyFileReader::loadMeshFromFile(%s)\n", file_location.c_str());
	// Loads the file
	std::ifstream theFile(file_location);
	// Checks if the file is open
	if (!theFile.is_open()) {
		std::cout << "File " << file_location << " could not be opened!" << std::endl;
		return;
	}

	// Iteration to Find the Total of Vertices
	std::string theNextToken;
	do {
		theFile >> theNextToken;
	} while (theNextToken != "vertex");
	theFile >> theNextToken;
	// Sets the total of Vertices
	m_numberOfVertices = std::stoul(theNextToken);
	// Create new instance of Vertex array for the model
	pTheModelArray = new sVertex_XYZ_N_RGBA_UV[m_numberOfVertices];

	// Iteration to Find the Total of Triangles
	do {
		theFile >> theNextToken;
	} while (theNextToken != "face");
	theFile >> theNextToken;
	// Sets the total of Faces
	m_numberOfTriangles = std::stoul(theNextToken);
	// Create a new instance of Triangles Array for the model
	pTheModelTriangleArray = new sTrianglePLY[m_numberOfTriangles];

	// Runs the file till reach "end_header" and be ready to read vertices
	do {
		theFile >> theNextToken;
	} while (theNextToken != "end_header");

	DEBUG_PRINT("Loading %s vertices ", file_location.c_str());
	// Iteration to read and store all vertices
	for (unsigned int i = 0; i < m_numberOfVertices; i++) {
		// Stores each property value per Vertice
		theFile >> pTheModelArray[i].x;
		theFile >> pTheModelArray[i].y;
		theFile >> pTheModelArray[i].z;

		theFile >> pTheModelArray[i].nx;
		theFile >> pTheModelArray[i].ny;
		theFile >> pTheModelArray[i].nz;

		theFile >> pTheModelArray[i].red;
		theFile >> pTheModelArray[i].green;
		theFile >> pTheModelArray[i].blue;
		theFile >> pTheModelArray[i].alpha;

		//theFile >> pTheModelArray[i].texture_u;
		//theFile >> pTheModelArray[i].texture_v;
	}
	DEBUG_PRINT("done!\n");


	DEBUG_PRINT("Loading %s triangle faces ", file_location.c_str());
	// Iteration to read and store all vertices
	for (unsigned int i = 0; i < m_numberOfTriangles; i++) {
		// First value of triangles is its ID that has no use
		unsigned int discard = 0;
		theFile >> discard;
		// Stores all vertices for each triangle
		theFile >> pTheModelTriangleArray[i].vertexIndices[0];
		theFile >> pTheModelTriangleArray[i].vertexIndices[1];
		theFile >> pTheModelTriangleArray[i].vertexIndices[2];
	}
	DEBUG_PRINT("done!\n");

	theFile.close();
}
