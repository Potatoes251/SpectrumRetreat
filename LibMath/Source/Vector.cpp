#define _USE_MATH_DEFINES

#include <cmath>
#include "./LibMath/Vector.h"
#include "./LibMath/Matrix.h"

#pragma region Vector2

LibMath::Vector2::Vector2()
{
	m_x = 0;
	m_y = 0;
}

LibMath::Vector2::Vector2(float nb)
{
	m_x = nb;
	m_y = nb;
}

LibMath::Vector2::Vector2(float x, float y)
{
	m_x = x;
	m_y = y;
}

LibMath::Vector2::Vector2(const Vector2& other)
{
	m_x = other.m_x;
	m_y = other.m_y;
}

float LibMath::Vector2::getX() const
{
	return m_x;
}

float LibMath::Vector2::getY() const
{
	return m_y;
}

float LibMath::Vector2::magnitude() const
{
	return sqrtf(powf(m_x, 2.0) + powf(m_y, 2.0));
}

float LibMath::Vector2::magnitudeSquare() const
{
	return (powf(m_x, 2) + powf(m_y, 2));
}

bool LibMath::Vector2::isUnit() const
{
	float mag = magnitude();

	return (mag == 1.0);  // Returns true if the magnitude is 1
}

float LibMath::Vector2::dotProduct(const Vector2 v) 
{
	return ((m_x * v.getX()) + (m_y * v.getY()));
}

float LibMath::Vector2::crossProduct(const Vector2 v)
{
	return ((m_x * v.getY()) - (m_y * v.getX()));
}

LibMath::Vector2 LibMath::Vector2::normalize()
{
	float mag = magnitude();

	return Vector2{m_x / mag, m_y / mag};
}

float LibMath::Vector2::operator[](int nb) const
{
	if (nb == 0)
	{
		return m_x;
	}

	if (nb == 1)
	{
		return m_y;
	}

	throw std::invalid_argument("Invalid argument");
}

float& LibMath::Vector2::operator[](int nb) 
{
	if (nb == 0)
	{
		return m_x;
	}

	if (nb == 1)
	{
		return m_y;
	}

	throw std::invalid_argument("Invalid argument");
}

LibMath::Vector2 LibMath::Vector2::projectOnto(Vector2 other)
{
	float dot = dotProduct(other);
	
	return (other * (dot / other.dotProduct(other)));
}

LibMath::Vector2 LibMath::Vector2::reflectOnto(Vector2 other)
{
	float dot = dotProduct(other);

	// Calculate dot product of the other vector with itself (magnitude squared of the normal)
	float magnitudeSquared = other.dotProduct(other);

	// Calculate the reflection vector using the reflection formula
	float scalar = 2.0f * dot / magnitudeSquared;

	Vector2 reflection = *this - (other * scalar);

	return reflection;
}

float LibMath::Vector2::angleBetween(Vector2 other)
{
	float dot = dotProduct(other);
	float magA = magnitude();
	float magB = other.magnitude();

	// Calculate the cosine of the angle
	float cosTheta = dot / (magA * magB);

	// Clamp cosTheta to avoid errors due to precision issues
	if (cosTheta > 1) cosTheta = 1;
	if (cosTheta < -1) cosTheta = -1;

	// Calculate the angle in radians
	float thetaRad = acosf(cosTheta);

	// Convert radians to degrees
	float thetaDeg = thetaRad * (180.0f / (float)M_PI);

	return thetaDeg;
}

LibMath::Vector2& LibMath::Vector2::operator=(LibMath::Vector2 other)
{
	m_x = other.m_x;
	m_y = other.m_y;

	return *this;
}

bool LibMath::operator==(Vector2 v1, Vector2 v2)
{
	return (v1.getX() == v2.getX() && v1.getY() == v2.getY());
}

LibMath::Vector2 LibMath::operator-(Vector2 v)
{
	return Vector2{ -v.getX(), -v.getY() };
}

LibMath::Vector2 LibMath::operator+(Vector2 v1, Vector2 v2)
{
	return Vector2{ v1.getX() + v2.getX(), v1.getY() + v2.getY() };
}

