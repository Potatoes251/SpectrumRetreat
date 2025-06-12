#define _USE_MATH_DEFINES
#include <cmath>
#include "./LibMath/Matrix.h"


#pragma region Matrix2

LibMath::Matrix2::Matrix2()
{
	m_values[0][0] = 0;
	m_values[0][1] = 0;
	m_values[1][0] = 0;
	m_values[1][1] = 0;
}

LibMath::Matrix2::Matrix2(float value)
{
	m_values[0][0] = value;
	m_values[0][1] = value;
	m_values[1][0] = value;
	m_values[1][1] = value;
}

LibMath::Matrix2::Matrix2(float nb1, float nb2, float nb3, float nb4)
{
	m_values[0][0] = nb1;
	m_values[1][0] = nb2;
	m_values[0][1] = nb3;
	m_values[1][1] = nb4;
}

LibMath::Matrix2::Matrix2(const Matrix2& other)
{
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			m_values[i][j] = other.m_values[i][j];
		}
	}
}

float LibMath::Matrix2::operator[](int nb) const
{
	if (nb < 0 || nb > 3)
		return 0.0f; // handle safely or throw

	int col = nb % 2;
	int row = nb / 2;

	return m_values[col][row];
}

float* LibMath::Matrix2::operator[](int col)
{
	if (col < 0 || col > 1)
		return nullptr; // or throw

	return m_values[col];
}

LibMath::Matrix2& LibMath::Matrix2::operator=(Matrix2 other)
{
	m_values[0][0] = other.m_values[0][0];
	m_values[0][1] = other.m_values[0][1];
	m_values[1][0] = other.m_values[1][0];
	m_values[1][1] = other.m_values[1][1];

	return *this;
}

LibMath::Matrix2 LibMath::operator+(LibMath::Matrix2 m, LibMath::Matrix2 other)
{
	Matrix2 sum;

	sum[0][0] = m[0][0] + other[0][0];
	sum[0][1] = m[0][1] + other[0][1];
	sum[1][0] = m[1][0] + other[1][0];
	sum[1][1] = m[1][1] + other[1][1];

	return sum;
}

LibMath::Vector2 LibMath::operator*(LibMath::Matrix2 m, LibMath::Vector2 v)
{
	//Create a vector to store the result
	LibMath::Vector2 result;

	result[0] = m[0][0] * v.getX() + m[1][0] * v.getY(); // column 0, row 0 and column 1, row 0
	result[1] = m[0][1] * v.getX() + m[1][1] * v.getY(); // column 0, row 1 and column 1, row 1

	return result;
}

LibMath::Matrix2 LibMath::operator*(LibMath::Matrix2 m, LibMath::Matrix2 other)
{
	// Multiplying the matrices
	Matrix2 result(0);
	for (int col = 0; col < 2; ++col)
		for (int row = 0; row < 2; ++row)
			for (int k = 0; k < 2; ++k)
				result[col][row] += m[k][row] * other[col][k];

	return result;
}

LibMath::Matrix2 LibMath::operator*(LibMath::Matrix2 m, float nb)
{
	Matrix2 result;
	for (int col = 0; col < 2; ++col)
		for (int row = 0; row < 2; ++row)
			result[col][row] = m[col][row] * nb;
	return result;
}

LibMath::Matrix2 LibMath::Matrix2::transpose()
{
	Matrix2 result;
	for (int col = 0; col < 2; ++col)
		for (int row = 0; row < 2; ++row)
			result[row][col] = m_values[col][row];
	return result;
}

LibMath::Matrix2 LibMath::Matrix2::minors()
{
	Matrix2 result(0);

	result[0][0] = m_values[1][1];
	result[0][1] = m_values[1][0];
	result[1][0] = m_values[0][1];
	result[1][1] = m_values[0][0];

	return result;
}

LibMath::Matrix2 LibMath::Matrix2::cofactors()
{
	Matrix2 minor = minors();

	// Create a 2x2 matrix to store the cofactors
	Matrix2 cofactor(0);

	// Apply the formula for cofactors: Cofactor = (-1)^(i+j) * Minor
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			int sign = (i + j) % 2 == 0 ? 1 : -1; // Determine the sign (-1)^(i+j)

			cofactor[i][j] = sign * minor[i][j];
		}
	}

	return cofactor;
}

LibMath::Matrix2 LibMath::Matrix2::adjugate()
{
	Matrix2 cofactor = cofactors();

	// The adjugate is the transpose of the cofactor matrix
	Matrix2 adj(0);

	// Transpose the cofactor matrix
	adj[0][0] = cofactor[0][0];
	adj[0][1] = cofactor[1][0];
	adj[1][0] = cofactor[0][1];
	adj[1][1] = cofactor[1][1];

	return adj;
}

float LibMath::Matrix2::determinant() const
{
	return (m_values[0][0] * m_values[1][1]) - (m_values[0][1] * m_values[1][0]);
}

