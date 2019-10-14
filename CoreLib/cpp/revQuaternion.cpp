#include "../include/revQuaternion.h"

const revQuaternion revQuaternion::ZERO = revQuaternion(0.0f, 0.0f, 0.0f, 0.0f);

revMatrix44 revQuaternion::CreateRotationMatrix()
{
	float data[16];

	float x2 = x * x * 2.0f;
	float y2 = y * y * 2.0f;
	float z2 = z * z * 2.0f;
	float xy = x * y * 2.0f;
	float yz = y * z * 2.0f;
	float zx = z * x * 2.0f;
	float xw = x * w * 2.0f;
	float yw = y * w * 2.0f;
	float zw = z * w * 2.0f;

	data[ 0] = 1.0f - y2 - z2;
	data[ 1] = xy + zw;
	data[ 2] = zx - yw;
	data[ 3] = 0.0f;
	data[ 4] = xy - zw;
	data[ 5] = 1.0f - z2 - x2;
	data[ 6] = yz + xw;
	data[ 7] = 0.0f;
	data[ 8] = zx + yw;
	data[ 9] = yz - xw;
	data[10] = 1.0f - x2 - y2;
	data[11] = 0.0f;
	data[12] = 0.0f;
	data[13] = 0.0f;
	data[14] = 0.0f;
	data[15] = 1.0f;

	return revMatrix44(data);
}

