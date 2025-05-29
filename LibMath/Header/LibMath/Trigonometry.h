#ifndef LIBMATH_TRIGONOMETRY_H_
#define LIBMATH_TRIGONOMETRY_H_

#include "Angle/Radian.h"

namespace LibMath
{
	float	sin(Radian x);			// float result = sin(Radian{0.5});		// 0.479426
	float	cos(Radian x);			// float result = sin(Degree{45});		// 0.707107			// this make use implicit conversion
	float	tan(Radian x);			// float result = sin(0.5_rad);			// 0.479426			// this make use user defined litteral
	Radian	asin(float x);			// Radian angle = asin(0.479426);		// Radian{0.500001}
	Radian	acos(float x);			// Degree angle = acos(0.707107);		// Degree{44.99998}	// this make use implicit conversion
	Radian	atan(float x);			// Radian angle = atan(0.546302);		// Radian{0.500000}
	Radian	atan(float x, float y); // Radian angle = atan(1, -2);			// Radian{2.677945}
}

#endif // !LIBMATH_TRIGONOMETRY_H_