LibMath::Matrix2 LibMath::Matrix2::inverse()
{
	float det = determinant();

	// If the determinant is 0, the inverse doesn't exist
	if (det == 0.0f)
	{
		//throw invalid_argument("Matrix is singular and cannot be inverted");
	}

	// This makes sure that if there is a zero, we can still do the operations without crashing
	float invDet = 1.0f / det;

	Matrix2 result(0);

	result[0][0] = m_values[1][1] * invDet;
	result[0][1] = -m_values[0][1] * invDet;
	result[1][0] = -m_values[1][0] * invDet;
	result[1][1] = m_values[0][0] * invDet;

	return result;
}

LibMath::Matrix2 LibMath::Matrix2::createTransform(LibMath::Radian rad, LibMath::Vector2 scale)
{
	// Create the scaling matrix (x, y)
	Matrix2 scalingMatrix(scale.getX(), 0, 0, scale.getY());

	// Create the rotation matrix 
	float cosAngle = std::cos(rad.radian());
	float sinAngle = std::sin(rad.radian());

	Matrix2 rotationMatrix(cosAngle, sinAngle, -sinAngle, cosAngle);

	// Combine scaling and rotation 
	return scalingMatrix * rotationMatrix;
}

LibMath::Matrix2 LibMath::Matrix2::createRotation(float rad)
{
	float cosAngle = std::cos(rad);
	float sinAngle = std::sin(rad);

	// Create the rotation matrix
	Matrix2 rotationMatrix(cosAngle, sinAngle, -sinAngle, cosAngle);

	return rotationMatrix;
}

LibMath::Matrix2 LibMath::Matrix2::createScale(LibMath::Vector2 v)
{
	return Matrix2(v.getX(), 0, 0, v.getY());
}

bool LibMath::Matrix2::identity()
{
	return (m_values[0][0] == 1 && m_values[1][0] == 0 && m_values[0][1] == 0 && m_values[1][1] == 1);
}

#pragma endregion All functions for Matrix2

#pragma region Matrix3

LibMath::Matrix3::Matrix3()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_values[i][j] = 0;
		}
	}
}

LibMath::Matrix3::Matrix3(float nb)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_values[i][j] = nb;
		}
	}
}

LibMath::Matrix3::Matrix3(float nb1, float nb2, float nb3, float nb4, float nb5, float nb6, float nb7, float nb8, float nb9)
{
	m_values[0][0] = nb1;
	m_values[1][0] = nb2;
	m_values[2][0] = nb3;
	m_values[0][1] = nb4;
	m_values[1][1] = nb5;
	m_values[2][1] = nb6;
	m_values[0][2] = nb7;
	m_values[1][2] = nb8;
	m_values[2][2] = nb9;
}

LibMath::Matrix3::Matrix3(const Matrix3& other)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_values[i][j] = other.m_values[i][j];
		}
	}
}

float LibMath::Matrix3::operator[](int nb) const
{
	if (nb < 0 || nb > 9)
	{
		//throw std::invalid_argument("Invalid argument");
	}

	return *m_values[nb];
}

float* LibMath::Matrix3::operator[](int nb)
{
	if (nb < 0 || nb > 9)
	{
		//throw std::invalid_argument("Invalid argument");
	}

	return m_values[nb];
}

LibMath::Matrix3& LibMath::Matrix3::operator=(Matrix3 other)
{
	m_values[0][0] = other.m_values[0][0];
	m_values[0][1] = other.m_values[0][1];
	m_values[0][2] = other.m_values[0][2];
	m_values[1][0] = other.m_values[1][0];
	m_values[1][1] = other.m_values[1][1];
	m_values[1][2] = other.m_values[1][2];
	m_values[2][0] = other.m_values[2][0];
	m_values[2][1] = other.m_values[2][1];
	m_values[2][2] = other.m_values[2][2];

	return *this;
}

LibMath::Matrix3 LibMath::Matrix3::createTransform2D(LibMath::Radian rad, LibMath::Vector2 scale)
{
	// Create the scaling matrix
	Matrix3 scalingMatrix(
		scale.getX(), 0, 0,
		0, scale.getY(), 0,
		0, 0, 1
	);

	//Create the rotation matrix 
	float cosAngle = std::cos(rad.radian());
	float sinAngle = std::sin(rad.radian());

	Matrix3 rotationMatrix(
		cosAngle, -sinAngle, 0,
		sinAngle, cosAngle, 0,
		0, 0, 1
	);

	// Combine scaling and rotation 
	return scalingMatrix * rotationMatrix;
}

LibMath::Matrix3 LibMath::Matrix3::createTranslation2D(LibMath::Vector2 v)
{
	return Matrix3(
		1, 0, v.getX(),
		0, 1, v.getY(),
		0, 0, 1          // homogeneous coordinate 
	);
}

