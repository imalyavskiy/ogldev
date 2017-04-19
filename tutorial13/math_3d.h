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
#include <math.h>

#ifndef M_PI
#define M_PI       3.14159265358979323846f   // pi
#endif

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

// ����� �������������� ����� ���������� ��������������� 3D ������� �� ���� float
struct Vector3f
{
    float x;
    float y;
    float z;

    Vector3f()
		: x( 0 )
		, y( 0 )
		, z( 0 )
    {
    }

    Vector3f(float _x, float _y, float _z)
		: x( _x )
		, y( _y )
		, z( _z )
    {
    }

	//////////////////////////////////////////////////////////////////////////
	/// Cross Product - ��������� ������������ ��������
	/// ����� a(x, y, z) � b(x, y, z) ������� �����(��� ������������� ������� ���������):
	/// [a,b] = ( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x )
	///                 |  i   j   k  |
	/// [a,b] ������� = | a.x a.y a.z | = i * a.y * b.z + k * a.x * b.y + j * a.z * b.x - k * a.y * b.x - i * a.z * b.y - j * a.x * b.z =
	///                 | b.x b.y b.z |
	/// = i * ( a.y * b.z - a.z * b.y ) + j * ( a.z * b.x - a.x * b.z ) + k * ( a.x * b.y - a.y * b.x )
	/// ������ ��������� i - � ���������� ��������������� �������, j - y, k - z.
	/// ��� ������������� ������� ��������� ������������ ������ �� ������ "-"
	Vector3f Cross(const Vector3f& v) const;

	// ������������ �������
	/// ���������� ������� � ��������� ������
	Vector3f& Normalize();

    void Print() const
    {
        printf("(%.02f, %.02f, %.02f", x, y, z);
    }
};

// ����� 2D ������� 4�4 �� float'��
class Matrix4f
{
public:
    float m[4][4];

	// ������������� ��������� ��������
	static void InitIdentity(Matrix4f& m);

	// ��������� �� ������� ������
	Matrix4f operator*(const Matrix4f& Right) const;

	// ������������ ������� ���������������
	static void InitScaleTransform(Matrix4f& m, const float x, const float y, const float z);
	
	// ������������� ������� ��������
	static void InitRotateTransform(Matrix4f& m, const float x, const float y, const float z);
	
	// ������������� ������� ������
	static void InitTranslationTransform(Matrix4f& m, const float x, const float y, const float z);
	
	// ������������� UVN ������� ������
	static void InitCameraTransform(Matrix4f& m, const Vector3f& target, const Vector3f& up);
	
	// ������������� ������� �������������� ������������� ��������
	static void InitPersProjTransform(Matrix4f& m, const float fov, const float w, const float h, const float zn, const float zf);
};


#endif	/* MATH_3D_H */

