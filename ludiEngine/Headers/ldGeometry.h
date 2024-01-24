/******************************************************************************************
*                                       GEOMETRY                                          *
*                                                                                         *
* DESCRIPTION: Implements several geometric primitives, like vectors					  *
*                                                                                         *
* AUTHOR: Pedro Pires <pedro.pires@acm.org>                                               *
*                                                                                         *
* COPYRIGHT: Copyright 2001, Pedro Pires & Diogo Andrade. All rights reserved.            *
******************************************************************************************/

#ifndef __PP_STORM_GEOMETRY__
#define __PP_STORM_GEOMETRY__

#include <math.h>
#include <iostream>			// Para debug apenas ...

const float PI = 3.1415926535f;
const float PI_OVER_180 = 3.1415926535f / 180.0f;
const float PI_UNDER_180 = 180.0f / 3.1415926535f;
const float PI_OVER_2 = 3.1415926535f / 2.0f;
const float PI_OVER_4 = 3.1415926535f / 4.0f;
const float PI_TIMES_2 = 3.1415926535f * 2.0f;

const float VERY_SMALL_FLOAT = 1e-8f;		// Depois usar o <limits.h> ...
const float SMALLEST_FLOAT = -99999999.0f;
const float BIGGEST_FLOAT = 99999999.0f;

/**
 *  Represents a vector in 2 space.
 */

class Vec2f
{
public:
	float x;
	float y;
public:
	Vec2f();									///< Builds an uninicialized vector
	Vec2f(float a, float b);					///< Builds and inicializes a vector
	void normalize();							///< Divides each component by its normal: \f$\left( \frac{x}{\sqrt{x^2+y^2}},\frac{y}{\sqrt{x^2+y^2}} \right) \f$
	float norm();								///< Returns the norm of the vector: \f$\sqrt{x^2+y^2}\f$
	float norm_squared();						///< Returns the square of the norm of the vector: \f$x^2+y^2\f$
	void set(float a,float b);
};


/**
 *  Represents a vector in 3 space.
 */

class Vec3f
{
public:
	float x;
	float y;
	float z;
public:
	Vec3f();									///< Builds an uninicialized vector
	Vec3f(float a, float b, float c);			///< Builds and inicializes a vector
	void normalize();							///< Divides each component by its normal: \f$\left( \frac{x}{\sqrt{x^2+y^2+z^2}},\frac{y}{\sqrt{x^2+y^2+z^2}},\frac{z}{\sqrt{x^2+y^2+z^2}} \right) \f$
	float norm();								///< Returns the norm of the vector: \f$\sqrt{x^2+y^2+z^2}\f$
	float norm_squared();						///< Returns the square of the norm of the vector: \f$x^2+y^2+z^2\f$
	void set(float a,float b,float c);
};

/**
 *  Represents a vector in 4 space.
 */

class Vec4f
{
public:
	float x;
	float y;
	float z;
	float w;
public:
	Vec4f();									///< Builds an uninicialized vector
	Vec4f(float a, float b, float c, float d);	///< Builds and inicializes a vector
	void normalize();							///< Divides each component by its normal: \f$\left( \frac{x}{\sqrt{x^2+y^2+z^2+w^2}},\frac{y}{\sqrt{x^2+y^2+z^2+w^2}},\frac{z}{\sqrt{x^2+y^2+z^2+w^2}},,\frac{w}{\sqrt{x^2+y^2+z^2+w^2}} \right) \f$
	float norm();								///< Returns the norm of the vector: \f$\sqrt{x^2+y^2+z^2+w^2}\f$
	float norm_squared();						///< Returns the square of the norm of the vector: \f$x^2+y^2+z^2+w^2\f$
	void set(float a,float b,float c,float d);
};

inline float abs(float f)
{
	return (f > 0) ? f : -f;
}

/*********************************************************************************************
*                                        >>> VEC2F <<<                                       *
*********************************************************************************************/

inline Vec2f::Vec2f()
{
}

inline Vec2f::Vec2f(float a, float b)
{
x = a;
y = b;
}

inline void Vec2f::set(float a,float b)
{
x=a; y=b;;
}

inline void Vec2f::normalize()
{
register float norm = (float)sqrt(x*x + y*y);
x = x/norm;
y = y/norm;
}

inline float Vec2f::norm()
{
return (float)sqrt(x*x + y*y);
}