LibMath::Matrix3 LibMath::Matrix3::createTranslation3D(LibMath::Vector3 v)
{
	Matrix3 translation;

	translation.m_values[0][2] = v.m_x;
	translation.m_values[1][2] = v.m_y;
	translation.m_values[2][2] = v.m_z;

	return translation;
}

LibMath::Matrix3 LibMath::Matrix3::createRotation(LibMath::Point2D point, float rad)
{
	float cosAngle = std::cos(rad);
	float sinAngle = std::sin(rad);

	// Translation to origin (column-major order)
	Matrix3 translateToOrigin(
		1, 0, 0,               // Column 1
		0, 1, 0,               // Column 2
		-point.X(), -point.Y(), 1  // Column 3
	);

	// Rotation matrix (column-major order)
	Matrix3 rotationMatrix(
		cosAngle, sinAngle, 0,   // Column 1
		-sinAngle, cosAngle, 0,  // Column 2
		0, 0, 1                  // Column 3
	);

	// Translation back to original position (column-major order)
	Matrix3 translateBack(
		1, 0, 0,              // Column 1
		0, 1, 0,              // Column 2
		point.X(), point.Y(), 1  // Column 3
	);

	// Final transformation
	return translateBack * rotationMatrix * translateToOrigin;
}

LibMath::Matrix3 LibMath::Matrix3::createRotationX(Vector3 origin, float rad)
{
	Matrix3 rotationMatrix;

	rotationMatrix.m_values[0][0] = 1;       rotationMatrix.m_values[0][1] = 0;                       rotationMatrix.m_values[0][2] = 0;
	rotationMatrix.m_values[1][0] = 0;       rotationMatrix.m_values[1][1] = cos(rad);                  rotationMatrix.m_values[1][2] = -sin(rad);
	rotationMatrix.m_values[2][0] = 0;       rotationMatrix.m_values[2][1] = sin(rad);                  rotationMatrix.m_values[2][2] = cos(rad);

	// Translate to the origin, rotate, and then translate back
	Matrix3 translationToOrigin = createTranslation3D(Vector3(-origin.m_x, -origin.m_y, -origin.m_z));
	Matrix3 translationBack = createTranslation3D(Vector3(origin.m_x, origin.m_y, origin.m_z));

	// Combine the translation, rotation, and translation back
	return translationBack * rotationMatrix * translationToOrigin;
}

LibMath::Matrix3 LibMath::Matrix3::createRotationY(Vector3 origin, float rad)
{
	float cosAngle = std::cos(rad);
	float sinAngle = std::sin(rad);

	// Rotation around X axis in column-major order
	Matrix3 rotationMatrix(
		1, 0, 0,   // Column 1
		0, cosAngle, sinAngle,   // Column 2
		0, -sinAngle, cosAngle    // Column 3
	);

	// Translate to the origin, rotate, then translate back
	Matrix3 translationToOrigin = createTranslation3D(Vector3(-origin.m_x, -origin.m_y, -origin.m_z));
	Matrix3 translationBack = createTranslation3D(Vector3(origin.m_x, origin.m_y, origin.m_z));

	return translationBack * rotationMatrix * translationToOrigin;
}

LibMath::Matrix3 LibMath::Matrix3::createRotationZ(Vector3 origin, float rad)
{
	float cosAngle = std::cos(rad);
	float sinAngle = std::sin(rad);

	// Rotation around Z axis in column-major order
	Matrix3 rotationMatrix(
		cosAngle, sinAngle, 0,   // Column 1
		-sinAngle, cosAngle, 0,   // Column 2
		0, 0, 1    // Column 3
	);

	// Translate to the origin, rotate, and then translate back
	Matrix3 translationToOrigin = createTranslation3D(Vector3(-origin.m_x, -origin.m_y, -origin.m_z));
	Matrix3 translationBack = createTranslation3D(Vector3(origin.m_x, origin.m_y, origin.m_z));

	// Combine the translation, rotation, and translation back
	return translationBack * rotationMatrix * translationToOrigin;
}


LibMath::Matrix3 LibMath::Matrix3::createScale2D(LibMath::Vector2 v)
{
	return Matrix3(
		v.getX(), 0.0f, 0.0f,   // Column 1
		0.0f, v.getY(), 0.0f,   // Column 2
		0.0f, 0.0f, 1.0f    // Column 3
	);
}

LibMath::Matrix3 LibMath::Matrix3::createScale3D(Vector3 v)
{
	return Matrix3(
		v.m_x, 0.0f, 0.0f,  // Column 1
		0.0f, v.m_y, 0.0f,  // Column 2
		0.0f, 0.0f, v.m_z   // Column 3
	);
}

LibMath::Matrix3 LibMath::Matrix3::transpose()
{
	Matrix3 transposed;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			transposed.m_values[i][j] = m_values[j][i];
		}
	}
	return transposed;
}

