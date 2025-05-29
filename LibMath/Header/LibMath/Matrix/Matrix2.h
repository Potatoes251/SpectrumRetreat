#ifndef LIBMATH_MATRIX_MATRIX2_H_
#define LIBMATH_MATRIX_MATRIX2_H_

#include "../Angle.h"
#include "../Vector.h"
#include "../Object2D.h"

namespace LibMath
{
	class Matrix2
	{
	public:

		Matrix2();
		Matrix2(float value);
		Matrix2(float nb1, float nb2, float nb3, float nb4);
		Matrix2(const Matrix2& other);
		~Matrix2() = default;

		float   operator[](int nb) const;
		float*	operator[](int nb);

		Matrix2& operator=(Matrix2 other);

		Matrix2		transpose();

		float		determinant() const;

		Matrix2		minors();
		Matrix2		cofactors();
		Matrix2		adjugate();
		Matrix2		inverse();
		Matrix2		createTransform(LibMath::Radian rad = 0_rad, LibMath::Vector2 scale = Vector2(1,1));
		Matrix2		createRotation(float rad);
		Matrix2		createScale(LibMath::Vector2 v);

		bool		identity();

	private:

		float m_values[2][2];
	};

	Matrix2		operator+(Matrix2 m, Matrix2 other);
	Matrix2		operator*(Matrix2 m, float nb);
	Matrix2		operator*(Matrix2 m, Matrix2 other);
	Vector2		operator*(Matrix2 m, Vector2 v);
}

#endif // !LIBMATH_MATRIX_MATRIX2_H_