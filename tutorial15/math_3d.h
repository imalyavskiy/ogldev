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

// TODO: comment
struct Vector2i
{
	int x;
	int y;
};

// Класс представляющий собой реализацию математического 3D вектора на базе float
struct Vector3f
{
	// Координата X
	float x;

	// Координата Y
	float y;

	// Координата Z
	float z;

	// Конструктор
    Vector3f()
		: x( 0 )
		, y( 0 )
		, z( 0 )
    {
    }

	// Конструктор
	Vector3f(float _x, float _y, float _z)
		: x( _x )
		, y( _y )
		, z( _z )
    {
    }

	// Оператор сложения векторов
    Vector3f& operator+=(const Vector3f& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;

        return *this;
    }

	// Оператор вычитания векторов
    Vector3f& operator-=(const Vector3f& r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;

        return *this;
    }

	// Оператор умножения вектора на скаляр
    Vector3f& operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

	//////////////////////////////////////////////////////////////////////////
	/// Cross Product - векторное произведение векторов
	/// пусть a(x, y, z) и b(x, y, z) вектора тогда(для правостронней системы координат):
	/// [a,b] = ( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x )
	///                 |  i   j   k  |
	/// [a,b] условно = | a.x a.y a.z | = i * a.y * b.z + k * a.x * b.y + j * a.z * b.x - k * a.y * b.x - i * a.z * b.y - j * a.x * b.z =
	///                 | b.x b.y b.z |
	/// = i * ( a.y * b.z - a.z * b.y ) + j * ( a.z * b.x - a.x * b.z ) + k * ( a.x * b.y - a.y * b.x )
	/// правый множитель i - х координата результирующего вектора, j - y, k - z.
	/// для левосторонней системы координат определитель берётся со знаком "-"
	Vector3f Cross(const Vector3f& v) const;

	// Нормализация вектора
	/// приведение вектора к единичной длине
	Vector3f& Normalize();

	// TODO: comment
	void Rotate(float Angle, const Vector3f& Axis);

	// TODO: comment
	void Print() const
	{
		printf("(%.02f, %.02f, %.02f)", x, y, z);
	}
};

// Оператор сложения векторов
inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
{
	return Vector3f{ l.x + r.x
				   , l.y + r.y
				   , l.z + r.z
				   };
}

// Оператор вычитания векторов
inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
{
    return Vector3f{ l.x - r.x
				   , l.y - r.y
				   , l.z - r.z
				   };
}

// Оператор умножения ветокра на скаляр
inline Vector3f operator*(const Vector3f& l, float f)
{
	return Vector3f{ l.x * f
				   , l.y * f
				   , l.z * f
				   };
}

// Класс 2D матрицы 4х4 из float'ов
class Matrix4f
{
public:
	// Непосредственно матрица
    float m[4][4];

	// Инициализация единичной матрицей
	static void InitIdentity(Matrix4f& m);

	// Умножение на матрицу справа
	Matrix4f operator*(const Matrix4f& Right) const;

	// Инициализаия матрицы масштабирования
	static void InitScaleTransform(Matrix4f& m, const float x, const float y, const float z);
	
	// Инициализация матрицы поворота
	static void InitRotateTransform(Matrix4f& m, const float x, const float y, const float z);
	
	// Инициализация матрицы сдвига
	static void InitTranslationTransform(Matrix4f& m, const float x, const float y, const float z);
	
	// Инициализация UVN матрицы камеры
	static void InitCameraTransform(Matrix4f& m, const Vector3f& target, const Vector3f& up);
	
	// Инициализация матрицы преобразования перспективной проекции
	static void InitPersProjTransform(Matrix4f& m, const float fov, const float w, const float h, const float zn, const float zf);
};


// TODO: comment
struct Quaternion
{
	// TODO: comment
	float x, y, z, w;

	// TODO: comment
	Quaternion(float _x, float _y, float _z, float _w);

	// TODO: comment
	void Normalize();

	// TODO: comment
	Quaternion Conjugate();
};

// TODO: comment
Quaternion operator*(const Quaternion& l, const Quaternion& r);

// TODO: comment
Quaternion operator*(const Quaternion& q, const Vector3f& v);

#endif	/* MATH_3D_H */