LibMath::Matrix3 LibMath::Matrix3::minors()
{
	Matrix3 minors;

	minors.m_values[0][0] = m_values[1][1] * m_values[2][2] - m_values[1][2] * m_values[2][1];
	minors.m_values[0][1] = m_values[1][0] * m_values[2][2] - m_values[1][2] * m_values[2][0];
	minors.m_values[0][2] = m_values[1][0] * m_values[2][1] - m_values[1][1] * m_values[2][0];

	minors.m_values[1][0] = m_values[0][1] * m_values[2][2] - m_values[0][2] * m_values[2][1];
	minors.m_values[1][1] = m_values[0][0] * m_values[2][2] - m_values[0][2] * m_values[2][0];
	minors.m_values[1][2] = m_values[0][0] * m_values[2][1] - m_values[0][1] * m_values[2][0];

	minors.m_values[2][0] = m_values[0][1] * m_values[1][2] - m_values[0][2] * m_values[1][1];
	minors.m_values[2][1] = m_values[0][0] * m_values[1][2] - m_values[0][2] * m_values[1][0];
	minors.m_values[2][2] = m_values[0][0] * m_values[1][1] - m_values[0][1] * m_values[1][0];

	return minors;
}

LibMath::Matrix3 LibMath::Matrix3::cofactors()
{
	Matrix3 minorsMatrix = minors();
	Matrix3 cofactors;

	// Put the cofactor sign (+ - +, - + -, + - +)
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			cofactors.m_values[i][j] = minorsMatrix.m_values[i][j] * ((i + j) % 2 == 0 ? 1 : -1);
		}
	}

	return cofactors;
}

LibMath::Matrix3 LibMath::Matrix3::adjugate()
{
	Matrix3 cofactorsMatrix = cofactors();

	return cofactorsMatrix.transpose();
}

LibMath::Matrix3 LibMath::Matrix3::inverse()
{
	float det = determinant();

	if (det == 0.0f)
	{
		// If the determinant is zero, the matrix is singular and doesn't have an inverse
		throw std::runtime_error("Matrix is singular and cannot be inverted.");
	}

	Matrix3 adjugateMatrix = adjugate();
	Matrix3 inverseMatrix;

	// Divide each element of the adjugate matrix by the determinant
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			inverseMatrix.m_values[i][j] = adjugateMatrix.m_values[i][j] / det;
		}
	}

	return inverseMatrix;
}

float LibMath::Matrix3::determinant() const
{
	return m_values[0][0] * (m_values[1][1] * m_values[2][2] - m_values[1][2] * m_values[2][1]) -
		m_values[0][1] * (m_values[1][0] * m_values[2][2] - m_values[1][2] * m_values[2][0]) +
		m_values[0][2] * (m_values[1][0] * m_values[2][1] - m_values[1][1] * m_values[2][0]);
}

LibMath::Matrix3 LibMath::operator+(Matrix3 m, Matrix3 other)
{
	Matrix3 sum;

	sum[0][0] = m[0][0] + other[0][0];
	sum[0][1] = m[0][1] + other[0][1];
	sum[0][2] = m[0][2] + other[0][2];

	sum[1][0] = m[1][0] + other[1][0];
	sum[1][1] = m[1][1] + other[1][1];
	sum[1][2] = m[1][2] + other[1][2];

	sum[2][0] = m[2][0] + other[2][0];
	sum[2][1] = m[2][1] + other[2][1];
	sum[2][2] = m[2][2] + other[2][2];

	return sum;
}

LibMath::Matrix3 LibMath::operator*(Matrix3 m, float nb)
{
	Matrix3 sum;

	// Scalar multiplication for each matrix element
	sum[0][0] = m[0][0] * nb;
	sum[1][0] = m[0][1] * nb;
	sum[2][0] = m[0][2] * nb;

	sum[0][1] = m[1][0] * nb;
	sum[1][1] = m[1][1] * nb;
	sum[2][1] = m[1][2] * nb;

	sum[0][2] = m[2][0] * nb;
	sum[1][2] = m[2][1] * nb;
	sum[2][2] = m[2][2] * nb;

	return sum;
}

LibMath::Matrix3 LibMath::operator*(Matrix3 m, Matrix3 other)
{
	Matrix3 result(0.0f);

	for (int col = 0; col < 3; ++col)
	{
		for (int row = 0; row < 3; ++row)
		{
			float sum = 0.0f;

			for (int k = 0; k < 3; ++k)
			{
				sum += m[k][row] * other[col][k];
			}

			result[col][row] = sum;
		}
	}

	return result;
}

LibMath::Vector2 LibMath::operator*(Matrix3 m, Vector2 v)
{
	// Homogeneous coordinate multiplication (matrix * vector)
	float x = m[0][0] * v.getX() + m[1][0] * v.getY() + m[2][0] * 1; // Column-major access
	float y = m[0][1] * v.getX() + m[1][1] * v.getY() + m[2][1] * 1; // Column-major access

	return LibMath::Vector2(x, y);
}

