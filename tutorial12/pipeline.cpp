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

#include "pipeline.h"

// Конструктор
Pipeline::Pipeline()
	: m_scale	  (1.0f, 1.0f, 1.0f)
	, m_worldPos  (0.0f, 0.0f, 0.0f)
	, m_rotateInfo(0.0f, 0.0f, 0.0f)
{
}

// Установка параметров преобразоваания масштабирования
void Pipeline::Scale(const float ScaleX, const float ScaleY, const float ScaleZ)
{
	m_scale.x = ScaleX;
	m_scale.y = ScaleY;
	m_scale.z = ScaleZ;
}

// Установка параметров преобразоваания сдвига
void Pipeline::WorldPos(const float x, const float y, const float z)
{
	m_worldPos.x = x;
	m_worldPos.y = y;
	m_worldPos.z = z;
}

// Установка параметров преобразоваания поворота 
void Pipeline::Rotate(const float x, const float y, const float z)
{
	m_rotateInfo.x = x;
	m_rotateInfo.y = y;
	m_rotateInfo.z = z;
}

// Установка параметров перспективной проекции
void Pipeline::SetPerspectiveProj(const float fov, const float w, const float h, const float zn, const float zf)
{
	m_persProj.fov	= fov;
	m_persProj.w	= w;
	m_persProj.h	= h;
	m_persProj.zn	= zn;
	m_persProj.zf	= zf;
}

// Инициализация матрицы масштабирования
void Pipeline::InitScaleTransform(Matrix4f& m) const
{
	m = Matrix4f
	{
        m_scale.x  ,     0.0f     ,      0.0f     ,      0.0f     ,
          0.0f     ,  m_scale.y   ,      0.0f     ,      0.0f     ,
          0.0f     ,     0.0f     ,   m_scale.z   ,      0.0f     ,
          0.0f     ,     0.0f     ,      0.0f     ,      1.0f     
	};
}

// Инициализация общей матрицы поворота
void Pipeline::InitRotateTransform(Matrix4f& m) const
{
    const float x = ToRadian(m_rotateInfo.x);
    const float y = ToRadian(m_rotateInfo.y);
    const float z = ToRadian(m_rotateInfo.z);

	// Транспонированная матрица поворота в плоскости YOZ
    Matrix4f rx 
	{
	     1.0f,     0.0f,      0.0f,     0.0f,
	     0.0f,    cosf(x),  -sinf(x),   0.0f,
	     0.0f,    sinf(x),   cosf(x),   0.0f,
	     0.0f,     0.0f,      0.0f,     1.0f,
	};

	// Транспонированная матрица поворота в плоскости XOZ
	Matrix4f ry
	{
	    cosf(y),   0.0f,    -sinf(y),   0.0f,
	     0.0f  ,   1.0f,     0.0f  ,    0.0f,
		sinf(y),   0.0f,    cosf(y),    0.0f,
		 0.0f  ,   0.0f,     0.0f  ,    1.0f,
	};

	// Транспонированная матрица поворота в плоскости XOY
	Matrix4f rz
	{
	    cosf(z), -sinf(z),   0.0f,      0.0f,
	    sinf(z),  cosf(z),   0.0f,      0.0f,
	     0.0f  ,   0.0f  ,   1.0f,      0.0f,
	     0.0f  ,   0.0f  ,   0.0f,      1.0f,
	};

    m = rz * ry * rx;
}

// Инициализация матрицы сдвига
void Pipeline::InitTranslationTransform(Matrix4f& m) const
{
	// Транспонированная матрица сдвига
	m = Matrix4f
	{
			1.0f    ,     0.0f    ,     0.0f    , m_worldPos.x,
			0.0f    ,     1.0f    ,     0.0f    , m_worldPos.y,
			0.0f    ,     0.0f    ,     1.0f    , m_worldPos.z,
			0.0f    ,     0.0f    ,     0.0f    ,     1.0f    ,
	};
}

// Инициализация матрицы перспективной проекции
void Pipeline::InitPerspectiveProj(Matrix4f& m) const
{
	const float ar			= m_persProj.w / m_persProj.h;
	const float zNear		= m_persProj.zn;
	const float zFar		= m_persProj.zf;
	const float zRange		= zFar - zNear;
	const float ctanHalfFOV	= 1.0f / tanf(ToRadian(m_persProj.fov / 2.0f));

	m = Matrix4f
	{
			ctanHalfFOV / ar,			0.0f,				0.0f,						0.0,
				0.0f,				ctanHalfFOV,			0.0f,						0.0,
				0.0f,					0.0f,		(zNear + zFar) / zRange,	-2.0f * zFar * zNear / zRange,
				0.0f,					0.0f,				1.0f,						0.0,
	};
}

// Вычисление матрицы преобразования
const Matrix4f& Pipeline::GetTrans()
{
	// Вычисляем матрицу масштабирования
    Matrix4f ScaleTrans;
	InitScaleTransform(ScaleTrans);

	// Вычисляем матрицу поворота
	Matrix4f RotateTrans;
    InitRotateTransform(RotateTrans);

	// Вычисляем матрицу сдвига
	Matrix4f TranslationTrans;
    InitTranslationTransform(TranslationTrans);

	// Вычисляем матрицу перспективной проекции
	Matrix4f PersProjTrans;
    InitPerspectiveProj(PersProjTrans);

	// Последовательность действий всегда такова:
	// 1. Масштабирование
	// 2. Поворот
	// 3. Сдвиг
	// 4. Проекция
	// m_transformation = ScaleTrans;
	// m_transformation = RotateTrans * m_transformation;
	// m_transformation = TranslationTrans * m_transformation;
	// m_transformation = PersProjTrans * m_transformation;
	// что то же самое, что и ниже

	// Производим умножение матриц
    m_transformation = PersProjTrans * TranslationTrans * RotateTrans * ScaleTrans;

	// результат
    return m_transformation;
}


