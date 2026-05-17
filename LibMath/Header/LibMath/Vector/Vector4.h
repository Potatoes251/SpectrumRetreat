#ifndef LIBMATH_VECTOR_VECTOR4_H_
#define LIBMATH_VECTOR_VECTOR4_H_

namespace LibMath
{
	class Vector4
	{
	public:
						Vector4() : m_x(0), m_y(0), m_z(0), m_w(1) {};														// set all component to 0
		explicit		Vector4(float nb) : m_x(nb), m_y(nb), m_z(nb), m_w(nb) {}											// set all component to the same value
						Vector4(float nb1, float nb2, float nb3, float nb4) : m_x(nb1), m_y(nb2), m_z(nb3), m_w(nb4) {}		// set all component individually
						Vector4(Vector4 const& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z), m_w(other.m_w) {} 	// copy all component
						Vector4(Vector3 const& other, float w) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z), m_w(w) {}
						~Vector4() {};

		Vector4&		operator=(Vector4 const& other);

		float&			operator[](int index);								// return this vector component value
		float			operator[](int index) const;						// return this vector component value

		float			dotProduct(Vector4 const& other) const;				// return dot product result

		float			magnitude() const;									// return vector magnitude
		float			magnitudeSquared() const;							// return square value of the vector magnitude

		void			homogenize();

		std::string		string() const;									// return a string representation of this vector

		float m_x;
		float m_y;
		float m_z;
		float m_w;
	};

	bool		operator==(Vector4 const& v1, Vector4 const& v2);		
	bool		operator!=(Vector4 const& v1, Vector4 const& v2);		

	Vector4		operator+(Vector4 v1, Vector4 const& v2);				
	Vector4		operator-(Vector4 v1, Vector4 const& v2);				
	Vector4		operator*(Vector4 v1, Vector4 const& v2);				
	Vector4		operator/(Vector4 v1, Vector4 const& v2);				

	Vector4&	operator+=(Vector4& v1, Vector4 const& v2);				// addition component wise
	Vector4&	operator-=(Vector4& v1, Vector4 const& v2);				// subtraction component wise
	Vector4&	operator*=(Vector4& v1, Vector4 const& v2);				// multiplication component wise
	Vector4&	operator/=(Vector4& v1, Vector4 const& v2);				// division component wise

	Vector4		operator*(Vector4 const& v, float nb);

	std::ostream& operator<<(std::ostream&, Vector4 const&);
}

#ifdef LIBMATH_MATRIX_MATRIX4_H_
//#include "Matrix4Vector4Operation.h"
#endif // LIBMATH_MATRIX_MATRIX4_H_

#endif // !LIBMATH_VECTOR_VECTOR4_H_
