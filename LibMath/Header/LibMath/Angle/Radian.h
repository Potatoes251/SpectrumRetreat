#ifndef LIBMATH_ANGLE_RADIAN_H_
#define LIBMATH_ANGLE_RADIAN_H_

namespace LibMath
{
	class Degree;

	class Radian
	{
	public:
					Radian() : m_value(0.0f) {}
		explicit	Radian(float value) : m_value(value) {}				// explicit so no ambiguous / implicit conversion from float to angle can happen
					Radian(Radian const& other) : m_value(other.m_value) {}
					~Radian() = default;

		operator	LibMath::Degree() const;				// Degree angle = Radian{0.5};		// implicit conversion from Radian to Degree

		Radian&		operator=(Radian other);
		Radian&		operator+=(Radian other);		// Radian angle += Radian{0.5};
		Radian&		operator-=(Radian other);		// Radian angle -= Radian{0.5};
		Radian&		operator*=(float x);			// Radian angle *= 3;
		Radian&		operator/=(float x);			// Radian angle /= 3;

		void		wrap(bool value = false);			// true -> limit m_value to range [-pi, pi[		// false -> limit m_value to range [0, 2 pi[

		float		degree(bool value = false);	// return angle in degree	// true -> return value in range [-180, 180[	// false -> return value in range [0, 360[
		float		radian(bool value = true);	// return angle in radian	// true -> return value in range [-pi, pi[		// false -> return value in range [0, 2 pi[
		float       range_to_wrap(float value, float min, float max);
		float		raw() const;				// return m_angle

	private:
		float m_value;
	};

	bool	operator==(Radian x, Radian y);			// bool isEqual = Radian{0.5} == Radian{0.5};	// true
	bool	operator==(Radian x, Degree y);	        // bool isEqual = Radian{0.5} == Degree{60};	// false

	Radian	operator-(Radian x);					// Degree angle = - Radian{0.5};				// Radian{-0.5}

	Radian	operator+(Radian x, Radian y);			// Radian angle = Radian{0.5} + Radian{0.5};	// Radian{1}
	Radian	operator-(Radian x, Radian y);			// Radian angle = Radian{0.5} - Radian{0.5};	// Radian{0}
	Radian	operator*(Radian x, float y);			// Radian angle = Radian{0.5} * 3;				// Radian{1.5}
	Radian	operator/(Radian x, float y);			// Radian angle = Radian{0.5} / 3;				// Radian{0.166...}

	inline namespace Literal
	{
		inline LibMath::Radian operator""_rad(long double x) { return Radian{ static_cast<float>(x) }; }			// Radian angle = 0.5_rad;
		inline LibMath::Radian operator""_rad(unsigned long long int x) { return Radian{ static_cast<float>(x) }; }	// Radian angle = 1_rad;
	}
}


#endif // !LIBMATH_ANGLE_RADIAN_H_
