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

#include <string>
#include <cmath>

#ifndef M_PI
#define M_PI       3.14159265358979323846f   // pi
#endif

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

namespace t13
{
  struct Vector3f
  {
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;

    Vector3f() = default;

    Vector3f(float _x, float _y, float _z);

    Vector3f Cross(const Vector3f& v) const;

    Vector3f& Normalize();

    static Vector3f Normalized(const Vector3f& v);

    void Print() const;
  };

  class Matrix4f
  {
  public:
    float m[4][4] = { {0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 0.f, 0.f} };
    std::string name_;

    static void InitIdentity(Matrix4f& m);

    Matrix4f operator*(const Matrix4f& Right) const;

    static void InitScaleTransform(Matrix4f& m, const float x, const float y, const float z);
	
    static void InitRotateTransform(Matrix4f& m, const float x, const float y, const float z);
	
    static void InitTranslationTransform(Matrix4f& m, const float x, const float y, const float z);
	
    static void InitCameraTransform(Matrix4f& m, const Vector3f& target, const Vector3f& up);
	
    static void InitPersProjTransform(Matrix4f& m, const float fov, const float w, const float h, const float zn, const float zf);
  };
}

#endif	/* MATH_3D_H */

