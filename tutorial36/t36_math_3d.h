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

#include <stdio.h>
#ifdef WIN32
#define _USE_MATH_DEFINES 
#include <cmath>
#else
#include <math.h>
#endif

#ifndef M_PI
#define M_PI       3.14159265358979323846f   // pi
#endif

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

namespace t36
{
  float RandomFloat();

  struct Vector2i
  {
    int x = 0;
    int y = 0;
  };

  struct Vector2f
  {
    float x = 0.f;
    float y = 0.f;

    Vector2f() = default;

    Vector2f(float _x, float _y)
      : x(_x), y(_y)
    {
    }
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

    void Rotate(float Angle, const Vector3f& Axis);

    void Print() const
    {
      printf("(%.02f, %.02f, %.02f)", x, y, z);
    }
  };


  struct Vector4f
  {
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float w = 0.f;

    Vector4f() = default;

    Vector4f(float _x, float _y, float _z, float _w)
      : x(_x), y(_y), z(_z), w(_w)
    {
    }

    void Print() const
    {
      printf("(%.02f, %.02f, %.02f, %.02f)", x, y, z, w);
    }
  };

  inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
  {
    return { l.x + r.x, l.y + r.y, l.z + r.z };
  }

  inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
  {
    return { l.x - r.x, l.y - r.y, l.z - r.z };
  }

  inline Vector3f operator*(const Vector3f& l, float f)
  {
    return { l.x * f, l.y * f, l.z * f };
  }

  struct PerspProjInfo
  {
    float FOV;
    float Width;
    float Height;
    float zNear;
    float zFar;
  };

  class Matrix4f
  {
  public:
    float m[4][4] = { {0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 0.f, 0.f} };

    Matrix4f() = default;

    [[nodiscard]]
    inline Matrix4f Transpose() const;

    void InitIdentity();

    inline Matrix4f operator*(const Matrix4f& Right) const;

    Vector4f operator*(const Vector4f& v) const;

    void Print() const;

    void InitScaleTransform(float scaleX, float scaleY, float scaleZ);
    void InitRotateTransform(float rotateX, float rotateY, float rotateZ);
    void InitTranslationTransform(float x, float y, float z);
    void InitCameraTransform(const Vector3f& target, const Vector3f& up);
    void InitPerspProjTransform(const PerspProjInfo& perspProjInfo);
  };


  struct Quaternion
  {
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float w = 0.f;

    Quaternion() = default;

    Quaternion(float _x, float _y, float _z, float _w);

    void Normalize();

    Quaternion Conjugate();
  };

  Quaternion operator*(const Quaternion& l, const Quaternion& r);

  Quaternion operator*(const Quaternion& q, const Vector3f& v);
}
#endif	/* MATH_3D_H */

