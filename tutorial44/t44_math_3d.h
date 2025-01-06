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

#include <assimp/vector3.h>
#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>

#include "t44_util.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846f   // pi
#endif

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

namespace t44 {
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

    Vector2f(float _x, float _y);
  };

  struct Vector3f
  {
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;

    Vector3f() = default;

    Vector3f(const float _x, const float _y, const float _z);

    Vector3f(const float* pFloat);

    Vector3f(float f);

    Vector3f& operator+=(const Vector3f& r);

    Vector3f& operator-=(const Vector3f& r);

    Vector3f& operator*=(float f);

    operator const float* () const;

    Vector3f Cross(const Vector3f& v) const;

    Vector3f& Normalize();

    void Rotate(float angle, const Vector3f& axis);

    void Print() const;
  };


  struct Vector4f
  {
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float w = 0.f;

    Vector4f() = default;

    Vector4f(float _x, float _y, float _z, float _w);

    void Print(bool endl = true) const;

    Vector3f To3F() const;
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

  inline Vector4f operator/(const Vector4f& l, float f)
  {
    return { l.x / f, l.y / f, l.z / f, l.w / f };
  }

  struct PerspProjInfo
  {
    float FOV;
    float Width;
    float Height;
    float zNear;
    float zFar;
  };


  struct OrthoProjInfo
  {
    float r;        // right
    float l;        // left
    float b;        // bottom
    float t;        // top
    float n;        // z near
    float f;        // z far
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

    [[nodiscard]]
    Quaternion Conjugate() const;

    Vector3f ToDegrees();
  };


  class Matrix4f
  {
  public:
    float m[4][4] = { {0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 0.f, 0.f} };

    Matrix4f() = default;

    // constructor from Assimp matrix
    Matrix4f(const aiMatrix4x4& assimpMatrix);

    Matrix4f(const aiMatrix3x3& assimpMatrix);

    Matrix4f(float a00, float a01, float a02, float a03,   float a10, float a11, float a12, float a13,
             float a20, float a21, float a22, float a23,   float a30, float a31, float a32, float a33);

    void SetZero();

    Matrix4f Transpose() const;


    inline void InitIdentity();

    inline Matrix4f operator*(const Matrix4f& Right) const;

    Vector4f operator*(const Vector4f& v) const;

    operator const float* () const;

    void Print() const;

    float Determinant() const;

    Matrix4f& Inverse();

    void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
    void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
    void InitRotateTransform(const Quaternion& quat);
    void InitTranslationTransform(float x, float y, float z);
    void InitCameraTransform(const Vector3f& target, const Vector3f& up);
    void InitPersProjTransform(const PerspProjInfo& p);
    void InitOrthoProjTransform(const OrthoProjInfo& p);
  };

  Quaternion operator*(const Quaternion& l, const Quaternion& r);

  Quaternion operator*(const Quaternion& q, const Vector3f& v);
}
#endif	/* MATH_3D_H */

