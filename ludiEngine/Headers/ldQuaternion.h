/******************************************************************************************
*                                      QUATERNION                                         *
*                                                                                         *
* DESCRIPTION: Implements an unit quaternion ...										  *
*                                                                                         *
* AUTHOR: Pedro Pires <pedro.pires@acm.org>                                               *
*                                                                                         *
* COPYRIGHT: Copyright 2001, Pedro Pires & Diogo Andrade. All rights reserved.            *
******************************************************************************************/

#ifndef __PP_STORM_QUATERNIONS__
#define __PP_STORM_QUATERNIONS__

/**
 *  Represents a unit quaternion.
 */

class Quaternion
{
public:
	Quaternion();											///< Builds an uninicialized quaternion
	Quaternion(float _x, float _y, float _z, float _w);		///< Builds and inicializes a quaternion

	void operator+=(const Quaternion& q);					///< Adds quaternion q to this quaternion
	void operator-=(const Quaternion& q);					///< Subtracts quaternion q from this quaternion

	void add(const Quaternion& q, const Quaternion& r);		///< This quaternion will be set to the sum q + r
	void sub(const Quaternion& q, const Quaternion& r);		///< This quaternion will be set to the subtraction q - r
	void mult(const Quaternion& q, const Quaternion& r);	///< This quaternion will be set to the product qr

	void conj();											///< This quaternion will be set to its conjugate
	void conj(const Quaternion& q);							///< This quaternion will be set to the conjugate of q

	void invert();											///< This quaternion will be inverted
	void invert(const Quaternion& q);						///< This quaternion will be set to the inverse of q

	void unit_invert();										///< This is assumed to be a unit quaternion and it will be inverted (deprecated - same as invert)
	void unit_invert(const Quaternion& q);					///< This quaternion will be set to the inverse of unit quaternion q (deprecated - same as invert)

	float norm();											///< Returns the norm (there is an bug in this function)

	void identity_quaternion();								///< This quaternion will be set to the identity quaternion

	void rotation_quaternion(float _x, float _y, float _z, float ang);	///< This quaternion will be set to a quaternion representing a rotation of ang degrees (*not radians*) aroung vector (x,y,z)
	void rotation_quaternion(const Vec3f& vec, float ang);				///< This quaternion will be set to a quaternion representing a rotation of ang degrees (*not radians*) aroung vector vec

	void euler_quaternion(float yaw, float pitch, float roll);		///< This quaternion will be set to a quaternion representing the orientation expressen by the euler angles in degrees (it assumes OpenGL's standard refernce frame)

	void rotate_point(Vec3f& dst, const Vec3f& org);				///< The rotation encoded in the quaternion will be applied to org and written to dst
	void slerp(const Quaternion& to, float t, Quaternion& res);		///< Interpolates betwwen this quaternion and quaternion to. t shoud be beeetween 0 and 1.
	
public:
	float x, y, z, w;
};


inline Quaternion::Quaternion()
{
}

inline Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

inline void Quaternion::identity_quaternion()
{
	x = y = z = 0;
	w = 1;
}


inline void Quaternion::operator+=(const Quaternion& q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
}

inline void Quaternion::operator-=(const Quaternion& q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
}

inline void Quaternion::add(const Quaternion& q, const Quaternion& r)
{
	x = q.x + r.x;
	y = q.y + r.y;
	z = q.z + r.z;
	w = q.w + r.w;
}

inline void Quaternion::sub(const Quaternion& q, const Quaternion& r)
{
	x = q.x - r.x;
	y = q.y - r.y;
	z = q.z - r.z;
	w = q.w - r.w;
}

