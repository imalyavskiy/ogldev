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

#ifndef PIPELINE_H
#define	PIPELINE_H

#include "math_3d.h"

//////////////////////////////////////////////////////////////////////////
/// Класс реализующий матричный конфейер
//////////////////////////////////////////////////////////////////////////
class Pipeline
{
public:
	// Конструктор
    Pipeline();
	
	// Установка коэффициентов масштабирования
    void Scale(const float x, const float y, const float z);

	// Установка параметров преобразоваания сдвига
    void WorldPos(const float x, const float y, const float z);

	// Установка поворота вокруг осей(в градусах)
    void Rotate(const float x, const float y, const float z);

	// Установка параметров перспективной проекции(угол обзора, ширина вьюпортаб высота вьюпорта, ближняя отсекаюшая плоскость, дальняя отсекающая плоскость)
	void SetPerspectiveProj(const float fov, const float w, const float h, const float zn, const float zf);

	// Вычисление матрицы преобразования
    const Matrix4f& GetTrans();

protected:
	// Инициализация матрицы масштабирования
    void InitScaleTransform(Matrix4f& m) const;
	
	// Инициализация общей матрицы поворота
    void InitRotateTransform(Matrix4f& m) const;
	
	// Инициализация матрицы сдвига
    void InitTranslationTransform(Matrix4f& m) const;
	
	// Инициализация матрицы перспективной проекции
    void InitPerspectiveProj(Matrix4f& m) const;

protected:
	// параметры масштабирования
    Vector3f m_scale;
	
	// параметры сдвига
    Vector3f m_worldPos;
	
	// параметры поворота
    Vector3f m_rotateInfo;

	// параметры перспективной проекции
    struct {
        float fov;
        float w;	// ширина
        float h;	// высота
        float zn;	// z координата ближней отсекающей плоскости
        float zf;	// z координата дальней отсекающей плоскости
    } m_persProj;

	// матрица преобразования
    Matrix4f m_transformation;
};


#endif	/* PIPELINE_H */

