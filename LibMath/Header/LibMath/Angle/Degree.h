#ifndef LIBMATH_ANGLE_DEGREE_H_
#define LIBMATH_ANGLE_DEGREE_H_

namespace LibMath
{
	class Radian;

	class Degree
	{
	public:
					Degree() : m_value(0.0f) {}
		explicit	Degree(float value) : m_value(value) {}				// explicit so no ambiguous / implicit conversion from float to angle can happen
					Degree(Degree const& other) : m_value(other.m_value) {}
					~Degree() = default;

		operator	LibMath::Radian() const;		// Radian angle = Degree{45};		// implicit conversion from Degree to Radian

		Degree&		operator=(Degree other);
		Degree&		operator+=(Degree other);		// Degree angle += Degree{45};
		Degree&		operator-=(Degree other);		// Degree angle -= Degree{45};
		Degree&		operator*=(float x);			// Degree angle *= 3;
		Degree&		operator/=(float x);			// Degree angle /= 3;

		void		wrap(bool value = false);			// true -> limit m_value to range [-180, 180[	// false -> limit m_value to range [0, 360[

		float		degree(bool value = false);	// return angle in degree // true -> return value in range [-180, 180]   //false -> return value in range [0, 360]
		float		radian(bool value = true);			// return angle in radian // true -> return value in range [-pi, pi]  // false -> return in range [0, 2 pi]	
		float		range_to_wrap(float _value, float min, float max);
		float		raw() const { return m_value; };	// return m_angle for the test

		void		set(float value);

	private:
		float m_value;
	};

	bool	operator==(Degree x, Degree y);			// bool isEqual = Degree{45} == Degree{45};		// true
	bool	operator==(Degree x, Radian y);	// bool isEqual = Degree{60} == Radian{0.5};	// false

	Degree	operator-(Degree x);			    // Degree angle = - Degree{45};					// Degree{-45}

	Degree	operator+(Degree x, Degree y);			// Degree angle = Degree{45} + Degree{45};		// Degree{90}
	Degree	operator-(Degree x, Degree y);			// Degree angle = Degree{45} - Degree{45};		// Degree{0}
	Degree	operator*(Degree x, float y);			// Degree angle = Degree{45} * 3;				// Degree{135}
	Degree	operator/(Degree x, float y);			// Degree angle = Degree{45} / 3;				// Degree{15}

	inline namespace Literal
	{
		inline LibMath::Degree operator""_deg(long double x) { return Degree{ static_cast<float>(x) }; }				// Degree angle = 7.5_deg;
		inline LibMath::Degree operator""_deg(unsigned long long int x) { return Degree{ static_cast<float>(x)}; }		// Degree angle = 45_deg;
	}
}

#endif // !LIBMATH_ANGLE_DEGREE_H_
