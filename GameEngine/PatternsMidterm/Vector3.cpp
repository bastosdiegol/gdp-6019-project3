#include "Vector3.h"
#include <iostream>

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

// Default Constructor
// Accepts no parameters
// Returns nothing
Vector3::Vector3()
	: x(0.f)
	, y(0.f)
	, z(0.f) {
	DEBUG_PRINT("Vector3::Vector3()\n");
}

// Parameterized Constructor
// Accepts a value to use for all axis
// Returns nothing
Vector3::Vector3(float value)
	: x(value)
	, y(value)
	, z(value) {
	DEBUG_PRINT("Vector3::Vector3(%f)\n", value);
}

// Parameterized Constructor
// Accepts a value for each axis
// Returns nothing
Vector3::Vector3(float x, float y, float z)
	: x(x)
	, y(y)
	, z(z) {
	DEBUG_PRINT("Vector3::Vector3(%f, %f, %f)\n", x, y , z);
}

// Destructor
// Accepts no parameters
// Returns nothing
Vector3::~Vector3() {
	DEBUG_PRINT("Vector3::~Vector3()\n");
}

// Copy Constructor
// Accepts a reference to another Vector3
// returns nothing
Vector3::Vector3(const Vector3& rhs)
//: x(other.x)		This is another option instead
//, y(other.y)		of assigning all of the values
//, z(other.z)		within the constructor.
{
	DEBUG_PRINT("Vector3::Vector3(rhs(%f, %f, %f))\n", rhs.x, rhs.y, rhs.z);
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
}

// Assignment Operator Overload
// Accepts a reference to another Vector3
// Returns a refernce to the new Vector3
Vector3& Vector3::operator=(const Vector3& rhs) {
	DEBUG_PRINT("Vector3::operator=(rhs(%f, %f, %f))\n", rhs.x, rhs.y, rhs.z);
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	return *this;
}

// Add Operator Overload
// Accepts a reference to the right hand side of the add operation
// Returns a copy to the new Vector3
Vector3 Vector3::operator+(const Vector3& rhs) {
	DEBUG_PRINT("Vector3::operator+(rhs(%f, %f, %f))\n", rhs.x, rhs.y, rhs.z);
	//Instead of creating a variable to store and return values
	//Directly returning the operation optimize 2 lines of execution (operator+destruction)
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

// Substract Operator Overload
// Accepts a reference to the right hand side of the substract operation
// Returns a copy to the new Vector3
Vector3 Vector3::operator-(const Vector3& rhs) {
	DEBUG_PRINT("Vector3::operator-(rhs(%f, %f, %f))\n", rhs.x, rhs.y, rhs.z);
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

// Multiply Operator Overload
// Accepts a scalar for the multiply operation
// Returns a copy to the new Vector3
Vector3 Vector3::operator*(const float& scalar) {
	DEBUG_PRINT("Vector3::operator*(%f)\n", scalar);
	return Vector3(x * scalar, y * scalar, z * scalar);
}

// Division Operator Overload
// Accepts a scalar for the division operation
// Returns a copy to the new Vector3
Vector3 Vector3::operator/(const float& scalar) {
	DEBUG_PRINT("Vector3::operator/(%f)\n", scalar);
	return Vector3(x / scalar, y / scalar, z / scalar);
}

// Add Assignment Operator Overload
// Accepts a reference to the right hand side of the add operation
// no return, values are changed internally
void Vector3::operator+=(const Vector3& rhs) {
	DEBUG_PRINT("Vector3::operator+=(rhs(%f, %f, %f))\n", rhs.x, rhs.y, rhs.z);
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
}

// Subtract Assignment Operator Overload
// Accepts a reference to the right hand side of the add operation
// no return, values are changed internally
void Vector3::operator-=(const Vector3& rhs) {
	DEBUG_PRINT("Vector3::operator-=(rhs(%f, %f, %f))\n", rhs.x, rhs.y, rhs.z);
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
}

// Multiply Assignment Operator Overload
// Accepts a reference to the right hand side of the add operation
// no return, values are changed internally
void Vector3::operator*=(const float& scalar) {
	DEBUG_PRINT("Vector3::operator*=(%f)\n", scalar);
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

// Division Assignment Operator Overload
// Accepts a reference to the right hand side of the add operation
// no return, values are changed internally
void Vector3::operator/=(const float& scalar) {
	DEBUG_PRINT("Vector3::operator/=(%f)\n", scalar);
	x /= scalar;
	y /= scalar;
	z /= scalar;
}

// Function that streams each axis position internally stored
void Vector3::vOut() {
	std::cout << "(" << x << ", " << y << ", " << z << ")";
}

// Returns a glm::vec3 of a Vector3
glm::vec3 Vector3::getGlmVec3() {
	return glm::vec3(this->x, this->y, this->z);
}

// Normalize changes the values of the vector to be 
// a unit length
void Vector3::Normalize() {
	float m = Magnitude();
	x /= m;
	y /= m;
	z /= m;
}

// Magnitude will return the length of the vector
float Vector3::Magnitude() {
	return sqrt(x * x + y * y + z * z);
}