inline float Vec2f::norm_squared()
{
return x*x + y*y;
}

/**
*  Adds two vectors..
*/
inline Vec2f operator+(const Vec2f& vec1, const Vec2f& vec2)
{
Vec2f retVec;

retVec.x = vec1.x + vec2.x;
retVec.y = vec1.y + vec2.y;

return retVec;
}

inline Vec2f operator-(const Vec2f& vec1, const Vec2f& vec2)
{
Vec2f retVec;

retVec.x = vec1.x - vec2.x;
retVec.y = vec1.y - vec2.y;

return retVec;
}

// Produto com um escalar
inline Vec2f operator*(const Vec2f& vec, float s)
{
Vec2f retVec;

retVec.x = vec.x * s;
retVec.y = vec.y * s;

return retVec;
}

// Produto com um escalar
inline Vec2f operator*(float s, const Vec2f& vec)
{
Vec2f retVec;

retVec.x = vec.x * s;
retVec.y = vec.y * s;

return retVec;
}

// Negar o vector (produto com -1)
inline Vec2f operator-(const Vec2f& vec)
{
Vec2f retVec;

retVec.x = -vec.x;
retVec.y = -vec.y;

return retVec;
}

inline bool operator==(const Vec2f& vec1, const Vec2f& vec2)
{
return ((vec1.x == vec2.x) &&
		(vec1.y == vec2.y)    );
}

inline bool operator!=(const Vec2f& vec1, const Vec2f& vec2)
{
return ((vec1.x != vec2.x) ||
		(vec1.y != vec2.y)    );
}

inline float dot_product(const Vec2f& vec1, const Vec2f& vec2)
{
return vec1.x * vec2.x + vec1.y * vec2.y;
}

/*********************************************************************************************
*                                        >>> VEC3F <<<                                       *
*********************************************************************************************/

inline Vec3f::Vec3f()
{
}

inline Vec3f::Vec3f(float a, float b, float c)
{
x = a;
y = b;
z = c;
}

inline void Vec3f::set(float a,float b,float c)
{
x=a; y=b; z=c;
}

inline void Vec3f::normalize()
{
register float norm = (float)sqrt(x*x + y*y + z*z);
x = x/norm;
y = y/norm;
z = z/norm;
}

inline float Vec3f::norm()
{
return (float)sqrt(x*x + y*y + z*z);
}

inline float Vec3f::norm_squared()
{
return x*x + y*y + z*z;
}

inline Vec3f operator+(const Vec3f& vec1, const Vec3f& vec2)
{
Vec3f retVec;

retVec.x = vec1.x + vec2.x;
retVec.y = vec1.y + vec2.y;
retVec.z = vec1.z + vec2.z;

return retVec;
}

inline Vec3f operator-(const Vec3f& vec1, const Vec3f& vec2)
{
Vec3f retVec;

retVec.x = vec1.x - vec2.x;
retVec.y = vec1.y - vec2.y;
retVec.z = vec1.z - vec2.z;

return retVec;
}

// Produto com um escalar
inline Vec3f operator*(const Vec3f& vec, float s)
{
Vec3f retVec;

retVec.x = vec.x * s;
retVec.y = vec.y * s;
retVec.z = vec.z * s;

return retVec;
}

// Produto com um escalar
inline Vec3f operator*(float s, const Vec3f& vec)
{
Vec3f retVec;

retVec.x = vec.x * s;
retVec.y = vec.y * s;
retVec.z = vec.z * s;

return retVec;
}

// Divisiao com um escalar
inline Vec3f operator/(const Vec3f& vec, float s)
{
Vec3f retVec;

retVec.x = vec.x / s;
retVec.y = vec.y / s;
retVec.z = vec.z / s;

return retVec;
}

// Divisao com um escalar
inline Vec3f operator/(float s, const Vec3f& vec)
{
Vec3f retVec;

retVec.x = vec.x / s;
retVec.y = vec.y / s;
retVec.z = vec.z / s;

return retVec;
}

// Negar o vector (produto com -1)
inline Vec3f operator-(const Vec3f& vec)
{
Vec3f retVec;

retVec.x = -vec.x;
retVec.y = -vec.y;
retVec.z = -vec.z;

return retVec;
}

inline bool operator==(const Vec3f& vec1, const Vec3f& vec2)
{
return ((vec1.x == vec2.x) &&
		(vec1.y == vec2.y) &&
		(vec1.z == vec2.z)    );
}

