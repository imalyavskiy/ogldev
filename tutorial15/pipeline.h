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
//  класс реализующий матричный конвейер
//////////////////////////////////////////////////////////////////////////
class Pipeline
{
public:
	//  конструктор
    Pipeline();

	// установка коэффициентов масштабирований
    void Scale(const float x, const float y, const float z);

    // установка параметров преобразований сдвига
    void WorldPos(const float x, const float y, const float z);

	// установка поворота вокруг осей(в градусах)
    void Rotate(const float x, const float y, const float z);

	// установка параметров перспективной проекции(угол обзора, ширина вьюпорта, высота вьюпорта, ближняя отсекаюшая плоскость, дальняя отсекающая плоскость)
    void SetPerspectiveProj(const float fov, const float w, const float h, const float zn, const float zf);
	
	// установка параметров ориентации камеры
	void SetCamera(const Vector3f& pos, const Vector3f& target, const Vector3f& up);

	// вычисление матрицы преобразования
    const Matrix4f& GetTrans();

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
	
	// параметры ориентации камеры
    struct {
		// позиция
		Vector3f Pos;
		
		// направление "взгляда"
		Vector3f Target;
		
		// направление "вверх"
		Vector3f Up;
    } m_camera;

	// матрица преобразования
    Matrix4f m_transformation;
};


#endif	/* PIPELINE_H */