inline void Quaternion::mult(const Quaternion& q, const Quaternion& r)
{
	float A, B, C, D, E, F, G, H;

	A = (q.w + q.x)*(r.w + r.x);
	B = (q.z - q.y)*(r.y - r.z);
	C = (q.w - q.x)*(r.y + r.z); 
	D = (q.y + q.z)*(r.w - r.x);
	E = (q.x + q.z)*(r.x + r.y);
	F = (q.x - q.z)*(r.x - r.y);
	G = (q.w + q.y)*(r.w - r.z);
	H = (q.w - q.y)*(r.w + r.z);

	w = B + (-E - F + G + H) /2;
	x = A - (E + F + G + H)/2; 
	y = C + (E - F + G - H)/2; 
	z = D + (E - F - G + H)/2;

	//x = q.y * r.z - q.z * r.y + r.w * q.x + q.w * r.x;
	//y = q.z * r.x - q.x * r.z + r.w * q.y + q.w * r.y;
	//z = q.x * r.y - q.y * r.x + r.w * q.z + q.w * r.z;
	//w = q.w * r.w - q.x * r.x - q.y * r.y - q.z * r.z;
}


inline void Quaternion::conj()
{
	x = -x;
	y = -y;
	z = -z;
}

inline void Quaternion::conj(const Quaternion& q)
{
	x = -q.x;
	y = -q.y;
	z = -q.z;
	w = q.w;
}

inline float Quaternion::norm()
{
	return x * x + y * y + z * z + w * w;	// Norma mal calculada
}

inline void Quaternion::invert(const Quaternion& q)
{
	float inv_norm = 1.0f / norm();

	x = -q.x * inv_norm;
	y = -q.z * inv_norm;
	z = -q.x * inv_norm;

	w = q.w * inv_norm;
}

inline void Quaternion::invert()
{
	float inv_norm = 1.0f / norm();

	x *= -inv_norm;
	y *= -inv_norm;
	z *= -inv_norm;

	w *= inv_norm;
}


inline void Quaternion::unit_invert()
{
	conj();
}

inline void Quaternion::unit_invert(const Quaternion& q)
{
	conj(q);
	w = q.w;
}

inline void Quaternion::rotation_quaternion(float _x, float _y, float _z, float ang)
{
	float ang_2 = ang * 0.5f * PI_OVER_180;
	float sin_ang_2 = sin(ang_2);

	x = _x * sin_ang_2;
	y = _y * sin_ang_2;
	z = _z * sin_ang_2;
	w = cos(ang_2);
}

inline void Quaternion::rotation_quaternion(const Vec3f& vec, float ang)
{
	ang = -ang;		// Aparentemente a rotação estava no sentido contrario, assim fica bem !!!
	float ang_2 = ang * 0.5f * PI_OVER_180;
	float sin_ang_2 = sin(ang_2);
	Vec3f v = vec;
	v.normalize();

	x = v.x * sin_ang_2;
	y = v.y * sin_ang_2;
	z = v.z * sin_ang_2;
	w = cos(ang_2);
}

// NOTA: Pequeno erro, chamar com (roll,yaw,pitch) ...
inline void Quaternion::euler_quaternion(float yaw, float pitch, float roll)
{
	/*
	float cr, cp, cy, sr, sp, sy, cpcy, spsy;

	yaw *= PI_OVER_180;
	pitch *= PI_OVER_180;
	roll *= PI_OVER_180;

	cr = cos(roll/2);
	cp = cos(pitch/2);
	cy = cos(yaw/2);

	sr = sin(roll/2);
	sp = sin(pitch/2);
	sy = sin(yaw/2);
	
	cpcy = cp * cy;
	spsy = sp * sy;

	x = sr * cpcy - cr * spsy;
	y = cr * sp * cy + sr * cp * sy;
	z = cr * cp * sy - sr * sp * cy;
	w = cr * cpcy + sr * spsy;
	*/
	Quaternion q1,q2,q3;
	//q1.rotation_quaternion(0,1,0,yaw);
	//q2.rotation_quaternion(1,0,0,pitch);
	//q3.mult(q2,q1);
	//q2.rotation_quaternion(0,0,-1,pitch);
	//mult(q2,q3);
	q1.rotation_quaternion(0,1,0,yaw);
	q2.rotation_quaternion(1,0,0,pitch);
	q3.mult(q1,q2);
	q2.rotation_quaternion(0,0,-1,roll);
	mult(q3,q2);
}


