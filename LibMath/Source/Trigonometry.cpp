#define _USE_MATH_DEFINES
#include <cmath>
#include "./LibMath/Trigonometry.h"

float LibMath::sin(Radian x)
{
	return std::sinf(x.radian());
}

float LibMath::cos(Radian x)
{
	return std::cosf(x.radian());
}

float LibMath::tan(Radian x)
{
    return std::tanf(x.radian()); 
}

LibMath::Radian LibMath::asin(float x)
{
    // Ensure the value is in the valid range for asin
    if (x < -1.0f || x > 1.0f)
    {
        //throw std::logic_error("Input value out of range for asin.");
    }
    return Radian(std::asinf(x));
}

LibMath::Radian LibMath::acos(float x)
{
    // Ensure the value is in the valid range for acos
    if (x < -1.0f || x > 1.0f)
    {
        //throw std::logic_error("Input value out of range for acos.");
    }
    return Radian(std::acosf(x));
}

LibMath::Radian LibMath::atan(float x)
{
    return Radian(std::atanf(x));
}

LibMath::Radian LibMath::atan(float x, float y)
{
    return Radian(std::atan2f(y, x));
}