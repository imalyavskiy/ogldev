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


#include "t44_util.h"
#include "t44_math_3d.h"

namespace t44 {
  Vector2f::Vector2f(float _x, float _y) : x(_x), y(_y)
  {
  }

  Vector3f::Vector3f(const float _x, const float _y, const float _z): x(_x), y(_y), z(_z)
  {
  }

  Vector3f::Vector3f(const float* pFloat)
  {
    x = pFloat[0];
    y = pFloat[0];
    z = pFloat[0];
  }

  Vector3f::Vector3f(float f)
  {
    x = y = z = f;
  }

  Vector3f& Vector3f::operator+=(const Vector3f& r)
  {
    x += r.x;
    y += r.y;
    z += r.z;

    return *this;
  }

  Vector3f& Vector3f::operator-=(const Vector3f& r)
  {
    x -= r.x;
    y -= r.y;
    z -= r.z;

    return *this;
  }

  Vector3f& Vector3f::operator*=(float f)
  {
    x *= f;
    y *= f;
    z *= f;

    return *this;
  }

  Vector3f::operator const float*() const
  {
    return &(x);
  }

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

  void Vector3f::Rotate(float angle, const Vector3f& Axe)
  {
    const float sinHalfAngle = sinf(ToRadian(angle / 2));
    const float cosHalfAngle = cosf(ToRadian(angle / 2));

    const float rx = Axe.x * sinHalfAngle;
    const float ry = Axe.y * sinHalfAngle;
    const float rz = Axe.z * sinHalfAngle;
    const float rw = cosHalfAngle;
    Quaternion rotationQ(rx, ry, rz, rw);

    const Quaternion conjugateQ = rotationQ.Conjugate();
    //  ConjugateQ.Normalize();
    Quaternion W = rotationQ * (*this) * conjugateQ;

    x = W.x;
    y = W.y;
    z = W.z;
  }

  void Vector3f::Print() const
  {
    printf("(%.02f, %.02f, %.02f)", x, y, z);
  }

  Vector4f::Vector4f(float _x, float _y, float _z, float _w): x(_x), y(_y), z(_z), w(_w)
  {
  }

  void Vector4f::Print(bool endl) const
  {
    printf("(%.02f, %.02f, %.02f, %.02f)", x, y, z, w);

    if (endl) {
      printf("\n");
    }
  }

  Vector3f Vector4f::To3F() const
  {
    return { x, y, z };
  }

  void Matrix4f::InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
  {
    m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
    m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
  }

