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

#include "t15_math_3d.h"

namespace t15
{
  class Camera
  {
  public:

    Camera(int WindowWidth, int WindowHeight);

    Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

    bool OnKeyboard(int Key);

    void OnMouse(int x, int y);

    void OnRender();

    const Vector3f& GetPos() const;

    const Vector3f& GetTarget() const;

    const Vector3f& GetUp() const;

  private:

    void Init();

    void Update();

    const int m_MARGIN;

    Vector3f m_pos;

    Vector3f m_target;

    Vector3f m_up;

    int m_windowWidth;
    int m_windowHeight;

    float m_HAngle;
    float m_VAngle;

    bool m_OnTEdge; // Top
    bool m_OnBEdge; // Bottom
    bool m_OnLEdge; // Left
    bool m_OnREdge; // Right

    Vector2i m_mousePos;
  };
}
#endif	/* CAMERA_H */

