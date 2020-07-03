#include "../include/revMatrix44.h"

revMatrix44::revMatrix44()
{
	Identity();
}

revMatrix44::revMatrix44(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44) : 
_11(m11), _12(m12), _13(m13), _14(m14), 
_21(m21), _22(m22), _23(m23), _24(m24), 
_31(m31), _32(m32), _33(m33), _34(m34), 
_41(m41), _42(m42), _43(m43), _44(m44)
{
}

revMatrix44::revMatrix44(float *afNum)
{
	for(int32 i = 0; i < 16; ++i){
		data[i] = afNum[i];
	}
}

revMatrix44::revMatrix44(const revMatrix44& m)
{
	*this = m;
}

revMatrix44::revMatrix44(const revVector3& x, const revVector3& y, const revVector3& z, const revVector3& w)
{
	m[0][0] = x.data[0];
	m[0][1] = x.data[1];
	m[0][2] = x.data[2];
	m[0][3] = 0.0f;

	m[1][0] = y.data[0];
	m[1][1] = y.data[1];
	m[1][2] = y.data[2];
	m[1][3] = 0.0f;

	m[2][0] = z.data[0];
	m[2][1] = z.data[1];
	m[2][2] = z.data[2];
	m[2][3] = 0.0f;

	m[3][0] = w.data[0];
	m[3][1] = w.data[1];
	m[3][2] = w.data[2];
	m[3][3] = 1.0f;
}

revMatrix44::revMatrix44(const revVector4& x, const revVector4& y, const revVector4& z, const revVector4& w)
{
	m[0][0] = x.data[0];
	m[0][1] = x.data[1];
	m[0][2] = x.data[2];
	m[0][3] = x.data[2];

	m[1][0] = y.data[0];
	m[1][1] = y.data[1];
	m[1][2] = y.data[2];
	m[1][3] = y.data[2];

	m[2][0] = z.data[0];
	m[2][1] = z.data[1];
	m[2][2] = z.data[2];
	m[2][3] = z.data[2];

	m[3][0] = w.data[0];
	m[3][1] = w.data[1];
	m[3][2] = w.data[2];
	m[3][3] = w.data[3];
}


revMatrix44::~revMatrix44()
{
}

revMatrix44 revMatrix44::Inverse()
{
	int i, j, k;
	float lu[20], *plu[4], det;
	float max, a;
	float *t;
	revMatrix44 s;
	revMatrix44 matrix;
	int count = 0;

	s = *this;

	for(j = 0; j < 4; ++j){
		*(plu[j] = lu + 5 * j) = s.data[count];
		++count;
		if(*(plu[j]) < 0.0f){
			max = *(plu[j]) * -1.0f;
		}
		else{
			max = *(plu[j]);
		}
		for(i = 0; ++i < 4;){
			plu[j][i] = s.data[count];
			++count;
			if(plu[j][i] < 0.0f){
				a = plu[j][i] * -1.0f;
			}
			else{
				a = plu[j][i];
			}
			if(a > max) max = a;
		}
		if(max == 0.0f){
			matrix.Identity();
			return matrix;
		}
		plu[j][4] = 1.0f / max;
	}

	det = 1.0;

	for(j = 0; j < 4; ++j){
		max = plu[j][j] * plu[j][4];
		if(max < 0.0f){
			max *= -1.0f;
		}
		i = j;
		for(k = j; ++k < 4;){
			a = plu[k][j] * plu[k][4];
			if(a < 0.0f){
				a *= -1.0f;
			}
			if(a > max) {
				max = a;
				i = k;
			}
		}
		if(i > j){
			t = plu[j];
			plu[j] = plu[i];
			plu[i] = t;
			det = -det;
		}
		if(plu[j][j] == 0.0){
			matrix.Identity();
			return matrix;
		}
		det *= plu[j][j];
		for(k = j; ++k < 4;) {
			plu[k][j] /= plu[j][j];
			for(i = j; ++i < 4;) {
				plu[k][i] -= plu[j][i] * plu[k][j];
			}
		}
	}

	for(k = 0; k < 4; ++k){
		for(i = 0; i < 4; ++i){
			matrix.data[i * 4 + k] = (plu[i] == lu + k * 5) ? 1.0f : 0.0f;
		}
		for(i = 0; i < 4; ++i){
			for(j = i; ++j < 4;){
				matrix.data[j * 4 + k] -= matrix.data[i * 4 + k] * plu[j][i];
			}
		}
		for(i = 4; --i >= 0;){
			for(j = i; ++j < 4;){
				matrix.data[i * 4 + k] -= plu[i][j] * matrix.data[j * 4 + k];
			}
			matrix.data[i * 4 + k] /= plu[i][i];
		}
	}

	return matrix;
}



void revMatrix44::CreateLookTo(const revVector3& eye, const revVector3& eyeDir, const revVector3& upVec)
{
	revVector3 z = revVector3::Normalize(eyeDir);
	revVector3 x = revVector3::Normalize(revVector3::Cross(upVec, z));
	revVector3 y = revVector3::Cross(z, x);

	revMatrix44 o(x, y, z, revVector3::Zero());
	revMatrix44 t;
	t.Translation(-1.0f * eye);

	*this = o * t;
}

void revMatrix44::CreateLookAtMatrixLH(const revVector3& eye, const revVector3& lookat, const revVector3& upvec)
{
	CreateLookTo(eye, lookat - eye, upvec);
}

void revMatrix44::CreateLookAtMatrixRH(const revVector3& eye, const revVector3& lookat, const revVector3& upvec)
{
	CreateLookTo(eye, eye - lookat, upvec);
}



void revMatrix44::CreatePerspectiveMatrixRH( float fov_radian,  float aspect,  float near,  float far)
{
	Identity();

	

}

void revMatrix44::CreatePerspectiveMatrixLH( float fov_radian,  float aspect,  float near,  float far)
{
	Identity();
	float fTemp = (1.0f / tanf(fov_radian / 2.0f));
	float near_to_far = near - far;

	_11 = fTemp / aspect;
	_22 = fTemp;
	_33 = -(far) / near_to_far;
	_34 = 1.0f;
	_43 = (far * near) / near_to_far;
	_44 =  0.0f;

}

void revMatrix44::CreateViewPortMatrixRH(int width, int height, int near, int far)
{
	Identity();
	_11 = (float)width/2;
	_41 = -_11;

	_22 = -(float)height/2;
	_42 = -_22;

	_33 = (float)far - near;
	_43 = (float)near;

}