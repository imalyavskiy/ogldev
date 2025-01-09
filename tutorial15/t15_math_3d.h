/*

  Copyright 2010 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MATH_3D_H
#define	MATH_3D_H

#include <cstdio>
#include <cmath>

namespace t15
{
  inline float toRadian(float x)
  {
    return ((x) * M_PI / 180.0f);
  }

  inline float toDegree(float x)
  {
    return ((x) * 180.0f / M_PI);
  }

  struct Vector2i
  {
    int x = 0;
    int y = 0;
  };

  struct Vector3f
  {
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;

    Vector3f() = default;

    Vector3f(const float _x, const float _y, const float _z)
      : x(_x), y(_y), z(_z)
    {
    }

    Vector3f& operator+=(const Vector3f& r)
    {
      x += r.x;
      y += r.y;
      z += r.z;

      return *this;
    }

    Vector3f& operator-=(const Vector3f& r)
    {
      x -= r.x;
      y -= r.y;
      z -= r.z;

      return *this;
    }

    Vector3f& operator*=(float f)
    {
      x *= f;
      y *= f;
      z *= f;

      return *this;
    }

    Vector3f Cross(const Vector3f& v) const;

    Vector3f& Normalize();

    void Rotate(float angle, const Vector3f& Axis);

    void Print() const
    {
      printf("(%.02f, %.02f, %.02f)", x, y, z);
    }
  };

  inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
  {
    return Vector3f{ l.x + r.x
             , l.y + r.y
             , l.z + r.z
    };
  }

  inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
  {
    return Vector3f{ l.x - r.x
           , l.y - r.y
           , l.z - r.z
    };
  }

  inline Vector3f operator*(const Vector3f& l, float f)
  {
    return Vector3f{ l.x * f
             , l.y * f
             , l.z * f
    };
  }

  class Matrix4f
  {
  public:
    float m[4][4] = { {0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 0.f, 0.f} };

    static void InitIdentity(Matrix4f& m);

    Matrix4f operator*(const Matrix4f& Right) const;

    static void InitScaleTransform(Matrix4f& m, const float x, const float y, const float z);

    static void InitRotateTransform(Matrix4f& m, const float x, const float y, const float z);

    static void InitTranslationTransform(Matrix4f& m, const float x, const float y, const float z);

    static void InitCameraTransform(Matrix4f& m, const Vector3f& target, const Vector3f& up);

    static void InitPersProjTransform(Matrix4f& m, const float fov, const float w, const float h, const float zn, const float zf);
  };


  struct Quaternion
  {
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float w = 0.f;

    Quaternion() = default;

    Quaternion(const float _x, const float _y, const float _z, const float _w);

    void Normalize();

    Quaternion Conjugate() const;
  };

  Quaternion operator*(const Quaternion& l, const Quaternion& r);

  Quaternion operator*(const Quaternion& q, const Vector3f& v);
}
#endif	/* MATH_3D_H */