LibMath::Vector2 LibMath::operator-(Vector2 v1, Vector2 v2)
{
	return Vector2{ v1.getX() - v2.getX(), v1.getY() - v2.getY() };
}

LibMath::Vector2 LibMath::operator*(Vector2 v, float nb)
{
	return Vector2{ v.getX() * nb, v.getY() * nb };
}

LibMath::Vector2 LibMath::operator/(Vector2 v, float nb)
{
	return Vector2{ v.getX() / nb, v.getY() / nb };
}



#pragma endregion All functions for Vector2

#pragma region Vector3

LibMath::Vector3::Vector3()
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
}

LibMath::Vector3::Vector3(float nb)
{
	m_x = nb;
	m_y = nb;
	m_z = nb;
}

LibMath::Vector3::Vector3(float nb1, float nb2, float nb3)
{
	m_x = nb1;
	m_y = nb2;
	m_z = nb3;
}

LibMath::Vector3::Vector3(Vector3 const& other)
{
	m_x = other.m_x;
	m_y = other.m_y;
	m_z = other.m_z;
}

LibMath::Vector3 LibMath::Vector3::zero() 
{
	return Vector3(0, 0, 0);
}

LibMath::Vector3 LibMath::Vector3::one() 
{
	return Vector3(1, 1, 1);
}

LibMath::Vector3 LibMath::Vector3::up() 
{
	return Vector3(0, 1, 0);
}

LibMath::Vector3 LibMath::Vector3::down() 
{
	return Vector3(0, -1, 0);
}

LibMath::Vector3 LibMath::Vector3::left()
{
	return Vector3(-1, 0, 0);
}

LibMath::Vector3 LibMath::Vector3::right() 
{
	return Vector3(1, 0, 0);
}

LibMath::Vector3 LibMath::Vector3::front() 
{
	return Vector3(0, 0, 1);
}

LibMath::Vector3 LibMath::Vector3::back() 
{
	return Vector3(0, 0, -1);
}

LibMath::Vector3& LibMath::Vector3::operator=(Vector3 const& other) 
{
	if (this != &other)
	{
		m_x = other.m_x;
		m_y = other.m_y;
		m_z = other.m_z;
	}

	return *this;
}

float& LibMath::Vector3::operator[](int index) 
{
	switch (index) 
	{
		case 0: return m_x;
		case 1: return m_y;
		case 2: return m_z;
		default: throw std::out_of_range("Index out of range");
	}
}

float LibMath::Vector3::operator[](int index) const 
{
	switch (index) 
	{
		case 0: return m_x;
		case 1: return m_y;
		case 2: return m_z;
		default: throw std::out_of_range("Index out of range");
	}
}

// Angle Between Vectors
LibMath::Radian LibMath::Vector3::angleFrom(Vector3 const& other) const 
{
	// Calculate the angle between this vector and the other vector
	float dotProd = this->dot(other);
	float magA = this->magnitude();
	float magB = other.magnitude();

	return Radian(std::acos(dotProd / (magA * magB)));
}

// Cross Product
LibMath::Vector3 LibMath::Vector3::cross(Vector3 const& other) const 
{
	float x = m_y * other.m_z - m_z * other.m_y;
	float y = m_z * other.m_x - m_x * other.m_z;
	float z = m_x * other.m_y - m_y * other.m_x;

	return Vector3(x, y, z);
}

float LibMath::Vector3::distanceFrom(Vector3 const& other) const 
{
	return std::sqrt(distanceSquaredFrom(other));
}

float LibMath::Vector3::distanceSquaredFrom(Vector3 const& other) const 
{
	return (m_x - other.m_x) * (m_x - other.m_x) +
		   (m_y - other.m_y) * (m_y - other.m_y) +
		   (m_z - other.m_z) * (m_z - other.m_z);
}

float LibMath::Vector3::distance2DFrom(Vector3 const& other) const 
{
	return std::sqrt(distance2DSquaredFrom(other));
}

float LibMath::Vector3::distance2DSquaredFrom(Vector3 const& other) const 
{
	return (m_x - other.m_x) * (m_x - other.m_x) +
		   (m_y - other.m_y) * (m_y - other.m_y);
}

