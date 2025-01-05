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

#include "t44_math_3d.h"
#include "t44_keys.h"
#include "t44_atb.h"

namespace t44 {
  class Camera
  {
  public:

    Camera(int winWidth, int winHeight);

    Camera(int winWidth, int winHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up);

    bool OnKeyboard(KEYBOARD_KEY key);

    void OnMouse(int x, int y);

    void OnRender();

    const Vector3f& GetPos() const;

    const Vector3f& GetTarget() const;

    const Vector3f& GetUp() const;

    void AddToATB(TwBar* bar);

  private:

    void Init();
    void Update();

    Vector3f m_pos;
    Vector3f m_target;
    Vector3f m_up;

    int m_winWidth;
    int m_winHeight;

    float m_hAngle;
    float m_vAngle;

    bool m_OnUpperEdge;
    bool m_OnLowerEdge;
    bool m_OnLeftEdge;
    bool m_OnRightEdge;

    Vector2i m_mousePos;
  };
}
#endif	/* CAMERA_H */

