#pragma once

#include <cmath>
#include <cassert>

__device__ __host__
float &get(float3 &v, int i)
{
	assert(i >=0 && i <3);
	return i == 0 ? v.x 
		: i == 1 ? v.y
		: v.z;
}

__device__ __host__
float &get(float4 &v, int i)
{
	assert(i >=0 && i <4);
	return i == 0 ? v.x 
		: i == 1 ? v.y
		: i == 2 ? v.z
		: v.w;
}

__device__ __host__
int &get(int3 &v, int i)
{
	assert(i >=0 && i <3);
	return i == 0 ? v.x 
		: i == 1 ? v.y
		: v.z;
}

__device__ __host__
uint &get(uint3 &v, int i)
{
	assert(i >=0 && i <3);
	return i == 0 ? v.x 
		: i == 1 ? v.y
		: v.z;
}

__device__ __host__
float dot(const float3 &a, const float3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

__device__ __host__
float norm2(const float3 &a)
{
	return dot(a,a);
}

__device__ __host__
float norm(const float3 &a)
{
	return sqrtf(norm2(a));
}

__device__ __host__
float3 interpolate(const float3 &a, const float3 &b, const float3 &c, const float3 &bary)
{
	float3 result;
	result.x = bary.x * a.x + bary.y * b.x + bary.z * c.x;
	result.y = bary.x * a.y + bary.y * b.y + bary.z * c.y;
	result.z = bary.x * a.z + bary.y * b.z + bary.z * c.z;
	return result;
}

__device__ __host__
float3 interpolate(const float3 vertices[3], const float3 &bary)
{
	return interpolate(vertices[0],vertices[1],vertices[2],bary);
}

__device__ __host__
bool isfinite(const float3 &v)
{
	return isfinite(v.x) && isfinite(v.y) && isfinite(v.z);
}

__device__ __host__
bool isinf(const float3 &v)
{
	return isinf(v.x) && isinf(v.y) && isinf(v.z);
}

__device__ __host__
bool similar(const float3 &a, const float3 &b, const float epsilon = 0.000001f)
{
	return fabs(a.x-b.x) < epsilon && fabs(a.y-b.y) < epsilon && fabs(a.z-b.z) < epsilon;
}

__device__ __host__
bool operator==(const float3 &a, const float3 &b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

__device__ __host__
float operator*(const float3 &a, const float3 &b)
{
	return dot(a,b);
}

__device__ __host__
void operator*=(float3 &a, float s)
{
	a.x *= s;
	a.y *= s;
	a.z *= s;
}

__device__ __host__
float3 operator*(const float3 &a, float s)
{
	float3 b = a;
	b *= s;
	return b;
}

__device__ __host__
float3 operator*(float s, const float3 &a)
{
	float3 b = a;
	b *= s;
	return b;
}

__device__ __host__
void operator+=(float3 &a, const float3 &b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
}

__device__ __host__
float3 operator+(const float3 &a, const float3 &b)
{
	float3 res = a;
	res += b;
	return res;
}

__device__ __host__
void operator-=(float3 &a, const float3 &b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
}

__device__ __host__
float3 operator-(const float3 &a, const float3 &b)
{
	float3 res = a;
	res -= b;
	return res;
}

__device__ __host__
void operator/=(float3 &a, float s)
{
	a.x /= s;
	a.y /= s;
	a.z /= s;
}

__device__ __host__
float3 operator/(const float3 &a, float s)
{
	float3 b = a;
	b /= s;
	return b;
}

__device__ __host__
float3 operator/(float s, const float3 &a)
{
	float3 b = a;
	b /= s;
	return b;
}

__device__
void accumulate(float3* sum, float3 a)
{
	atomicAdd(&sum->x,a.x);
	atomicAdd(&sum->y,a.y);
	atomicAdd(&sum->z,a.z);
}

__device__
void accumulate(float3 &sum, float3 a)
{
	accumulate(&sum, a);
}

__device__
void accumulate(int *sum, int a)
{
	atomicAdd(sum,a);
}

__device__
void accumulate(int &sum, int a)
{
	accumulate(&sum, a);
}


__device__ __host__
void normalize(float3 &v)
{
	if(norm2(v) > 0.000001f)
		v /= norm(v);
	else 
	{
		v.x = 0;
		v.y = 1;
		v.z = 0;
	}
}

__device__ __host__
float3 normalized(float3 &v)
{
	float3 n = v;
	normalize(n);
	return n;
}

__device__ __host__
dim3 calculate_block_count(dim3 size, dim3 thread_count)
{
	return dim3((size.x+thread_count.x - 1)/thread_count.x,
			(size.y + thread_count.y -1)/thread_count.y,
			(size.z + thread_count.z -1)/thread_count.z);
}
