#define _USE_MATH_DEFINES
#include <cmath>
#include "./LibMath/Arithmetic.h"

// Function to check if two floating-point numbers are almost equal
bool LibMath::almostEqual(float x, float y)
{
	const float epsilon = 1e-6f; // Change this value based on wanted precision

	return std::fabs(x - y) < epsilon;
}

// Function to get the ceiling of a value (smallest integer >= value)
float LibMath::ceiling(float x)
{
	return std::ceil(x);
}

// Function to clamp a value between a minimum and maximum range
float LibMath::clamp(float value, float min, float max)
{
	return std::fmax(min, std::fmin(value, max));
}

// Function to get the floor of a value (largest integer <= value)
float LibMath::floor(float x)
{
	return std::floor(x);
}

// Function to return the square root of a value
float LibMath::squareRoot(float x)
{
	if (x < 0.0f)
	{
		//throw std::logic_error("Cannot take the square root of a negative number.");
	}
	return std::sqrt(x); // Standard library sqrt function
}

// Function to wrap a value inside a specified range 
float LibMath::wrap(float value, float min, float max)
{
	if (min >= max)
	{
		//throw std::logic_error("Invalid range: min must be less than max.");
	}

	float range = max - min;
	while (value < min)
	{
		value += range;
	}
		
	while (value >= max)
	{
		value -= range;
	}
		
	return value; 
}