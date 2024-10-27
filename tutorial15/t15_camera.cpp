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

#include <GL/freeglut.h>

#include "t15_camera.h"

namespace t15
{
  const static float StepScale = 0.1f;

  Camera::Camera(int WindowWidth, int WindowHeight)
    : m_MARGIN(WindowWidth / 3)
  {
    m_pos = Vector3f(0.0f, 0.0f, 0.0f);
    m_target = Vector3f(0.0f, 0.0f, 1.0f);
    m_up = Vector3f(0.0f, 1.0f, 0.0f);

    m_windowWidth = WindowWidth;
    m_windowHeight = WindowHeight;
    m_target.Normalize();

    Init();
  }

  Camera::Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
    : m_MARGIN(WindowWidth / 3)
  {
    m_pos = Pos;

    m_windowWidth = WindowWidth;
    m_windowHeight = WindowHeight;

    m_target = Target;
    m_target.Normalize();
    m_up = Up;
    m_up.Normalize();

    Init();
  }

  void Camera::Init()
  {
    Vector3f HTarget(m_target.x, 0.0, m_target.z);
    HTarget.Normalize();

    if (HTarget.z >= 0.0f)
    {
      if (HTarget.x >= 0.0f)
        m_HAngle = 360.0f - ToDegree(asinf(HTarget.z));
      else
        m_HAngle = 180.0f + ToDegree(asinf(HTarget.z));
    }
    else
    {
      if (HTarget.x >= 0.0f)
        m_HAngle = ToDegree(asinf(-HTarget.z));
      else
        m_HAngle = 90.0f + ToDegree(asinf(-HTarget.z));
    }

    m_VAngle = -ToDegree(asinf(m_target.y));

    m_OnTEdge = false;
    m_OnBEdge = false;
    m_OnLEdge = false;
    m_OnREdge = false;
    m_mousePos.x = m_windowWidth / 2;
    m_mousePos.y = m_windowHeight / 2;

    glutWarpPointer(m_mousePos.x, m_mousePos.y);
  }

  bool Camera::OnKeyboard(int Key)
  {
    bool Ret = false;

    switch (Key)
    {
      case GLUT_KEY_UP: {
        m_pos += (m_target * StepScale);
        Ret = true;
      }
      break;

      case GLUT_KEY_DOWN: {
        m_pos -= (m_target * StepScale);
        Ret = true;
      }
      break;

      case GLUT_KEY_LEFT: {
        Vector3f Left = m_target.Cross(m_up);
        Left.Normalize();
        Left *= StepScale;
        m_pos += Left;
        Ret = true;
      }
      break;

      case GLUT_KEY_RIGHT: {
        Vector3f Right = m_up.Cross(m_target);
        Right.Normalize();
        Right *= StepScale;
        m_pos += Right;
        Ret = true;
      }
      break;
    }

    return Ret;
  }

  const Vector3f& Camera::GetPos() const
  {
    return m_pos;
  }

  const Vector3f& Camera::GetTarget() const
  {
    return m_target;
  }

  const Vector3f& Camera::GetUp() const
  {
    return m_up;
  }

  void Camera::OnMouse(int x, int y)
  {
    const int dx = x - m_mousePos.x;
    const int dy = y - m_mousePos.y;

    m_mousePos.x = x;
    m_mousePos.y = y;

    m_HAngle += dx / 20.0f;
    m_VAngle += dy / 20.0f;

    if (0 == dx) {
      if (x <= m_MARGIN) {
        m_OnLEdge = true;
      }
      else if (x >= (m_windowWidth - m_MARGIN)) {
        m_OnREdge = true;
      }
    }
    else {
      m_OnLEdge = false;
      m_OnREdge = false;
    }

    if (0 == dy) {
      if (y <= m_MARGIN) {
        m_OnTEdge = true;
      }
      else if (y >= (m_windowHeight - m_MARGIN)) {
        m_OnBEdge = true;
      }
    }
    else {
      m_OnTEdge = false;
      m_OnBEdge = false;
    }

    Update();
  }

  void Camera::OnRender()
  {
    bool ShouldUpdate = false;

    if (m_OnLEdge) {
      m_HAngle -= 0.1f;
      ShouldUpdate = true;
    }
    else if (m_OnREdge) {
      m_HAngle += 0.1f;
      ShouldUpdate = true;
    }

    if (m_OnTEdge) {
      if (m_VAngle > -90.0f) {
        m_VAngle -= 0.1f;
        ShouldUpdate = true;
      }
    }
    else if (m_OnBEdge) {
      if (m_VAngle < 90.0f) {
        m_VAngle += 0.1f;
        ShouldUpdate = true;
      }
    }

    if (ShouldUpdate) {
      Update();
    }
  }

  void Camera::Update()
  {
    const Vector3f VAxis(0.0f, 1.0f, 0.0f);

    Vector3f View(1.0f, 0.0f, 0.0f);
    View.Rotate(m_HAngle, VAxis);
    View.Normalize();

    Vector3f HAxis = VAxis.Cross(View);
    HAxis.Normalize();
    View.Rotate(m_VAngle, HAxis);

    m_target = View;
    m_target.Normalize();

    m_up = m_target.Cross(HAxis);
    m_up.Normalize();
  }
}