float LibMath::Vector3::dot(Vector3 const& other) const
{
	return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

// Magnitude and Unit Vector Check
bool LibMath::Vector3::isLongerThan(Vector3 const& other) const
{
	return magnitude() > other.magnitude();
}

bool LibMath::Vector3::isShorterThan(Vector3 const& other) const
{
	return magnitude() < other.magnitude();
}

bool LibMath::Vector3::isUnitVector() const
{
	return std::abs(magnitude() - 1.0f) < 0.0001f;
}

float LibMath::Vector3::magnitude() const
{
	return std::sqrt(magnitudeSquared());
}

float LibMath::Vector3::magnitudeSquared() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z;
}

LibMath::Vector3 LibMath::Vector3::normalize()
{
	float mag = magnitude();

	if (mag > 0)
	{
		m_x /= mag;
		m_y /= mag;
		m_z /= mag;
	}

	return *this;
}

// Projection and Reflection
void LibMath::Vector3::projectOnto(Vector3 const& other) 
{
	float dotProd = dot(other);
	float magSquared = other.magnitudeSquared();

	*this = other * (dotProd / magSquared);
}

void LibMath::Vector3::reflectOnto(Vector3 const& other) 
{
	*this = *this - other * (2 * dot(other));
}

// Rotate using Euler
void LibMath::Vector3::rotate(Radian pitch, Radian yaw, Radian roll) 
{
	float cosPitch = std::cos(pitch.radian());
	float sinPitch = std::sin(pitch.radian());

	float cosYaw = std::cos(yaw.radian());
	float sinYaw = std::sin(yaw.radian());

	float cosRoll = std::cos(roll.radian());
	float sinRoll = std::sin(roll.radian());

	// Pitch
	Matrix3 rotX;

	rotX[0][0] = 1;
	rotX[1][1] = cosPitch;
	rotX[1][2] = -sinPitch;
	rotX[2][1] = sinPitch; 
	rotX[2][2] = cosPitch;

	// Yaw
	Matrix3 rotY;

	rotY[0][0] = cosYaw;
	rotY[0][2] = sinYaw;
	rotY[1][1] = 1; 
	rotY[2][0] = -sinYaw;
	rotY[2][2] = cosYaw;

	// Roll
	Matrix3 rotZ;

	rotZ[0][0] = cosRoll; 
	rotZ[0][1] = -sinRoll;
	rotZ[1][0] = sinRoll;
	rotZ[1][1] = cosRoll;
	rotZ[2][2] = 1;

	// Combined rotation
	Matrix3 rotationMatrix = rotZ * rotX * rotY;

	*this = rotationMatrix * (*this);
}

// Rotate vector around an arbitrary axis
void LibMath::Vector3::rotate(Radian angle, Vector3 const& axis)
{
	// Normalize the axis to make sure it's a unit vector
	Vector3 unitAxis = axis;

	unitAxis.normalize();

	float dotProduct = unitAxis.dot(*this);

	Vector3 crossProduct = unitAxis.cross(*this);

	// Rodrigue rotation formula
	*this = *this * std::cos(angle.radian()) + crossProduct * std::sin(angle.radian()) 
			+ unitAxis * dotProduct * (1 - std::cos(angle.radian()));
}

// Scaling
void LibMath::Vector3::scale(Vector3 const& factor)
{
	m_x *= factor.m_x;
	m_y *= factor.m_y;
	m_z *= factor.m_z;
}

std::string LibMath::Vector3::string() const
{
	return "{" + std::to_string(m_x) + "," + std::to_string(m_y) + "," + std::to_string(m_z) + "}";
}

//// String Representation
//std::string LibMath::Vector3::string() const 
//{
//	std::stringstream ss;
//
//	ss << "{" << m_x << ", " << m_y << ", " << m_z << "}";
//
//	return ss.str();
//}
//
//std::string LibMath::Vector3::stringLong() const 
//{
//	std::stringstream ss;
//
//	ss << "Vector3(" << m_x << ", " << m_y << ", " << m_z << ")";
//
//	return ss.str();
//}

// Translation
void LibMath::Vector3::translate(Vector3 const& offset) 
{
	m_x += offset.m_x;
	m_y += offset.m_y;
	m_z += offset.m_z;
}