inline bool operator!=(const Vec3f& vec1, const Vec3f& vec2)
{
return ((vec1.x != vec2.x) ||
		(vec1.y != vec2.y) ||
		(vec1.z != vec2.z)    );
}

inline float dot_product(const Vec3f& vec1, const Vec3f& vec2)
{
return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

inline float dot_product(Vec3f *vec1,Vec3f *vec2)
{
return vec1->x * vec2->x + vec1->y * vec2->y + vec1->z * vec2->z;
}

inline Vec3f cross_product(const Vec3f& u, const Vec3f& v)
{
Vec3f retVec;

retVec.x = u.y * v.z - u.z * v.y;
retVec.y = u.z * v.x - u.x * v.z;
retVec.z = u.x * v.y - u.y * v.x;

return retVec;
}

/*********************************************************************************************
*                                        >>> VEC4F <<<                                       *
*********************************************************************************************/

inline Vec4f::Vec4f()
{
}

inline Vec4f::Vec4f(float a, float b, float c, float d)
{
x = a;
y = b;
z = c;
w = d;
}

inline void Vec4f::normalize()
{
register float norm = (float)sqrt(x*x + y*y + z*z + w*w);
x = x/norm;
y = y/norm;
z = z/norm;
w = w/norm;
}

inline float Vec4f::norm()
{
return (float)sqrt(x*x + y*y + z*z + w*w);
}

inline float Vec4f::norm_squared()
{
return x*x + y*y + z*z + w*w;
}

inline void Vec4f::set(float a,float b,float c,float d)
{
x=a; y=b; z=c; w=d;
}

inline Vec4f operator+(const Vec4f& vec1, const Vec4f& vec2)
{
Vec4f retVec;

retVec.x = vec1.x + vec2.x;
retVec.y = vec1.y + vec2.y;
retVec.z = vec1.z + vec2.z;
retVec.w = vec1.w + vec2.w;

return retVec;
}

inline Vec4f operator-(const Vec4f& vec1, const Vec4f& vec2)
{
Vec4f retVec;

retVec.x = vec1.x - vec2.x;
retVec.y = vec1.y - vec2.y;
retVec.z = vec1.z - vec2.z;
retVec.z = vec1.w - vec2.w;

return retVec;
}

// Produto com um escalar
inline Vec4f operator*(const Vec4f& vec, float s)
{
Vec4f retVec;

retVec.x = vec.x * s;
retVec.y = vec.y * s;
retVec.z = vec.z * s;
retVec.w = vec.z * s;

return retVec;
}

// Produto com um escalar
inline Vec4f operator*(float s, const Vec4f& vec)
{
Vec4f retVec;

retVec.x = vec.x * s;
retVec.y = vec.y * s;
retVec.z = vec.z * s;
retVec.w = vec.z * s;

return retVec;
}

// Negar o vector (produto com -1)
inline Vec4f operator-(const Vec4f& vec)
{
Vec4f retVec;

retVec.x = -vec.x;
retVec.y = -vec.y;
retVec.z = -vec.z;
retVec.w = -vec.w;

return retVec;
}

inline bool operator==(const Vec4f& vec1, const Vec4f& vec2)
{
return ((vec1.x == vec2.x) &&
		(vec1.y == vec2.y) &&
		(vec1.z == vec2.z) &&
		(vec1.w == vec2.w)    );
}

inline bool operator!=(const Vec4f& vec1, const Vec4f& vec2)
{
return ((vec1.x != vec2.x) ||
		(vec1.y != vec2.y) ||
		(vec1.z != vec2.z) ||
		(vec1.w != vec2.w)    );
}

inline float dot_product(const Vec4f& vec1, const Vec4f& vec2)
{
return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
}

inline float deg_to_rad(float ang) { return ang*PI_OVER_180; }
inline float rad_to_deg(float ang) { return ang*PI_UNDER_180; }

bool is_ray_intersecting_tri(Vec3f *ray_origin,Vec3f *ray_dir,
							 Vec3f *pos1,Vec3f *pos2,Vec3f *pos3,
							 Vec3f *point);

bool are_triangles_intersecting(Vec3f *pta1,Vec3f *pta2,Vec3f *pta3,
								Vec3f *ptb1,Vec3f *ptb2,Vec3f *ptb3);

#endif
