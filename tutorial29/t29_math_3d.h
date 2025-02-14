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

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

namespace t29
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

    void Rotate(float angle, const Vector3f& Axis);

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
    Vector3f Ret(l.x + r.x,
                 l.y + r.y,
                 l.z + r.z);

    return Ret;
  }

  inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
  {
    Vector3f Ret(l.x - r.x,
                 l.y - r.y,
                 l.z - r.z);

    return Ret;
  }

  inline Vector3f operator*(const Vector3f& l, float f)
  {
    Vector3f Ret(l.x * f,
                 l.y * f,
                 l.z * f);

    return Ret;
  }

  struct PersProjInfo
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

    inline void InitIdentity()
    {
      m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
      m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
      m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
      m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    inline Matrix4f operator*(const Matrix4f& Right) const
    {
      Matrix4f Ret;

      for (unsigned int i = 0 ; i < 4 ; i++) {
        for (unsigned int j = 0 ; j < 4 ; j++) {
          Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                        m[i][1] * Right.m[1][j] +
                        m[i][2] * Right.m[2][j] +
                        m[i][3] * Right.m[3][j];
        }
      }

      return Ret;
    }
    
    Vector4f operator*(const Vector4f& v) const
    {
      Vector4f r;
        
      r.x = m[0][0]* v.x + m[0][1]* v.y + m[0][2]* v.z + m[0][3]* v.w;
      r.y = m[1][0]* v.x + m[1][1]* v.y + m[1][2]* v.z + m[1][3]* v.w;
      r.z = m[2][0]* v.x + m[2][1]* v.y + m[2][2]* v.z + m[2][3]* v.w;
      r.w = m[3][0]* v.x + m[3][1]* v.y + m[3][2]* v.z + m[3][3]* v.w;
        
      return r;
    }
    
    void Print()
    {
      for (int i = 0 ; i < 4 ; i++) {
        printf("%f %f %f %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
      }       
    }

    void InitScaleTransform(float scaleX, float scaleY, float scaleZ);
    void InitRotateTransform(float rotateX, float rotateY, float rotateZ);
    void InitTranslationTransform(float x, float y, float z);
    void InitCameraTransform(const Vector3f& target, const Vector3f& up);
    void InitPersProjTransform(const PersProjInfo& p);
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

    Quaternion Conjugate();  
  };

  Quaternion operator*(const Quaternion& l, const Quaternion& r);

  Quaternion operator*(const Quaternion& q, const Vector3f& v);
}

#endif	/* MATH_3D_H */

