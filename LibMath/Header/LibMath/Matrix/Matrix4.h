#ifndef LIBMATH_MATRIX_MATRIX4_H_
#define LIBMATH_MATRIX_MATRIX4_H_

#include "../Vector/Vector4.h"
namespace LibMath
{
	class Matrix4
	{
	public:

		Matrix4();
		Matrix4(float value);
		Matrix4(float nb1, float nb2, float nb3, float nb4, 
			    float nb5, float nb6, float nb7, float nb8, 
			    float nb9, float nb10, float nb11, float nb12,
				float nb13, float nb14, float nb15, float nb16);

		Matrix4(const Matrix4& other);
		~Matrix4() = default;

		float		operator[](int nb) const;
		float*		operator[](int nb);

		Matrix4&	operator=(Matrix4 other);

		static Matrix4	    createRotation(Vector3 axis, Radian angle);
		static Matrix4		createTransform(Vector3 position, Vector3 axis, Radian angle, Vector3 scale);
		static Matrix4		createTransform(Radian rad, Vector4 scale);
		static Matrix4		createTransform(float x, float y, float z, float degree);
		static Matrix4		createTransform(LibMath::Vector3 translate, LibMath::Radian rotation, LibMath::Vector3 scale, LibMath::Vector3 axis);
			   Matrix4      Transform(LibMath::Vector3 translate, LibMath::Radian rotation, LibMath::Vector3 scale, LibMath::Vector3 axis);
		static Matrix4		createTranslation(Vector3 v);

		static Matrix4		createRotationX(Vector4 origin, float rad);
		static Matrix4		createRotationY(float rad);
		static Matrix4		createRotationZ(Vector4 origin, float rad);

		static Matrix4		createScale(Vector3 v);

		static Matrix4     perspectiveProjection(float fovy, float aspect, float near, float far);
		static LibMath::Matrix4 createViewMatrix(const LibMath::Vector3& eye, const LibMath::Vector3& target, const LibMath::Vector3& up);
		

		static Matrix4		lookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

		Matrix4		transpose();

		float		determinant() const;

		Matrix4		minors();
		Matrix4		cofactors();
		Matrix4		adjugate();
		Matrix4		inverse();

		bool		isIdentity();
		static		Matrix4 getIdentity();

	//private:

		float m_values[4][4];
	};

	Matrix4	operator+(Matrix4 m, Matrix4 other);
	Vector4	operator*(Matrix4 m, Vector4 v);
	Matrix4	operator*(Matrix4 m, float nb);
	Matrix4	operator*(Matrix4 m, Matrix4 other);
	Matrix4 operator*=(Matrix4 m, Matrix4 other);
}

#ifdef LIBMATH_VECTOR_VECTOR4_H_
//#include "../Matrix4Vector4Operation.h"
#endif // LIBMATH_MATRIX_H_

#endif // !LIBMATH_MATRIX_MATRIX4_H_