bool LibMath::operator==(Vector3 const& v1, Vector3 const& v2) 
{
	return v1.m_x == v2.m_x && v1.m_y == v2.m_y && v1.m_z == v2.m_z;
}

bool LibMath::operator!=(Vector3 const& v1, Vector3 const& v2) 
{
	return !(v1 == v2);
}

LibMath::Vector3 LibMath::operator-(Vector3 other) 
{
	return Vector3(-other.m_x, -other.m_y, -other.m_z);
}

LibMath::Vector3 LibMath::operator+(Vector3 v1, Vector3 const& v2) 
{
	v1.m_x += v2.m_x;
	v1.m_y += v2.m_y;
	v1.m_z += v2.m_z;

	return v1;
}

LibMath::Vector3 LibMath::operator-(Vector3 v1, Vector3 const& v2) 
{
	v1.m_x -= v2.m_x;
	v1.m_y -= v2.m_y;
	v1.m_z -= v2.m_z;

	return v1;
}

LibMath::Vector3 LibMath::operator*(Vector3 v1, Vector3 const& v2) 
{
	v1.m_x *= v2.m_x;
	v1.m_y *= v2.m_y;
	v1.m_z *= v2.m_z;

	return v1;
}

LibMath::Vector3 LibMath::operator/(Vector3 v1, Vector3 const& v2) 
{
	v1.m_x /= v2.m_x;
	v1.m_y /= v2.m_y;
	v1.m_z /= v2.m_z;

	return v1;
}

LibMath::Vector3& LibMath::operator+=(Vector3& v1, Vector3 const& v2) 
{
	v1.m_x += v2.m_x;
	v1.m_y += v2.m_y;
	v1.m_z += v2.m_z;

	return v1;
}

LibMath::Vector3& LibMath::operator-=(Vector3& v1, Vector3 const& v2) 
{
	v1.m_x -= v2.m_x;
	v1.m_y -= v2.m_y;
	v1.m_z -= v2.m_z;

	return v1;
}

LibMath::Vector3& LibMath::operator*=(Vector3& v1, Vector3 const& v2) 
{
	v1.m_x *= v2.m_x;
	v1.m_y *= v2.m_y;
	v1.m_z *= v2.m_z;

	return v1;
}

LibMath::Vector3& LibMath::operator/=(Vector3& v1, Vector3 const& v2) 
{
	v1.m_x /= v2.m_x;
	v1.m_y /= v2.m_y;
	v1.m_z /= v2.m_z;

	return v1;
}

LibMath::Vector3	LibMath::operator*(Vector3 const& v, float nb)
{
	Vector3 result;

	result.m_x = v.m_x * nb;
	result.m_y = v.m_y * nb;
	result.m_z = v.m_z * nb;

	return result;
}

std::ostream& LibMath::operator<<(std::ostream& os, Vector3 const& vec)
{
	os << vec.string();
	return os;
}

//std::ostream& operator<<(std::ostream& os, LibMath::Vector3 const& vec) 
//{
//	return os << vec.string();
//}

std::istream& operator>>(std::istream& is, LibMath::Vector3& vec)
{
	char ch;

	is >> ch; // '{'
	is >> vec.m_x;
	is >> ch; // ','
	is >> vec.m_y;
	is >> ch; // ','
	is >> vec.m_z;
	is >> ch; // '}'

	return is;
}

#pragma endregion All functions for Vector3

#pragma region Vector4


LibMath::Vector4& LibMath::Vector4::operator=(Vector4 const& other)
{
	if (this != &other) 
	{
		m_x = other.m_x;
		m_y = other.m_y;
		m_z = other.m_z;
		m_w = other.m_w;
	}

	return *this;
}

float& LibMath::Vector4::operator[](int index)
{
	if (index == 0) return m_x;
	if (index == 1) return m_y;
	if (index == 2) return m_z;

	return m_w; 
}

float  LibMath::Vector4::operator[](int index) const 
{
	if (index == 0) return m_x;
	if (index == 1) return m_y;
	if (index == 2) return m_z;

	return m_w;  
}

