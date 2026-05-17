#ifndef LIBMATH_VECTOR_VECTOR2_H_
#define LIBMATH_VECTOR_VECTOR2_H_

namespace LibMath
{
	class Vector2
	{
	public:
		Vector2();
		Vector2(float nb);
		Vector2(float x, float y);
		Vector2(const Vector2& other);
		~Vector2() = default;

		float	getX() const;
		float	getY() const;
		
		float	magnitude() const;
		float   magnitudeSquare() const;
		bool	isUnit() const;
		float   dotProduct(const Vector2 v);
		float   crossProduct(const Vector2 v);
		float   angleBetween(Vector2 other);

		float	operator[](int nb) const;
		float&	operator[](int nb);

		Vector2 projectOnto(Vector2 other);
		Vector2 reflectOnto(Vector2 other);
		Vector2 normalize();

		Vector2& operator=(Vector2 other);

		float m_x;
		float m_y;
	};

	bool	operator==(Vector2 v1, Vector2 v2);

	Vector2	operator-(Vector2 v);			

	Vector2	operator+(Vector2 v1, Vector2 v2);		
	Vector2	operator-(Vector2 v1, Vector2 v2);		
	Vector2	operator*(Vector2 v, float nb);			
	Vector2	operator/(Vector2 v, float nb);			
}




#endif // !LIBMATH_VECTOR_VECTOR2_H_