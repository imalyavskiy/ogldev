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

#ifndef CAMERA_H
#define	CAMERA_H

#include "math_3d.h"

// Класс реализации свободной камеры
class Camera
{
public:

	// Конструктор
	Camera(int WindowWidth, int WindowHeight);

	// Конструктор
	Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

	// Обработчик нажатия кнопки
	bool OnKeyboard(int Key);

	// TODO: comment
	void OnMouse(int x, int y);

	// TODO: comment
	void OnRender();

	// Get'тер текущей позиции камеры
	const Vector3f& GetPos() const;

	// Get'тер текущего направления камеры
	const Vector3f& GetTarget() const;

	// Get'тер текущего направления "вверх" камеры
	const Vector3f& GetUp() const;

private:

	// TODO: comment
	  void Init();

	// TODO: comment
	void Update();

	// Текущая позиция камеры
	Vector3f m_pos;

	// Текущее направление камеры
	Vector3f m_target;

	// Текущее направление "вверх" камеры
	Vector3f m_up;

	// TODO: comment
	int m_windowWidth;
	// TODO: comment
	int m_windowHeight;

	// TODO: comment
	float m_AngleH;
	// TODO: comment
	float m_AngleV;

	// TODO: comment
	bool m_OnUpperEdge;
	// TODO: comment
	bool m_OnLowerEdge;
	// TODO: comment
	bool m_OnLeftEdge;
	// TODO: comment
	bool m_OnRightEdge;

	// TODO: comment
	Vector2i m_mousePos;
};

#endif	/* CAMERA_H */

