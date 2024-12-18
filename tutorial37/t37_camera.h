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

#include "t37_math_3d.h"


namespace t37
{
  class Camera
  {
  public:

    Camera(int winWidth, int winHeight);

    Camera(int winWidth, int winHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up);

    bool OnKeyboard(int key);

    void OnMouse(int x, int y);

    void OnRender();

    const Vector3f& GetPos() const
    {
      return m_pos;
    }

    const Vector3f& GetTarget() const
    {
      return m_target;
    }

    const Vector3f& GetUp() const
    {
      return m_up;
    }

  private:

    void Init();
    void Update();

    int m_winWidth;
    int m_winHeight;

    Vector3f m_pos = { 0.0f, 0.0f, 0.0f };
    Vector3f m_target = { 0.0f, 0.0f, 1.0f };
    Vector3f m_up = { 0.0f, 1.0f, 0.0f };

    float m_hAngle = 0.f;
    float m_vAngle = 0.f;

    bool m_onUpperEdge = false;
    bool m_onLowerEdge = false;
    bool m_onLeftEdge = false;
    bool m_onRightEdge = false;

    Vector2i m_mousePos = { 0,0 };
  };
}
#endif	/* CAMERA_H */

