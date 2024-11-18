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

#include "t16_camera.h"

namespace t16
{
  const static float StepScale = 0.1f;

  const static int MARGIN = 10;

  Camera::Camera(int WindowWidth, int WindowHeight)
  {
    m_winWidth = WindowWidth;
    m_winHeight = WindowHeight;
    m_pos = Vector3f(0.0f, 0.0f, 0.0f);
    m_target = Vector3f(0.0f, 0.0f, 1.0f);
    m_target.Normalize();
    m_up = Vector3f(0.0f, 1.0f, 0.0f);

    Init();
  }


  Camera::Camera(int winWidth, int winHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up)
  {
    m_pos = pos;

    m_winWidth = winWidth;
    m_winHeight = winHeight;

    m_target = target;
    m_target.Normalize();
    m_up = up;
    m_up.Normalize();

    Init();
  }

  void Camera::Init()
  {
    Vector3f hTarget(m_target.x, 0.0, m_target.z);
    hTarget.Normalize();

    if (hTarget.z >= 0.0f)
    {
      if (hTarget.x >= 0.0f)
        m_AngleH = 360.0f - ToDegree(asinf(hTarget.z));
      else
        m_AngleH = 180.0f + ToDegree(asinf(hTarget.z));
    }
    else
    {
      if (hTarget.x >= 0.0f)
        m_AngleH = ToDegree(asinf(-hTarget.z));
      else
        m_AngleH = 90.0f + ToDegree(asinf(-hTarget.z));
    }

    m_AngleV = -ToDegree(asinf(m_target.y));





    m_mousePos.x = m_winWidth / 2;
    m_mousePos.y = m_winHeight / 2;

    glutWarpPointer(m_mousePos.x, m_mousePos.y);
  }

  bool Camera::OnKeyboard(int key)
  {
    bool ret = false;

    switch (key)
    {
      case GLUT_KEY_UP: {
        m_pos += (m_target * StepScale);
        ret = true;
      }
      break;

      case GLUT_KEY_DOWN: {
        m_pos -= (m_target * StepScale);
        ret = true;
      }
      break;

      case GLUT_KEY_LEFT: {
        Vector3f Left = m_target.Cross(m_up);
        Left.Normalize();
        Left *= StepScale;
        m_pos += Left;
        ret = true;
      }
      break;

      case GLUT_KEY_RIGHT: {
        Vector3f Right = m_up.Cross(m_target);
        Right.Normalize();
        Right *= StepScale;
        m_pos += Right;
        ret = true;
      }
      break;
    }

    return ret;
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

    if ((dx == 0) && (dy == 0)) 
      return;

    m_AngleH += dx / 20.0f;
    m_AngleV += dy / 20.0f;
    
    Update();
    glutWarpPointer(m_winWidth / 2, m_winHeight / 2);
  }


  void Camera::OnRender()
  {
    const bool shouldUpdate = false;

    if (shouldUpdate) {
      Update();
    }
  }

  void Camera::Update()
  {
    const Vector3f vAxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vector3f view(1.0f, 0.0f, 0.0f);
    view.Rotate(m_AngleH, vAxis);
    view.Normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f hAxis = vAxis.Cross(view);
    hAxis.Normalize();
    view.Rotate(m_AngleV, hAxis);

    m_target = view;
    m_target.Normalize();

    m_up = m_target.Cross(hAxis);
    m_up.Normalize();
  }
}