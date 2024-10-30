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

#include "t21_math_3d.h"

namespace t21
{
  Vector3f Vector3f::Cross(const Vector3f& v) const
  {
    const float _x = y * v.z - z * v.y;
    const float _y = z * v.x - x * v.z;
    const float _z = x * v.y - y * v.x;

    return Vector3f(_x, _y, _z);
  }

  Vector3f& Vector3f::Normalize()
  {
    const float Length = sqrtf(x * x + y * y + z * z);

    x /= Length;
    y /= Length;
    z /= Length;

    return *this;
  }

  void Vector3f::Rotate(float angle, const Vector3f& axe)
  {
    const float SinHalfAngle = sinf(ToRadian(angle/2));
    const float CosHalfAngle = cosf(ToRadian(angle/2));

    const float Rx = axe.x * SinHalfAngle;
    const float Ry = axe.y * SinHalfAngle;
    const float Rz = axe.z * SinHalfAngle;
    const float Rw = CosHalfAngle;
    Quaternion RotationQ(Rx, Ry, Rz, Rw);

    Quaternion ConjugateQ = RotationQ.Conjugate();
    //  ConjugateQ.Normalize();
    Quaternion W = RotationQ * (*this) * ConjugateQ;

    x = W.x;
    y = W.y;
    z = W.z;
  }


  void Matrix4f::InitScaleTransform(float scaleX, float scaleY, float scaleZ)
  {
    m[0][0] = scaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
    m[1][0] = 0.0f;   m[1][1] = scaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = scaleZ; m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
  }

  void Matrix4f::InitRotateTransform(float rotateX, float rotateY, float rotateZ)
  {
    Matrix4f rx, ry, rz;

    const float x = ToRadian(rotateX);
    const float y = ToRadian(rotateY);
    const float z = ToRadian(rotateZ);

    rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f   ; rx.m[0][2] = 0.0f    ; rx.m[0][3] = 0.0f;
    rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x); rx.m[1][2] = -sinf(x); rx.m[1][3] = 0.0f;
    rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x); rx.m[2][2] = cosf(x) ; rx.m[2][3] = 0.0f;
    rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f   ; rx.m[3][2] = 0.0f    ; rx.m[3][3] = 1.0f;

    ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y); ry.m[0][3] = 0.0f;
    ry.m[1][0] = 0.0f   ; ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f    ; ry.m[1][3] = 0.0f;
    ry.m[2][0] = sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y) ; ry.m[2][3] = 0.0f;
    ry.m[3][0] = 0.0f   ; ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f    ; ry.m[3][3] = 1.0f;

    rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z); rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
    rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z) ; rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
    rz.m[2][0] = 0.0f   ; rz.m[2][1] = 0.0f    ; rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
    rz.m[3][0] = 0.0f   ; rz.m[3][1] = 0.0f    ; rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

    *this = rz * ry * rx;
  }

  void Matrix4f::InitTranslationTransform(float x, float y, float z)
  {
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
  }


  void Matrix4f::InitCameraTransform(const Vector3f& target, const Vector3f& up)
  {
    Vector3f N = target;
    N.Normalize();
    Vector3f U = up;
    U.Normalize();
    U = U.Cross(N);
    Vector3f V = N.Cross(U);

    m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
    m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
    m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
    m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
  }

  void Matrix4f::InitPersProjTransform(float FOV, float width, float Height, float zNear, float zFar)
  {
    const float ar         = width / Height;
    const float zRange     = zNear - zFar;
    const float tanHalfFOV = tanf(ToRadian(FOV / 2.0f));

    m[0][0] = 1.0f/(tanHalfFOV * ar); m[0][1] = 0.0f;            m[0][2] = 0.0f;          m[0][3] = 0.0;
    m[1][0] = 0.0f;                   m[1][1] = 1.0f/tanHalfFOV; m[1][2] = 0.0f;          m[1][3] = 0.0;
    m[2][0] = 0.0f;                   m[2][1] = 0.0f;            m[2][2] = (-zNear -zFar)/zRange ; m[2][3] = 2.0f * zFar*zNear/zRange;
    m[3][0] = 0.0f;                   m[3][1] = 0.0f;            m[3][2] = 1.0f;          m[3][3] = 0.0;
  }


  Quaternion::Quaternion(const float _x, const float _y, const float _z, const float _w)
      : x(_x), y(_y), z(_z), w(_w)
  {
  }

  void Quaternion::Normalize()
  {
    const float length = sqrtf(x * x + y * y + z * z + w * w);

    x /= length;
    y /= length;
    z /= length;
    w /= length;
  }


  Quaternion Quaternion::Conjugate()
  {
    return { -x, -y, -z, w };
  }

  Quaternion operator*(const Quaternion& l, const Quaternion& r)
  {
    const float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
    const float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
    const float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
    const float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

    return { x, y, z, w };
  }

  Quaternion operator*(const Quaternion& q, const Vector3f& v)
  {
    const float w = - (q.x * v.x) - (q.y * v.y) - (q.z * v.z);
    const float x =   (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
    const float y =   (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
    const float z =   (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

    return { x, y, z, w };
  }
}