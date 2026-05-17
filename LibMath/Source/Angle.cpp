#define _USE_MATH_DEFINES
#include <cmath>
#include "./LibMath/Angle.h"

#pragma region Degree
LibMath::Degree::operator LibMath::Radian() const
{
	return Radian(m_value * ((float)M_PI / 180.0f));
}

LibMath::Degree&	LibMath::Degree::operator=(Degree other)
{
	m_value = other.m_value;

	return *this;
}

LibMath::Degree&	LibMath::Degree::operator+=(Degree other)
{
	m_value += other.m_value;

	return *this;
}

LibMath::Degree&	LibMath::Degree::operator-=(Degree other)
{
	m_value -= other.m_value;

	return *this;
}

LibMath::Degree&	LibMath::Degree::operator*=(float x)
{
	m_value *= x;

	return *this;
}

LibMath::Degree&	LibMath::Degree::operator/=(float x)
{
	m_value /= x;

	return *this;
}

void	LibMath::Degree::wrap(bool value)
{
	if (value)
	{
		m_value = range_to_wrap(m_value, -180.0f, 180.0f);
	}
	else
	{
		m_value = range_to_wrap(m_value, 0.0f, 360.0f);
	}
}

float	LibMath::Degree::degree(bool value)
{
	float deg = m_value;

	if (value)
	{
		return range_to_wrap(deg, -180.0f, 180.0f);
	}

	return range_to_wrap(deg, 0.0f, 360.0f);
}

float	LibMath::Degree::radian(bool value) 
{
	float radians = m_value * ((float)M_PI / 180.0f);

	if (value)
	{
		return range_to_wrap(radians, (float)-M_PI, (float)M_PI);
	}

	return range_to_wrap(radians, 0.0f, 2 * (float)M_PI);
}

float LibMath::Degree::range_to_wrap(float value, float min, float max)
{
	float range = max - min;

	while (value < min)
	{ // increase until in interval
		value += range;
	}

	while (value >= max)
	{ // decrease until in interval
		value -= range;
	}

	return value;
}

void LibMath::Degree::set(float value)
{
	m_value = value;
}

bool	LibMath::operator==(LibMath::Degree x, LibMath::Degree y)
{
	float tempX = x.range_to_wrap(x.raw(), 0, 360);
	float tempY = y.range_to_wrap(y.raw(), 0, 360);

	return tempX == tempY;
}

bool	LibMath::operator==(LibMath::Degree x, LibMath::Radian y)
{
	return x.radian() == y.radian();
}

LibMath::Degree LibMath::operator-(LibMath::Degree x)
{
	return LibMath::Degree{ -x.raw() };
}

LibMath::Degree LibMath::operator+(LibMath::Degree x, LibMath::Degree y)
{
	return LibMath::Degree{ x.raw() + y.raw() };
}

LibMath::Degree LibMath::operator-(LibMath::Degree x, LibMath::Degree y)
{
	return LibMath::Degree{ x.raw() - y.raw() };
}

LibMath::Degree LibMath::operator*(LibMath::Degree x, float y)
{
	return LibMath::Degree{ x.raw() * y };
}

LibMath::Degree LibMath::operator/(LibMath::Degree x, float y)
{
	return LibMath::Degree{ x.raw() / y };
}

#pragma endregion All functions for Degree

#pragma region Radian
LibMath::Radian::operator LibMath::Degree() const
{
	return Degree(m_value * 180.0F / (float)M_PI);
}

LibMath::Radian& LibMath::Radian::operator=(Radian other)
{
	m_value = other.m_value;

	return *this;
}

LibMath::Radian& LibMath::Radian::operator+=(Radian other)
{
	m_value += other.m_value;

	return *this;
}

LibMath::Radian& LibMath::Radian::operator-=(Radian other)
{
	m_value -= other.m_value;

	return *this;
}

LibMath::Radian& LibMath::Radian::operator*=(float x)
{
	m_value *= x;

	return *this;
}

LibMath::Radian& LibMath::Radian::operator/=(float x)
{
	m_value /= x;

	return *this;
}

void LibMath::Radian::wrap(bool value)
{
	if (value)
	{
		m_value = range_to_wrap(m_value, (float)-M_PI, (float)M_PI);
	}
	else
	{
		m_value = range_to_wrap(m_value, 0.0f, (float)M_PI * 2.0f);
	}
}

float LibMath::Radian::degree(bool value) 
{
	float deg = m_value * 180.0f / (float)M_PI;

	if (value)
	{
		return range_to_wrap(deg, -180.0f, 180.0f);
	}

	return range_to_wrap(deg, 0.0f, 360.0f);
}

float LibMath::Radian::radian(bool value)
{
	float rad = m_value;

	if (value)
	{
		return range_to_wrap(rad, (float)-M_PI, (float)M_PI);
	}

	return range_to_wrap(rad, 0.0f, (float)M_PI * 2);
}

float LibMath::Radian::range_to_wrap(float value, float min, float max)
{
	float range = max - min;

	while (value < min)
	{ // increase until in interval
		value += range;
	}

	while (value >= max)
	{ // decrease until in interval
		value -= range;
	}

	return value;
}

float LibMath::Radian::raw() const
{
	return m_value;
}

bool LibMath::operator==(LibMath::Radian x, LibMath::Radian y)
{
	float tempX = x.range_to_wrap(x.raw(), 0.0f, (float)M_PI * 2.0f);
	float tempY = y.range_to_wrap(y.raw(), 0.0f, (float)M_PI * 2.0f);

	return (tempX - tempY < 0.000001 && tempX - tempY > -0.000001);	// Approx
}

bool LibMath::operator==(LibMath::Radian x, LibMath::Degree y)
{
	return x == static_cast<Radian>(y);
}

LibMath::Radian LibMath::operator-(LibMath::Radian x)
{
	return Radian{ -x.raw() };
}

LibMath::Radian LibMath::operator+(LibMath::Radian x, LibMath::Radian y)
{
	return Radian{ x.raw() + y.raw() };
}

LibMath::Radian LibMath::operator-(LibMath::Radian x, LibMath::Radian y)
{
	return Radian{ x.raw() - y.raw() };
}

LibMath::Radian LibMath::operator*(LibMath::Radian x, float y)
{
	return Radian{ x.raw() * y };
}

LibMath::Radian LibMath::operator/(LibMath::Radian x, float y)
{
	return Radian{ x.raw() / y };
}

#pragma endregion All functions for Radian