  void Matrix4f::InitRotateTransform(float RotateX, float RotateY, float RotateZ)
  {
    Matrix4f rx, ry, rz;

    const float x = ToRadian(RotateX);
    const float y = ToRadian(RotateY);
    const float z = ToRadian(RotateZ);

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


  void Matrix4f::InitRotateTransform(const Quaternion& quat)
  {
    const float yy2 = 2.0f * quat.y * quat.y;
    const float xy2 = 2.0f * quat.x * quat.y;
    const float xz2 = 2.0f * quat.x * quat.z;
    const float yz2 = 2.0f * quat.y * quat.z;
    const float zz2 = 2.0f * quat.z * quat.z;
    const float wz2 = 2.0f * quat.w * quat.z;
    const float wy2 = 2.0f * quat.w * quat.y;
    const float wx2 = 2.0f * quat.w * quat.x;
    const float xx2 = 2.0f * quat.x * quat.x;

    const float m00 = -yy2 - zz2 + 1.f;
    const float m01 = xy2 + wz2;
    const float m02 = xz2 - wy2;
    const float m10 = xy2 - wz2;
    const float m11 = -xx2 - zz2 + 1.f;
    const float m12 = yz2 + wx2;
    const float m20 = xz2 + wy2;
    const float m21 = yz2 - wx2;
    const float m22 = -xx2 - yy2 + 1.f;

    m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = 0.f;
    m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = 0.f;
    m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = 0.f;
    m[3][0] = 0.f; m[3][1] = 0.f; m[3][2] = 0.f; m[3][3] = 1.f;

  }

  void Matrix4f::InitTranslationTransform(float x, float y, float z)
  {
    m[0][0] = 1.f; m[0][1] = 0.f; m[0][2] = 0.f; m[0][3] =   x;
    m[1][0] = 0.f; m[1][1] = 1.f; m[1][2] = 0.f; m[1][3] =   y;
    m[2][0] = 0.f; m[2][1] = 0.f; m[2][2] = 1.f; m[2][3] =   z;
    m[3][0] = 0.f; m[3][1] = 0.f; m[3][2] = 0.f; m[3][3] = 1.f;
  }


  void Matrix4f::InitCameraTransform(const Vector3f& target, const Vector3f& up)
  {
    Vector3f n = target;
    n.Normalize();
    Vector3f u = up;
    u.Normalize();
    u = u.Cross(n);
    Vector3f v = n.Cross(u);

    m[0][0] = u.x;   m[0][1] = u.y;   m[0][2] = u.z;   m[0][3] = 0.0f;
    m[1][0] = v.x;   m[1][1] = v.y;   m[1][2] = v.z;   m[1][3] = 0.0f;
    m[2][0] = n.x;   m[2][1] = n.y;   m[2][2] = n.z;   m[2][3] = 0.0f;
    m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
  }

  void Matrix4f::InitPersProjTransform(const PerspProjInfo& p)
  {
    const float ar = p.Width / p.Height;
    const float zRange = p.zNear - p.zFar;
    const float tanHalfFOV = tanf(ToRadian(p.FOV / 2.0f));

    const float m00 = 1.f / (tanHalfFOV * ar);
    const float m11 = 1.f / tanHalfFOV;
    const float m22 = (-p.zNear - p.zFar) / zRange;
    const float m23 = 2.f * p.zFar * p.zNear / zRange;

    m[0][0] = m00; m[0][1] = 0.f; m[0][2] = 0.f; m[0][3] = 0.f;
    m[1][0] = 0.f; m[1][1] = m11; m[1][2] = 0.f; m[1][3] = 0.f;
    m[2][0] = 0.f; m[2][1] = 0.f; m[2][2] = m22; m[2][3] = m23;
    m[3][0] = 0.f; m[3][1] = 0.f; m[3][2] = 1.f; m[3][3] = 0.f;
  }


  void Matrix4f::InitOrthoProjTransform(const OrthoProjInfo& p)
  {
    const float m00 = 2.0f / (p.r - p.l);
    const float m11 = 2.0f / (p.t - p.b);
    const float m22 = 2.0f / (p.f - p.n);
    const float m03 = -(p.r + p.l) / (p.r - p.l);
    const float m13 = -(p.t + p.b) / (p.t - p.b);
    const float m23 = -(p.f + p.n) / (p.f - p.n);

    m[0][0] = m00; m[0][1] = 0.f; m[0][2] = 0.f; m[0][3] = m03;
    m[1][0] = 0.f; m[1][1] = m11; m[1][2] = 0.f; m[1][3] = m13;
    m[2][0] = 0.f; m[2][1] = 0.f; m[2][2] = m22; m[2][3] = m23;
    m[3][0] = 0.f; m[3][1] = 0.f; m[3][2] = 0.f; m[3][3] = 1.f;
  }


  float Matrix4f::Determinant() const
  {
    return
        m[0][0] * m[1][1] * m[2][2] * m[3][3] - m[0][0] * m[1][1] * m[2][3] * m[3][2] + m[0][0] * m[1][2] * m[2][3] * m[3][1] - m[0][0] * m[1][2] * m[2][1] * m[3][3]
      + m[0][0] * m[1][3] * m[2][1] * m[3][2] - m[0][0] * m[1][3] * m[2][2] * m[3][1] - m[0][1] * m[1][2] * m[2][3] * m[3][0] + m[0][1] * m[1][2] * m[2][0] * m[3][3]
      - m[0][1] * m[1][3] * m[2][0] * m[3][2] + m[0][1] * m[1][3] * m[2][2] * m[3][0] - m[0][1] * m[1][0] * m[2][2] * m[3][3] + m[0][1] * m[1][0] * m[2][3] * m[3][2]
      + m[0][2] * m[1][3] * m[2][0] * m[3][1] - m[0][2] * m[1][3] * m[2][1] * m[3][0] + m[0][2] * m[1][0] * m[2][1] * m[3][3] - m[0][2] * m[1][0] * m[2][3] * m[3][1]
      + m[0][2] * m[1][1] * m[2][3] * m[3][0] - m[0][2] * m[1][1] * m[2][0] * m[3][3] - m[0][3] * m[1][0] * m[2][1] * m[3][2] + m[0][3] * m[1][0] * m[2][2] * m[3][1]
      - m[0][3] * m[1][1] * m[2][2] * m[3][0] + m[0][3] * m[1][1] * m[2][0] * m[3][2] - m[0][3] * m[1][2] * m[2][0] * m[3][1] + m[0][3] * m[1][2] * m[2][1] * m[3][0];
  }


  Matrix4f& Matrix4f::Inverse()
  {
    // Compute the reciprocal determinant
    const float det = Determinant();
    if (det == 0.0f)
    {
      // Matrix not invertible. Setting all elements to nan is not really
      // correct in a mathematical sense but it is easy to debug for the
      // programmer.
      /*const float nan = std::numeric_limits<float>::quiet_NaN();
      *this = Matrix4f(
        nan,nan,nan,nan,
        nan,nan,nan,nan,
        nan,nan,nan,nan,
        nan,nan,nan,nan);*/
      assert(0);
      return *this;
    }

    float invdet = 1.0f / det;

    Matrix4f res;
    res.m[0][0] =  invdet * (m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
    res.m[0][1] = -invdet * (m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[0][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
    res.m[0][2] =  invdet * (m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][1] - m[1][1] * m[3][3]) + m[0][3] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]));
    res.m[0][3] = -invdet * (m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][1] - m[1][1] * m[2][3]) + m[0][3] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]));
    res.m[1][0] = -invdet * (m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
    res.m[1][1] =  invdet * (m[0][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
    res.m[1][2] = -invdet * (m[0][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][2] - m[1][2] * m[3][0]));
    res.m[1][3] =  invdet * (m[0][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]));
    res.m[2][0] =  invdet * (m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[1][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
    res.m[2][1] = -invdet * (m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[0][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
    res.m[2][2] =  invdet * (m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) + m[0][1] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
    res.m[2][3] = -invdet * (m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) + m[0][1] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
    res.m[3][0] = -invdet * (m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[1][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[1][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
    res.m[3][1] =  invdet * (m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[0][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[0][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
    res.m[3][2] = -invdet * (m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) + m[0][1] * (m[1][2] * m[3][0] - m[1][0] * m[3][2]) + m[0][2] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
    res.m[3][3] =  invdet * (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) + m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
    *this = res;

    return *this;
  }

  Quaternion::Quaternion(const float _x, const float _y, const float _z, const float _w)
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


  Quaternion Quaternion::Conjugate() const
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


  Vector3f Quaternion::ToDegrees()
  {
    float f[3];

    f[0] = atan2(x * z + y * w, x * w - y * z);
    f[1] = acos(-x * x - y * y - z * z - w * w);
    f[2] = atan2(x * z - y * w, x * w + y * z);

    f[0] = ToDegree(f[0]);
    f[1] = ToDegree(f[1]);
    f[2] = ToDegree(f[2]);

    return Vector3f(f);
  }

  Matrix4f::Matrix4f(const aiMatrix4x4& assimpMatrix)
  {
    m[0][0] = assimpMatrix.a1; m[0][1] = assimpMatrix.a2; m[0][2] = assimpMatrix.a3; m[0][3] = assimpMatrix.a4;
    m[1][0] = assimpMatrix.b1; m[1][1] = assimpMatrix.b2; m[1][2] = assimpMatrix.b3; m[1][3] = assimpMatrix.b4;
    m[2][0] = assimpMatrix.c1; m[2][1] = assimpMatrix.c2; m[2][2] = assimpMatrix.c3; m[2][3] = assimpMatrix.c4;
    m[3][0] = assimpMatrix.d1; m[3][1] = assimpMatrix.d2; m[3][2] = assimpMatrix.d3; m[3][3] = assimpMatrix.d4;
  }

  Matrix4f::Matrix4f(const aiMatrix3x3& assimpMatrix)
  {
    m[0][0] = assimpMatrix.a1; m[0][1] = assimpMatrix.a2; m[0][2] = assimpMatrix.a3; m[0][3] = 0.0f;
    m[1][0] = assimpMatrix.b1; m[1][1] = assimpMatrix.b2; m[1][2] = assimpMatrix.b3; m[1][3] = 0.0f;
    m[2][0] = assimpMatrix.c1; m[2][1] = assimpMatrix.c2; m[2][2] = assimpMatrix.c3; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
  }

  Matrix4f::Matrix4f( float a00, float a01, float a02, float a03,    float a10, float a11, float a12, float a13, 
                      float a20, float a21, float a22, float a23,    float a30, float a31, float a32, float a33)
  {
    m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
    m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
    m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
    m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
  }

  void Matrix4f::SetZero()
  {
    ZERO_MEM(m);
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

    for (uint32_t i = 0; i < 4; i++) {
      for (uint32_t j = 0; j < 4; j++) {
        Ret.m[i][j] = 
          m[i][0] * Right.m[0][j] + m[i][1] * Right.m[1][j] +
          m[i][2] * Right.m[2][j] + m[i][3] * Right.m[3][j];
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

  Matrix4f::operator const float*() const
  {
    return &(m[0][0]);
  }

  void Matrix4f::Print() const
  {
    for (const auto e : m)
      printf("%f %f %f %f\n", e[0], e[1], e[2], e[3]);
  }


  float RandomFloat()
  {
    return (static_cast<float>(RANDOM()) / RAND_MAX);
  }
}
