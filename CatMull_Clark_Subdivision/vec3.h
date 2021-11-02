#ifndef _VEC3_H_
#define _VEC3_H_

#include <iostream>
//#include <xmmintrin.h>
//_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

//_mm_setcsr(_mm_getcsr() | 0x8040);
//#include "Distribution.h"
/*
#define min(x, y) x < y ? x : y
#define max(x, y) x > y ? x : y
#define square(x) x * x
const float eps = 0.0001f;
const float inf = 1e20f;
#define pi 3.1415f
#define ipi 0.3183f
#define i2pi 0.1591f
#define i180 0.0055f
#define abs(x) ((x)<0 ? -(x) : (x))
*/
#define min(x, y) x < y ? x : y
#define max(x, y) x > y ? x : y
//#define max3(x, y, z) max(max(x, y), z)
//#define min3(x, y, z) min(min(x, y), z)
#define abs(x) ((x)<0 ? -(x) : (x))
#define square(x) x * x
const float eps = 0.0001f;
//const float inf = 1000000000.0f;
//#define delta 1e-4
#define inf 1e20
const float pi = 3.1415f;
const float ipi = 0.3183f;
#define i2pi 0.1591f
#define i180 0.0055f


double inline __declspec (naked) __fastcall sqrt14(double n)
{
	_asm fld qword ptr[esp + 4]
		_asm fsqrt
	_asm ret 8
}

struct vec3
{
	 vec3() : x(0), y(0), z(0) {}
	 vec3(float v) : x(v), y(v), z(v) {}
	 vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
	
	//union //alignas(16)
	//{
	//	struct
	//	{
	float x;
	float y;
	float z;
			//float w;
	//	};
	//	__m128 v;
	//};
	 float maxc() const { float d = max(y, z); return max(x, d); }//{ return max(max(x, y), z); }//
	 float minc() const { float d = min(y, z); return min(x, d); }//{ return min(min(x, y), z); }//
	 float operator[](const int& i) const { return (&x)[i]; }
	
	//operator __m128() const { return v; }
	//operator vec3() const { return { x,y,z,w }; }

	 friend vec3 operator+(const vec3& a, const vec3& b) { return{ a.x + b.x, a.y + b.y, a.z + b.z }; }
	 friend vec3 operator-(const vec3& a, const vec3& b) { return{ a.x - b.x, a.y - b.y, a.z - b.z }; }
	 friend vec3 operator*(const vec3& a, const vec3& b) { return{ a.x * b.x, a.y * b.y, a.z * b.z }; }
	 friend vec3 operator*=(const vec3& a, const float& v) { return{ a.x * v, a.y * v, a.z * v }; }
	 friend vec3 operator/(const vec3& a, const vec3& b) { return{ a.x / b.x, a.y / b.y, a.z / b.z }; }
	 friend vec3 operator/=(const vec3& a, const float& v) { return{ a.x / v, a.y / v, a.z / v }; }
	 friend vec3 operator-(const vec3& a) { return{ -a.x, -a.y, -a.z }; }

	 friend vec3 operator/(const vec3& a, const float& v) { return{ a.x / v, a.y / v, a.z / v }; }
	 friend vec3 operator/(const vec3& a, const int& v) { return{ a.x / v, a.y / v, a.z / v }; }
	 friend vec3 operator*(const vec3& a, const float& v) { return{ a.x * v, a.y * v, a.z * v }; }
	 friend vec3 operator*(const float& v, const vec3& a) { return{ a.x * v, a.y * v, a.z * v }; }
	 void operator+=(const vec3& v) { x += v.x; y += v.y; z += v.z;  }
	 void operator*=(const float& value) { x *= value; y *= value; z *= value; }

    vec3 norm() const
	{
		float l = 1 / sqrt(x*x + y*y + z*z); return *this * l;
	}

	void normalize() const
	{
		float l = 1 / sqrt(x*x + y*y + z*z); *this *= l;
	}
	 float dot(const vec3& v) const { return x * v.x + y * v.y + z * v.z; }
	
	 float length() const { return sqrt(x * x + y * y + z * z); }
	 float length2() const { return x * x + y * y + z * z; }
	 vec3 operator%(const vec3& v) const { return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	 vec3 __fastcall  cross(const vec3& v) const { return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	 friend std::ostream& operator<<(std::ostream&os, const vec3& v)
	{
		os << v.x << "  " << v.y << "  " << v.z << "  " << "\n";
		return os;
	}
};

/*
vec3 minvec(const vec3& v1, const vec3& v2)
{
	return vec3(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z));
}

vec3 maxvec(const vec3& v1, const vec3& v2)
{
	return vec3(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z));
}

vec3 minvec(const vec3& v1, const vec3& v2, const vec3& v3)
{
	return vec3(min(v1.x, min(v2.x, v3.x)), min(v1.y, min(v2.y, v3.y)), min(v1.z, min(v2.z, v3.z)));
}

vec3 maxvec(const vec3& v1, const vec3& v2, const vec3& v3)
{
	return vec3(max(v1.x, max(v2.x, v3.x)), max(v1.y, max(v2.y, v3.y)), max(v1.z, max(v2.z, v3.z)));
}
*/


#endif // !_MATH_H_

