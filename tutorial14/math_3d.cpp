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

#include "math_3d.h"
#pragma region -- Vector3f --
// Cross Product - векторное произведение веторов
Vector3f Vector3f::Cross(const Vector3f& v) const
{
    const float _x = y * v.z - z * v.y;
    const float _y = z * v.x - x * v.z;
    const float _z = x * v.y - y * v.x;

    return Vector3f(_x, _y, _z);
}

// Ќормализаци€ вектора
Vector3f& Vector3f::Normalize()
{
    const float Length = sqrtf(x * x + y * y + z * z);

    x /= Length;
    y /= Length;
    z /= Length;

    return *this;
}
#pragma endregion
#pragma region -- Matrix4f --
// »нициализаци€ единичной матрицей
void Matrix4f::InitIdentity(Matrix4f& m)
{
	m = Matrix4f
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
}

// ”множение на матрицу справа
Matrix4f Matrix4f::operator*(const Matrix4f& r) const
{
	Matrix4f Ret;

	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < 4; j++) {
			Ret.m[i][j] = m[i][0] * r.m[0][j] + 
						  m[i][1] * r.m[1][j] + 
						  m[i][2] * r.m[2][j] + 
						  m[i][3] * r.m[3][j];
		}
	}

	return Ret;
}

// »нициализаи€ матрицы масштабировани€
void Matrix4f::InitScaleTransform(Matrix4f& m, const float x, const float y, const float z)
{
	// матрица транспонируетс€ зеркалированием относительно главной(\) диагонали, даннойм случае везде кроме главной длиагонали нули,
	// т.о. транспонированна€ и оригинальна€ матрицы тождественны
	m = Matrix4f
	{
		  x  ,  0.0f,  0.0f,  0.0f,
		 0.0f,   y  ,  0.0f,  0.0f,
		 0.0f,  0.0f,   z  ,  0.0f,
		 0.0f,  0.0f,  0.0f,  1.0f,
	};
}

// »нициализаци€ матрицы поворота
void Matrix4f::InitRotateTransform(Matrix4f& m, const float RotateX, const float RotateY, const float RotateZ)
{
	// ѕреобразование градусов в радианы
	const float x = ToRadian(RotateX);
    const float y = ToRadian(RotateY);
    const float z = ToRadian(RotateZ);

	// “ранспонированна€ матрица поворота в плоскости YOZ
	Matrix4f rx
	{
		1.0f,     0.0f,      0.0f,     0.0f,
		0.0f,    cosf(x),  -sinf(x),   0.0f,
		0.0f,    sinf(x),   cosf(x),   0.0f,
		0.0f,     0.0f,      0.0f,     1.0f,
	};

	// “ранспонированна€ матрица поворота в плоскости XOZ
	Matrix4f ry
	{
		cosf(y),   0.0f,    -sinf(y),   0.0f,
		 0.0f  ,   1.0f,     0.0f  ,    0.0f,
		sinf(y),   0.0f,    cosf(y),    0.0f,
		 0.0f  ,   0.0f,     0.0f  ,    1.0f,
	};

	// “ранспонированна€ матрица поворота в плоскости XOY
	Matrix4f rz
	{
		cosf(z), -sinf(z),   0.0f,      0.0f,
		sinf(z),  cosf(z),   0.0f,      0.0f,
		0.0f  ,   0.0f  ,    1.0f,      0.0f,
		0.0f  ,   0.0f  ,    0.0f,      1.0f,
	};

	// ѕроизводим вычисление общей матрицы поворота
    m = rz * ry * rx;
}

// »нициализаци€ матрицы сдвига
void Matrix4f::InitTranslationTransform(Matrix4f& m, const float x, const float y, const float z)
{
	// “ранспонированна€ матрица сдвига
	m = Matrix4f
	{
		1.0f, 0.0f, 0.0f,  x  ,
		0.0f, 1.0f, 0.0f,  y  ,
		0.0f, 0.0f, 1.0f,  z  ,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
}

// »нициализаци€ UVN матрицы камеры, где:
// N - вектор "взгл€да" камеры, т.н. "Looak At"
// V - вектор "вверх"
// U - вектор "право"
void Matrix4f::InitCameraTransform(Matrix4f& m, const Vector3f& target, const Vector3f& up)
{
    Vector3f N = target;
    Vector3f V = up;
	Vector3f U;
	
	// Ќормализаци€ вектора направлени€ "взгл€да"(привеление к 1 по модулю)
	N.Normalize();
	
	// Ќормализаци€ вектора "вверх"
	V.Normalize();
    
	// ¬ычисление векторного произведени€ векторов N и V - вектора "вправо"
	U = V.Cross(N);
    
	// ѕеревычисл€ем вектор V, как векторное произведение векторов N и U(U у нас теперь перпендикул€рен плоскости VN), и,
	// что очень ¬ј∆Ќќ - вектор V, даже если изначально он был (0, 1, 0, 0) и угол между V и N не был равен 90
	// теперь становитс€ перпендикул€рен плоскости NU, т.о. UVN образуют полноценный базис, где скал€рное произведение 
	// любой пары векторов даЄт 0
	V = N.Cross(U);

	// “ранспонированна€ матрица преобразовани€ мирового пространства в пространство камеры
	m = Matrix4f
	{
		 U.x,   U.y,   U.z,  0.0f,
		 V.x,   V.y,   V.z,  0.0f,
		 N.x,   N.y,   N.z,  0.0f,
		0.0f,  0.0f,  0.0f,  1.0f,
	};
}

// »нициализаци€ матрицы преобразовани€ перспективной проекции
void Matrix4f::InitPersProjTransform(Matrix4f& m, const float fov, const float w, const float h, const float zn, const float zf)
{
	const float ar		= w / h;
	const float zNear	= zn;
	const float zFar	= zf;
	const float zRange	= zf - zn;

	const float ctanHalfFOV = 1.0f / tanf(ToRadian(fov / 2.0f));

	m = Matrix4f
	{
	ctanHalfFOV / ar,			0.0f,				0.0f,						0.0,
		0.0f,				ctanHalfFOV,			0.0f,						0.0,
		0.0f,					0.0f,		(zNear + zFar) / zRange,	-2.0f * zFar * zNear / zRange,
		0.0f,					0.0f,				1.0f,						0.0,
	};
}
#pragma endregion