// Assumes a unit quaternion
inline void Quaternion::rotate_point(Vec3f& dst, const Vec3f& org)
{
	Quaternion p(org.x, org.y, org.z,0);		// NOTA: Optimizar isto 'a mao ...
	Quaternion r,r1,c;

	c.conj(*this);
	r1.mult(*this,p);
	r.mult(r1,c);

	dst.x = r.x;
	dst.y = r.y;
	dst.z = r.z;	
}

inline void Quaternion::slerp(const Quaternion& to, float t, Quaternion& res)
{
	float	to1[4];
	double omega, cosom, sinom, scale0, scale1;
	const float DELTA = 0.0001f;


	// calc cosine
	cosom = x * to.x + y * to.y + z * to.z + w * to.w;

	// adjust signs (if necessary)
	if ( cosom <0.0 ){ cosom = -cosom; to1[0] = - to.x;
		to1[1] = - to.y;
		to1[2] = - to.z;
		to1[3] = - to.w;
	} else  {
		to1[0] = to.x;
		to1[1] = to.y;
		to1[2] = to.z;
		to1[3] = to.w;
	}

	// calculate coefficients
	if ( (1.0 - cosom) > DELTA ) {
		// standard case (slerp)
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0 - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;

	} else {        
		// "from" and "to" quaternions are very close 
		//  ... so we can do a linear interpolation
		scale0 = 1.0 - t;
		scale1 = t;
	}

	// calculate final values
	res.x = scale0 * x + scale1 * to1[0];
	res.y = scale0 * y + scale1 * to1[1];
	res.z = scale0 * z + scale1 * to1[2];
	res.w = scale0 * w + scale1 * to1[3];
}

inline Quaternion vector_rotation(Vec3f from, Vec3f to)
{
	from.normalize();
	to.normalize();

	if(from == to){
		return Quaternion(0,0,0,1);
	}

	if(from == -to){
		// Existem infinitas escolhas para o vector de rotação, ERRO ...
		return Quaternion(0,0,0,1);
	}else{
		Vec3f rot = cross_product(from,to);
		float cos_2ang = dot_product(from,to);
		rot.normalize();
		float sin_ang = sqrt((1 - cos_2ang) / 2);
		float cos_ang = sqrt(1 - sin_ang * sin_ang);
		return Quaternion(rot.x * sin_ang, rot.y * sin_ang,rot.z * sin_ang, cos_ang);
	}
}


// ********************************************************************/
// Menos eficientes - use with care !!!

inline Quaternion operator+(const Quaternion& q, const Quaternion& r)
{
	Quaternion ret;

	ret.x = q.x + r.x;
	ret.y = q.y + r.y;
	ret.z = q.z + r.z;
	ret.w = q.w + r.w;

	return ret;
}

inline Quaternion operator-(const Quaternion& q, const Quaternion& r)
{
	Quaternion ret;

	ret.x = q.x - r.x;
	ret.y = q.y - r.y;
	ret.z = q.z - r.z;
	ret.w = q.w - r.w;

	return ret;
}

inline Quaternion operator*(const Quaternion& q, const Quaternion& r)
{
	Quaternion ret;

	float A, B, C, D, E, F, G, H;

	A = (q.w + q.x)*(r.w + r.x);
	B = (q.z - q.y)*(r.y - r.z);
	C = (q.w - q.x)*(r.y + r.z); 
	D = (q.y + q.z)*(r.w - r.x);
	E = (q.x + q.z)*(r.x + r.y);
	F = (q.x - q.z)*(r.x - r.y);
	G = (q.w + q.y)*(r.w - r.z);
	H = (q.w - q.y)*(r.w + r.z);

	ret.w = B + (-E - F + G + H) /2;
	ret.x = A - (E + F + G + H)/2; 
	ret.y = C + (E - F + G - H)/2; 
	ret.z = D + (E - F - G + H)/2;


	//ret.x += q.y * r.z - q.z * r.y + r.w * q.x + q.w * r.x;
	//ret.y += q.z * r.x - q.x * r.z + r.w * q.y + q.w * r.y;
	//ret.z += q.x * r.y - q.y * r.x + r.w * q.z + q.w * r.z;

	//ret.w += q.w * r.w - q.x * r.x - q.y * r.y - q.z * r.z;


	return ret;
}

#endif
