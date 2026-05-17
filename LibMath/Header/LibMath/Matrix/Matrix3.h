#ifndef LIBMATH_MATRIX_MATRIX3_H_
#define LIBMATH_MATRIX_MATRIX3_H_

#include "../Vector/Vector2.h"
#include "../Object2D.h"

namespace LibMath
{
	class Matrix3
	{
	public:

		Matrix3();
		Matrix3(float value);
		Matrix3(float nb1, float nb2, float nb3, float nb4, float nb5, float nb6, float nb7, float nb8, float nb9);
		Matrix3(const Matrix3& other);
		~Matrix3() = default;

		float   operator[](int nb) const;
		float* operator[](int nb);

		Matrix3& operator=(Matrix3 other);

		Matrix3		createTransform2D(Radian rad = 0_rad, Vector2 scale = Vector2(1, 1));
		Matrix3		createTranslation2D(Vector2 v);
		Matrix3		createTranslation3D(Vector3 v);

		Matrix3		createRotation(Point2D point, float rad);

		Matrix3		createRotationX(Vector3 origin, float rad);
		Matrix3		createRotationY(Vector3 origin, float rad);
		Matrix3		createRotationZ(Vector3 origin, float rad);

		Matrix3		createScale2D(Vector2 v);
		Matrix3		createScale3D(Vector3 v);

		Matrix3		transpose();

		float		determinant() const;

		Matrix3		minors();
		Matrix3		cofactors();
		Matrix3		adjugate();
		Matrix3		inverse();

		bool		identity();

	private:

		float m_values[3][3];
	};

	Matrix3	operator+(Matrix3 m, Matrix3 other);
	Vector2	operator*(Matrix3 m, Vector2 v);
	Vector3	operator*(Matrix3 m, Vector3 v);
	Matrix3	operator*(Matrix3 m, float nb);
	Matrix3	operator*(Matrix3 m, Matrix3 other);

}

#endif // !LIBMATH_MATRIX_MATRIX3_H_
