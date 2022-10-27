#include "cScene.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cScene::cScene(std::string name) {
	DEBUG_PRINT("cScene::cScene()\n");
	this->m_name = name;
}

cScene::~cScene() {
	DEBUG_PRINT("cScene::~cScene()\n");
}