LibMath::Vector3 LibMath::operator*(Matrix3 m, Vector3 v)
{
	// Matrix-vector multiplication using column-major storage
	float x = m[0][0] * v.m_x + m[1][0] * v.m_y + m[2][0] * v.m_z;  // Column-major access
	float y = m[0][1] * v.m_x + m[1][1] * v.m_y + m[2][1] * v.m_z;  // Column-major access
	float z = m[0][2] * v.m_x + m[1][2] * v.m_y + m[2][2] * v.m_z;  // Column-major access

	return Vector3(x, y, z);  // Return the result as a new Vector3
}

bool	LibMath::Matrix3::identity()
{

	// Check if the matrix matches the identity matrix for column-major storage
	if (m_values[0][0] == 1 && m_values[1][0] == 0 && m_values[2][0] == 0 &&  // First column
		m_values[0][1] == 0 && m_values[1][1] == 1 && m_values[2][1] == 0 &&  // Second column
		m_values[0][2] == 0 && m_values[1][2] == 0 && m_values[2][2] == 1)    // Third column
	{
		return true;
	}

	return false;
}

#pragma endregion All functions for Matrix3

#pragma region Matrix4

LibMath::Matrix4::Matrix4()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m_values[i][j] = 0;
		}
	}
}

LibMath::Matrix4::Matrix4(float value)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m_values[i][j] = value;
		}
	}
}

LibMath::Matrix4::Matrix4(const Matrix4& other)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m_values[i][j] = other.m_values[i][j];
		}
	}
}

LibMath::Matrix4::Matrix4(float nb1, float nb2, float nb3, float nb4, float nb5, float nb6, float nb7, float nb8, float nb9, float nb10, float nb11, float nb12, float nb13, float nb14, float nb15, float nb16)
{
	// Column-major storage: Assign values column by column
	m_values[0][0] = nb1;  m_values[1][0] = nb2;  m_values[2][0] = nb3;  m_values[3][0] = nb4;
	m_values[0][1] = nb5;  m_values[1][1] = nb6;  m_values[2][1] = nb7;  m_values[3][1] = nb8;
	m_values[0][2] = nb9;  m_values[1][2] = nb10; m_values[2][2] = nb11; m_values[3][2] = nb12;
	m_values[0][3] = nb13; m_values[1][3] = nb14; m_values[2][3] = nb15; m_values[3][3] = nb16;
}


float LibMath::Matrix4::operator[](int nb) const
{
	return m_values[nb / 4][nb % 4];
}


float* LibMath::Matrix4::operator[](int nb)
{
	return m_values[nb];
}


LibMath::Matrix4& LibMath::Matrix4::operator=(Matrix4 other)
{
	std::swap(m_values, other.m_values);

	return *this;
}

// Need to make it with 3 type of rotation
LibMath::Matrix4 LibMath::Matrix4::createTransform(Radian rad, Vector4 scale)
{
	Matrix4 transformMatrix;

	transformMatrix = Matrix4(1.0f);

	transformMatrix.m_values[0][0] = scale.m_x;
	transformMatrix.m_values[1][1] = scale.m_y;
	transformMatrix.m_values[2][2] = scale.m_z;
	transformMatrix.m_values[3][3] = scale.m_w;

	float cosRad = cos(rad.radian());
	float sinRad = sin(rad.radian());

	// rotation around the Z-axis
	transformMatrix.m_values[0][0] *= cosRad;
	transformMatrix.m_values[0][1] *= -sinRad;
	transformMatrix.m_values[1][0] *= sinRad;
	transformMatrix.m_values[1][1] *= cosRad;

	return transformMatrix;
}

LibMath::Matrix4 LibMath::Matrix4::createTransform(float x, float y, float z, float degree)
{
	Matrix4 transformMatrix;

	transformMatrix = Matrix4(1.0f);

	transformMatrix.m_values[0][0] = x;
	transformMatrix.m_values[1][1] = y;
	transformMatrix.m_values[2][2] = z;
	transformMatrix.m_values[3][3] = 1;

	float cosRad = cos(degree * (M_PI / 180));
	float sinRad = sin(degree * (M_PI / 180));

	// rotation around the Z-axis
	transformMatrix.m_values[0][0] *= cosRad;
	transformMatrix.m_values[0][1] *= -sinRad;
	transformMatrix.m_values[1][0] *= sinRad;
	transformMatrix.m_values[1][1] *= cosRad;

	return transformMatrix;

}

