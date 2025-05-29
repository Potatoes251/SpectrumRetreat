#ifndef LIBMATH_ARITHMETIC_H_
#define LIBMATH_ARITHMETIC_H_

namespace LibMath
{
	bool almostEqual(float x, float y);				// Return if two floating value are similar enought to be considered equal

	float ceiling(float x);							// Return lowest integer value higher or equal to parameter
	float clamp(float value, float min, float max);	// Return parameter limited by the given range
	float floor(float x);							// Return highest integer value lower or equal to parameter
	float squareRoot(float x);						// Return square root of parameter
	float wrap(float value, float min, float max);	// Return parameter as value inside the given range
}

#endif // !LIBMATH_ARITHMETIC_H_
