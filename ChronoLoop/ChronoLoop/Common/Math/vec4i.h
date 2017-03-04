#pragma once
#include <DirectXMath.h>

namespace Epoch {

	struct vec4i {
		union {
			DirectX::XMVECTOR vector;
			struct {
				int x, y, z, w;
			};
			int xyzw[4];
		};
		vec4i() { x = 0; y = 0; z = 0; w = 0; };
		vec4i(int _x, int _y, int _z, int _w) { x = _x; _y = y; _z = z; w = _w; };
		vec4i(const vec4i& _copy);

		bool   operator==(const vec4i& _other) const;
		inline bool operator!=(const vec4i& _other) const { return !(*this == _other); }
		vec4i& operator=(const vec4i& _other);
		vec4i  operator^(const vec4i& _other) const;
		int  operator*(const vec4i  & _other) const;
		vec4i  operator*(const int  & _other) const;
		vec4i& operator*=(const int &_other);
		vec4i  operator/(const int  & _other) const;
		vec4i& operator/=(const int & _other);
		vec4i  operator-() const;
		vec4i  operator-(const vec4i & _other) const;
		vec4i  operator-(const int & _other) const;
		vec4i& operator-=(const vec4i& _other);
		vec4i& operator-=(const int& _other);
		vec4i  operator+(const int & _other) const;
		vec4i  operator+(const vec4i & _other) const;
		vec4i& operator+=(const int& _other);
		vec4i& operator+=(const vec4i& _other);
		int& operator[](const unsigned int _index);
		vec4i  Cross(const vec4i& _other) const;
		int  Dot(const vec4i& _other) const;
		int  Magnitude() const;
		int  SquaredMagnitude() const;
		vec4i  Normalize() const;
		vec4i  Reflect(const vec4i& other) const;
		vec4i& Set(const int _x, const int _y, const int _z, const int _w);
	};

}