LibMath::Matrix4 LibMath::Matrix4::createRotation(Vector3 axis, Radian angle)
{
	axis.normalize();

	float cosTheta = cos(angle.raw());
	float sinTheta = sin(angle.raw());
	float k = 1.0f - cosTheta;

	float x = axis.m_x;
	float y = axis.m_y;
	float z = axis.m_z;

	return Matrix4(
		cosTheta + x * x * k, x * y * k + z * sinTheta, x * z * k - y * sinTheta, 0.0f,
		y * x * k - z * sinTheta, cosTheta + y * y * k, y * z * k + x * sinTheta, 0.0f,
		z * x * k + y * sinTheta, z * y * k - x * sinTheta, cosTheta + z * z * k, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

LibMath::Matrix4 LibMath::Matrix4::createTransform(Vector3 position, Vector3 axis, Radian angle, Vector3 scale)
{
	Matrix4 rotationMatrix = createRotation(axis, angle);

	return Matrix4(
		scale[0] * rotationMatrix[0][0], scale[0] * rotationMatrix[0][1], scale[0] * rotationMatrix[0][2], 0.f,
		scale[1] * rotationMatrix[1][0], scale[1] * rotationMatrix[1][1], scale[1] * rotationMatrix[1][2], 0.f,
		scale[2] * rotationMatrix[2][0], scale[2] * rotationMatrix[2][1], scale[2] * rotationMatrix[2][2], 0.f,
		position[0], position[1], position[2], 1.0f
	).transpose();
}

LibMath::Matrix4 LibMath::Matrix4::createTransform(LibMath::Vector3 translate, LibMath::Radian rotation, LibMath::Vector3 scale, LibMath::Vector3 axis)
{
	float cosR = cos(rotation.raw());
	float sinR = sin(rotation.raw());
	float k = 1 - cosR;

	// Axis components squared
	float pow2X = powf(axis.getX(), 2.f);
	float pow2Y = powf(axis.getY(), 2.f);
	float pow2Z = powf(axis.getZ(), 2.f);

	// Construct the transformation matrix in column-major order
	return Matrix4(
		// Column 1
		scale.getX() * ((pow2X * k) + cosR), scale.getY() * ((axis.getX() * axis.getY() * k) + (axis.getZ() * sinR)), scale.getZ() * ((axis.getX() * axis.getZ() * k) - (axis.getY() * sinR)), 0.f,

		// Column 2
		scale.getX() * ((axis.getX() * axis.getY() * k) - (axis.getZ() * sinR)), scale.getY() * ((pow2Y * k) + cosR), scale.getZ() * ((axis.getY() * axis.getZ() * k) + (axis.getX() * sinR)), 0.f,

		// Column 3
		scale.getX() * ((axis.getX() * axis.getZ() * k) + (axis.getY() * sinR)), scale.getY() * ((axis.getY() * axis.getZ() * k) - (axis.getX() * sinR)), scale.getZ() * ((pow2Z * k) + cosR), 0.f,

		// Column 4 (Translation)
		translate.getX(), translate.getY(), translate.getZ(), 1.f
	);
}

LibMath::Matrix4 LibMath::Matrix4::Transform(LibMath::Vector3 translate, LibMath::Radian rotation, LibMath::Vector3 scale, LibMath::Vector3 axis)
{
	float cosR = cos(rotation.raw());
	float sinR = sin(rotation.raw());

	float k = 1 - cosR;

	float pow2X = powf(axis.getX(), 2.f);
	float pow2Y = powf(axis.getY(), 2.f);
	float pow2Z = powf(axis.getZ(), 2.f);

	Matrix4 temp(
		// Column 1
		scale.getX() * ((pow2X * k) + cosR), scale.getY() * ((axis.getX() * axis.getY() * k) + (axis.getZ() * sinR)), scale.getZ() * ((axis.getX() * axis.getZ() * k) - (axis.getY() * sinR)), 0.f,

		// Column 2
		scale.getX() * ((axis.getX() * axis.getY() * k) - (axis.getZ() * sinR)), scale.getY() * ((pow2Y * k) + cosR), scale.getZ() * ((axis.getY() * axis.getZ() * k) + (axis.getX() * sinR)), 0.f,

		// Column 3
		scale.getX() * ((axis.getX() * axis.getZ() * k) + (axis.getY() * sinR)), scale.getY() * ((axis.getY() * axis.getZ() * k) - (axis.getX() * sinR)), scale.getZ() * ((pow2Z * k) + cosR), 0.f,

		// Column 4 (Translation)
		translate.getX(), translate.getY(), translate.getZ(), 1.f
	);

	// Apply the transformation (this * temp), resulting in the transformed matrix.
	*this = *this * temp;

	return temp;
}

LibMath::Matrix4 LibMath::Matrix4::createTranslation(Vector3 v)
{
	Matrix4 translationMatrix = Matrix4::getIdentity();

	// Set translation in last column for column-major
	translationMatrix.m_values[3][0] = v.m_x;
	translationMatrix.m_values[3][1] = v.m_y;
	translationMatrix.m_values[3][2] = v.m_z;

	return translationMatrix;
}

LibMath::Matrix4 LibMath::Matrix4::createRotationX(Vector4 origin, float rad)
{
	Matrix4 rotationMatrix = Matrix4::getIdentity();

	rotationMatrix.m_values[1][1] = cos(rad);     
	rotationMatrix.m_values[2][1] = -sin(rad);    
	rotationMatrix.m_values[1][2] = sin(rad);     
	rotationMatrix.m_values[2][2] = cos(rad);    

	rotationMatrix.m_values[3][0] = origin.m_x;   
	rotationMatrix.m_values[3][1] = origin.m_y;   
	rotationMatrix.m_values[3][2] = origin.m_z;   

	return rotationMatrix;
}

// Create rotation matrix around the Y-axis
LibMath::Matrix4 LibMath::Matrix4::createRotationY(float rad)
{
	Matrix4 rotationMatrix = Matrix4::getIdentity();

	rotationMatrix.m_values[0][0] = cos(rad);
	rotationMatrix.m_values[2][0] = sin(rad);
	rotationMatrix.m_values[0][2] = -sin(rad);
	rotationMatrix.m_values[2][2] = cos(rad);

	return rotationMatrix;
}

LibMath::Matrix4 LibMath::Matrix4::createRotationZ(Vector4 origin, float rad)
{
	Matrix4 rotationMatrix = Matrix4::getIdentity();

	rotationMatrix.m_values[0][0] = cos(rad);
	rotationMatrix.m_values[0][1] = -sin(rad);
	rotationMatrix.m_values[1][0] = sin(rad);
	rotationMatrix.m_values[1][1] = cos(rad);

	// Translation in last column (column-major)
	rotationMatrix.m_values[0][3] = origin.m_x;
	rotationMatrix.m_values[1][3] = origin.m_y;
	rotationMatrix.m_values[2][3] = origin.m_z;

	return rotationMatrix;
}

// Create scale matrix
LibMath::Matrix4 LibMath::Matrix4::createScale(Vector3 v)
{
	Matrix4 scaleMatrix;

	// Apply scaling to the diagonal of the matrix in column-major order
	scaleMatrix.m_values[0][0] = v.m_x;  // Scaling factor for X-axis
	scaleMatrix.m_values[1][1] = v.m_y;  // Scaling factor for Y-axis
	scaleMatrix.m_values[2][2] = v.m_z;  // Scaling factor for Z-axis
	scaleMatrix.m_values[3][3] = 1.0f;   // Homogeneous coordinate

	return scaleMatrix;
}

// Transpose the matrix
LibMath::Matrix4 LibMath::Matrix4::transpose()
{
	Matrix4 transposedMatrix;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			transposedMatrix.m_values[i][j] = m_values[j][i];
		}
	}
	return transposedMatrix;
}

