#include "cLight.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cLight::cLight() {
	DEBUG_PRINT("cLight::cLight()\n");
	this->position	= glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->diffuse	= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->specular	= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->atten		= glm::vec4(0.01f, 0.01f, 0.0f, 1.0f);
	this->direction = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	this->param1	= glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->param2	= glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

}

cLight::~cLight() {
	DEBUG_PRINT("cLight::~cLight()\n");
}

void cLight::setConstantAttenuation(float newConstAtten) {
	this->atten.x = newConstAtten;
	return;
}
void cLight::setLinearAttenuation(float newLinearAtten) {
	this->atten.y = newLinearAtten;
	return;
}

void cLight::setQuadraticAttenuation(float newQuadAtten) {
	this->atten.z = newQuadAtten;
	return;
}

void cLight::TurnOn(void) {
	DEBUG_PRINT("cLight::TurnOn()\n");
	this->param2.x = 1;
	return;
}

void cLight::TurnOff(void) {
	DEBUG_PRINT("cLight::TurnOff()\n");
	this->param2.x = 0;
	return;
}