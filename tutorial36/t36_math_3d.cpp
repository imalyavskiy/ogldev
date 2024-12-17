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

#include <cstdlib>
#include "t36_util.h"
#include "t36_math_3d.h"

namespace t36
{
  Vector3f Vector3f::Cross(const Vector3f& v) const
  {
    const float _x = y * v.z - z * v.y;
    const float _y = z * v.x - x * v.z;
    const float _z = x * v.y - y * v.x;

    return { _x, _y, _z };
  }

  Vector3f& Vector3f::Normalize()
  {
    const float len = sqrtf(x * x + y * y + z * z);

    x /= len;
    y /= len;
    z /= len;

    return *this;
  }

  void Vector3f::Rotate(float Angle, const Vector3f& axis)
  {
    const float sinHalfAngle = sinf(ToRadian(Angle / 2.f));
    const float cosHalfAngle = cosf(ToRadian(Angle / 2.f));

    const float rx = axis.x * sinHalfAngle;
    const float ry = axis.y * sinHalfAngle;
    const float rz = axis.z * sinHalfAngle;
    const float rw = cosHalfAngle;
    Quaternion rotationQ(rx, ry, rz, rw);

    const Quaternion conjugateQ = rotationQ.Conjugate();

    //  ConjugateQ.Normalize();
    const Quaternion w = rotationQ * (*this) * conjugateQ;

    x = w.x;
    y = w.y;
    z = w.z;
  }


  Matrix4f Matrix4f::Transpose() const
  {
    Matrix4f n;

    for (unsigned int i = 0; i < 4; i++) {
      for (unsigned int j = 0; j < 4; j++) {
        n.m[i][j] = m[j][i];
      }
    }

    return n;
  }

  void Matrix4f::InitIdentity()
  {
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
  }

  Matrix4f Matrix4f::operator*(const Matrix4f& Right) const
  {
    Matrix4f Ret;

    for (unsigned int i = 0; i < 4; i++) {
      for (unsigned int j = 0; j < 4; j++) {
        Ret.m[i][j] = m[i][0] * Right.m[0][j] +
          m[i][1] * Right.m[1][j] +
          m[i][2] * Right.m[2][j] +
          m[i][3] * Right.m[3][j];
      }
    }

    return Ret;
  }

  Vector4f Matrix4f::operator*(const Vector4f& v) const
  {
    Vector4f r;

    r.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
    r.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
    r.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
    r.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;

    return r;
  }

  void Matrix4f::Print() const
  {
    for (int i = 0; i < 4; i++) {
      printf("%f %f %f %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
    }
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

    rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f; rx.m[0][2] = 0.0f; rx.m[0][3] = 0.0f;
    rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x); rx.m[1][2] = -sinf(x); rx.m[1][3] = 0.0f;
    rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x); rx.m[2][2] = cosf(x); rx.m[2][3] = 0.0f;
    rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f; rx.m[3][2] = 0.0f; rx.m[3][3] = 1.0f;

    ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y); ry.m[0][3] = 0.0f;
    ry.m[1][0] = 0.0f; ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f; ry.m[1][3] = 0.0f;
    ry.m[2][0] = sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y); ry.m[2][3] = 0.0f;
    ry.m[3][0] = 0.0f; ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f; ry.m[3][3] = 1.0f;

    rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z); rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
    rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z); rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
    rz.m[2][0] = 0.0f; rz.m[2][1] = 0.0f; rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
    rz.m[3][0] = 0.0f; rz.m[3][1] = 0.0f; rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

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
    Vector3f n = target;
    n.Normalize();
    Vector3f u = up;
    u.Normalize();
    u = u.Cross(n);
    const Vector3f v = n.Cross(u);

    m[0][0] = u.x; m[0][1] = u.y; m[0][2] = u.z; m[0][3] = 0.f;
    m[1][0] = v.x; m[1][1] = v.y; m[1][2] = v.z; m[1][3] = 0.f;
    m[2][0] = n.x; m[2][1] = n.y; m[2][2] = n.z; m[2][3] = 0.f;
    m[3][0] = 0.f; m[3][1] = 0.f; m[3][2] = 0.f; m[3][3] = 1.f;
  }

  void Matrix4f::InitPerspProjTransform(const PerspProjInfo& perspProjInfo)
  {
    const float ar         = perspProjInfo.Width / perspProjInfo.Height;
    const float zRange     = perspProjInfo.zNear - perspProjInfo.zFar;
    const float tanHalfFOV = tanf(ToRadian(perspProjInfo.FOV / 2.0f));

    const float m00 = 1.0f / (tanHalfFOV * ar);
    const float m11 = 1.0f / tanHalfFOV;
    const float m22 = (-perspProjInfo.zNear - perspProjInfo.zFar) / zRange;
    const float m23 = 2.0f * perspProjInfo.zFar * perspProjInfo.zNear / zRange;

    m[0][0] = m00; m[0][1] = 0.f; m[0][2] = 0.f; m[0][3] = 0.f;
    m[1][0] = 0.f; m[1][1] = m11; m[1][2] = 0.f; m[1][3] = 0.f;
    m[2][0] = 0.f; m[2][1] = 0.f; m[2][2] = m22; m[2][3] = m23;
    m[3][0] = 0.f; m[3][1] = 0.f; m[3][2] = 1.f; m[3][3] = 0.f;
  }


  Quaternion::Quaternion(float _x, float _y, float _z, float _w)
    : x(_x), y(_y), z(_z), w(_w)
  {
  }

  void Quaternion::Normalize()
  {
    const float len = sqrtf(x * x + y * y + z * z + w * w);

    x /= len;
    y /= len;
    z /= len;
    w /= len;
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
    const float w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
    const float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
    const float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
    const float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

    return { x, y, z, w };
  }

  float RandomFloat()
  {
    return RANDOM() / (1.f * RAND_MAX);
  }
}