float LibMath::Matrix4::determinant() const
{
	return m_values[0][0] * (m_values[1][1] * m_values[2][2] - m_values[1][2] * m_values[2][1]) -
		m_values[0][1] * (m_values[1][0] * m_values[2][2] - m_values[1][2] * m_values[2][0]) +
		m_values[0][2] * (m_values[1][0] * m_values[2][1] - m_values[1][1] * m_values[2][0]);
}

LibMath::Matrix4 LibMath::Matrix4::minors()
{
	Matrix4 minorsMatrix;

	// Calculate minors for each element of the 4x4 matrix
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			// Create a 3x3 submatrix by excluding row and column
			int subRow = 0;
			for (int i = 0; i < 4; ++i)
			{
				if (i == row) continue;  // Skip the row to remove
				int subCol = 0;

				for (int j = 0; j < 4; ++j)
				{
					if (j == col) continue;  // Skip the column to remove
					minorsMatrix[subRow][subCol] = m_values[i][j];
					subCol++;
				}
				subRow++;
			}

			// Calculate the determinant of the 3x3 submatrix
			minorsMatrix[row][col] = minorsMatrix.determinant();
		}
	}

	return minorsMatrix;
}

LibMath::Matrix4 LibMath::Matrix4::cofactors()
{
	Matrix4 cofactorsMatrix;
	Matrix4 minorsMatrix = minors();  // Get minors matrix

	// Calculate cofactors for each element
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			cofactorsMatrix[row][col] = minorsMatrix[row][col] * ((row + col) % 2 == 0 ? 1 : -1);
		}
	}

	return cofactorsMatrix;
}

LibMath::Matrix4 LibMath::Matrix4::adjugate()
{
	Matrix4 adjugateMatrix;
	Matrix4 cofactorsMatrix = cofactors();  // Get cofactors matrix

	// Calculate adjugate matrix (transpose of the cofactor matrix)
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			adjugateMatrix[row][col] = cofactorsMatrix[col][row];  // Transpose the cofactors
		}
	}

	return adjugateMatrix;
}


