#include "LibMath/Matrix4Vector4Operation.h"

LibMath::Vector4 LibMath::operator*(Matrix4& m , Vector4& vec)
{
	return LibMath::Vector4(
		(m[0][0] * vec[0]) + (m[0][1] * vec[1]) + (m[0][2] * vec[2]) + (m[0][3] * vec[3]),
		(m[1][0] * vec[0]) + (m[1][1] * vec[1]) + (m[1][2] * vec[2]) + (m[1][3] * vec[3]),
		(m[2][0] * vec[0]) + (m[2][1] * vec[1]) + (m[2][2] * vec[2]) + (m[2][3] * vec[3]),
		(m[3][0] * vec[0]) + (m[3][1] * vec[1]) + (m[3][2] * vec[2]) + (m[3][3] * vec[3])
	);
}
