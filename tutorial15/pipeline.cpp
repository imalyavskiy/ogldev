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

// Установка параметров преобразования масштабирования
void Pipeline::Scale(const float x, const float y, const float z)
{
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
}

// Установка параметров преобразования сдвига
void Pipeline::WorldPos(const float x, const float y, const float z)
{
	m_worldPos.x = x;
	m_worldPos.y = y;
	m_worldPos.z = z;
}

// Установка параметров преобразования поворота 
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

// Установка параметров камеры: позиция, направление "взгляда", направление вверх.
void Pipeline::SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
{
	m_camera.Pos = Pos;
	m_camera.Target = Target;
	m_camera.Up = Up;
}

// Вычисление матрицы преобразования
const Matrix4f& Pipeline::GetTrans()
{
	// Вычисляем матрицу масштабирования
    Matrix4f ScaleTrans;
	Matrix4f::InitScaleTransform(ScaleTrans, m_scale.x, m_scale.y, m_scale.z);
	
	// Вычисляем матрицу поворота
	Matrix4f RotateTrans;
	Matrix4f::InitRotateTransform(RotateTrans, m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
	
	// Вычисляем матрицу сдвига
	Matrix4f TranslationTrans;
	Matrix4f::InitTranslationTransform(TranslationTrans, m_worldPos.x, m_worldPos.y, m_worldPos.z);

	// Вычисление матрицы описывающей положение камеры
	Matrix4f CameraTranslationTrans;
	Matrix4f::InitTranslationTransform(CameraTranslationTrans, -m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
	
	// Вычисление матрицы описывающей ориентацию камеры
	Matrix4f CameraRotateTrans;
	Matrix4f::InitCameraTransform(CameraRotateTrans, m_camera.Target, m_camera.Up);

	// Вычисляем матрицу перспективной проекции
	Matrix4f PersProjTrans;
	Matrix4f::InitPersProjTransform(PersProjTrans, m_persProj.fov, m_persProj.w, m_persProj.h, m_persProj.zn, m_persProj.zf);

	// Последовательность действий всегда такова:
	// 1. Масштабирование :		m_transformation = ScaleTrans;
	// 2. Поворот					:		m_transformation = RotateTrans * m_transformation;
	// 3. Сдвиг						:		m_transformation = TranslationTrans * m_transformation;
	// 4. Двиг камеры			:		m_transformation = CameraTranslationTrans * m_transformation;
	// 5. Поворот камеры  :		m_transformation = CameraRotateTrans * m_transformation;
	// 6. Проекция				:		m_transformation = PersProjTrans * m_transformation;
	// что то же самое, что и ниже

	// Производим умножение матриц
    m_transformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;

	// результат
	return m_transformation;
}