LibMath::Matrix4 LibMath::Matrix4::inverse()
{
	Matrix4 inverseMatrix;

	// Calculate determinant of the matrix
	float det = determinant();

	if (det == 0)
	{
		throw std::runtime_error("Matrix is singular and cannot be inverted.");
	}

	// Compute the inverse matrix (adjugate / determinant)
	inverseMatrix = adjugate() * (1.0f / det);

	return inverseMatrix;
}

bool LibMath::Matrix4::isIdentity()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if ((i == j && m_values[i][j] != 1.0f) || (i != j && m_values[i][j] != 0.0f))
			{
				return false;
			}
		}
	}

	return true;
}

LibMath::Matrix4 LibMath::Matrix4::getIdentity()
{
	return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f );
}

LibMath::Matrix4 LibMath::Matrix4::perspectiveProjection(float fovY, float aspectRatio, float near, float far)
{
	// Ensure valid input
	if (fovY <= 0 || aspectRatio <= 0 || near <= 0 || far <= 0 || near >= far)
	{
		throw std::invalid_argument("Invalid arguments for perspective projection.");
	}

	// Compute the tangent of half the vertical field of view
	float tanHalfFovY = std::tan(Radian(fovY / 2.0f).raw());

	// Initialize the perspective projection matrix
	Matrix4 result;

	// Set the elements of the perspective matrix
	result[0][0] = 1.0f / (aspectRatio * tanHalfFovY);
	result[1][1] = 1.0f / tanHalfFovY;
	result[2][2] = -(far + near) / (far - near);
	result[2][3] = -1.0f;
	result[3][2] = -(2.0f * far * near) / (far - near);
	result[3][3] = 0.0f;

	return result;
}

LibMath::Matrix4 LibMath::Matrix4::createViewMatrix(const LibMath::Vector3& eye,
	const LibMath::Vector3& target, const LibMath::Vector3& up)
{
	LibMath::Vector3 forward = (target - eye); // Z

	forward.normalize();

	LibMath::Vector3 right = up.cross(forward); // X

	right.normalize();

	LibMath::Vector3 camUp = forward.cross(right); // Y

	// Model matrix
	LibMath::Matrix4 viewMatrix(
		right.m_x, camUp.m_x, -forward.m_x, 0.0f,  // Column 1
		right.m_y, camUp.m_y, -forward.m_y, 0.0f,  // Column 2
		right.m_z, camUp.m_z, -forward.m_z, 0.0f,  // Column 3
		-right.dot(eye), -camUp.dot(eye), forward.dot(eye), 1.0f // Column 4
	);

	//viewMatrix = viewMatrix.inverse();

	return viewMatrix;
}

LibMath::Matrix4 LibMath::Matrix4::lookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	Vector3 f = (center - eye).normalize();
	Vector3 s = f.cross(up).normalize();
	Vector3 u = s.cross(f);

	Matrix4 result = Matrix4::getIdentity();

	result[0][0] = s.m_x; result[1][0] = s.m_y; result[2][0] = s.m_z;
	result[0][1] = u.m_x; result[1][1] = u.m_y; result[2][1] = u.m_z;
	result[0][2] = -f.m_x; result[1][2] = -f.m_y; result[2][2] = -f.m_z;

	result[3][0] = -s.dot(eye);
	result[3][1] = -u.dot(eye);
	result[3][2] = f.dot(eye);

	return result;
}


LibMath::Matrix4 LibMath::operator+(Matrix4 m, Matrix4 other)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] += other[i][j];
		}
	}

	return m;
}

LibMath::Vector4 LibMath::operator*(Matrix4 m, Vector4 v)
{
	Vector4 result;

	for (int i = 0; i < 4; ++i)
	{
		result[i] = m[i][0] * v.m_x + m[i][1] * v.m_y + m[i][2] * v.m_z + m[i][3] * v.m_w;
	}

	return result;
}

LibMath::Matrix4 LibMath::operator*(Matrix4 m, float nb)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] *= nb;
		}
	}

	return m;
}

LibMath::Matrix4 LibMath::operator*(Matrix4 m, Matrix4 other)
{
	Matrix4 result;

	for (int i = 0; i < 4; ++i)         // row of result
	{
		for (int j = 0; j < 4; ++j)     // column of result
		{
			result[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k)
			{
				result[i][j] += m[i][k] * other[k][j];
			}
		}
	}
	return result;
}

LibMath::Matrix4 LibMath::operator*=(Matrix4 m, Matrix4 other)
{
	Matrix4 result;

	for (int col = 0; col < 4; ++col)         // column of result
	{
		for (int row = 0; row < 4; ++row)     // row of result
		{
			result[col][row] = 0.0f;
			for (int k = 0; k < 4; ++k)
			{
				// Remember: m[col][row] means "column-major"
				result[col][row] += m[k][row] * other[col][k];
			}
		}
	}
	return result;
}

#pragma endregion All functions for Matrix4