float  LibMath::Vector4::dotProduct(Vector4 const& other) const
{
	return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z + m_w * other.m_w;
}


float LibMath::Vector4::magnitude() const
{
	return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

float LibMath::Vector4::magnitudeSquared() const 
{
	return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
}

void LibMath::Vector4::homogenize() 
{
	if (m_w != 0)
	{
		m_x /= m_w;
		m_y /= m_w;
		m_z /= m_w;
		m_w = 1.0f;
	}
}

std::string LibMath::Vector4::string() const
{
	return "{" + std::to_string(m_x) + "," + std::to_string(m_y) + "," + std::to_string(m_z) + "," + std::to_string(m_w) + "}";
}

// Operator == (comparison for equality)
bool LibMath::operator==(Vector4 const& v1, Vector4 const& v2) 
{
	return v1.m_x == v2.m_x && v1.m_y == v2.m_y && v1.m_z == v2.m_z && v1.m_w == v2.m_w;
}

// Operator != (comparison for inequality)
bool LibMath::operator!=(Vector4 const& v1, Vector4 const& v2) 
{
	return !(v1 == v2);  // Simply return the negation of operator==
}

// Operator + (addition)
LibMath::Vector4 LibMath::operator+(Vector4 v1, Vector4 const& v2)
{
	v1.m_x += v2.m_x;
	v1.m_y += v2.m_y;
	v1.m_z += v2.m_z;
	v1.m_w += v2.m_w;

	return v1;
}

// Operator - (subtraction)
LibMath::Vector4 LibMath::operator-(Vector4 v1, Vector4 const& v2)
{
	v1.m_x -= v2.m_x;
	v1.m_y -= v2.m_y;
	v1.m_z -= v2.m_z;
	v1.m_w -= v2.m_w;

	return v1;
}

// Operator * (component-wise multiplication)
LibMath::Vector4 LibMath::operator*(Vector4 v1, Vector4 const& v2)
{
	v1.m_x *= v2.m_x;
	v1.m_y *= v2.m_y;
	v1.m_z *= v2.m_z;
	v1.m_w *= v2.m_w;

	return v1;
}

// Operator / (component-wise division)
LibMath::Vector4 LibMath::operator/(Vector4 v1, Vector4 const& v2)
{
	v1.m_x /= v2.m_x;
	v1.m_y /= v2.m_y;
	v1.m_z /= v2.m_z;
	v1.m_w /= v2.m_w;

	return v1;
}

// Operator += (addition assignment)
LibMath::Vector4& LibMath::operator+=(Vector4& v1, Vector4 const& v2) 
{
	v1.m_x += v2.m_x;
	v1.m_y += v2.m_y;
	v1.m_z += v2.m_z;
	v1.m_w += v2.m_w;

	return v1;
}

// Operator -= (subtraction assignment)
LibMath::Vector4& LibMath::operator-=(Vector4& v1, Vector4 const& v2)
{
	v1.m_x -= v2.m_x;
	v1.m_y -= v2.m_y;
	v1.m_z -= v2.m_z;
	v1.m_w -= v2.m_w;

	return v1;
}

// Operator *= (component-wise multiplication assignment)
LibMath::Vector4& LibMath::operator*=(Vector4& v1, Vector4 const& v2)
{
	v1.m_x *= v2.m_x;
	v1.m_y *= v2.m_y;
	v1.m_z *= v2.m_z;
	v1.m_w *= v2.m_w;

	return v1;
}

// Operator /= (component-wise division assignment)
LibMath::Vector4& LibMath::operator/=(Vector4& v1, Vector4 const& v2)
{
	v1.m_x /= v2.m_x;
	v1.m_y /= v2.m_y;
	v1.m_z /= v2.m_z;
	v1.m_w /= v2.m_w;

	return v1;
}

// Scalar multiplication with Vector4 and Vector3
LibMath::Vector4 LibMath::operator*(Vector4 const& v, float nb)
{
	return Vector4(v.m_x * nb, v.m_y * nb, v.m_z * nb, v.m_w * nb);
}

std::ostream& LibMath::operator<<(std::ostream& os, Vector4 const& vec)
{
	os << vec.string();
	return os;
}


#pragma endregion All functions for Vector4