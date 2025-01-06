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

#include "t44_camera.h"

namespace t44 {
  const static float STEP_SCALE = 1.0f;
  const static float EDGE_STEP = 0.5f;
  const static int MARGIN = 10;

  Camera::Camera(int winWidth, int winHeight)
  {
    m_winWidth = winWidth;
    m_winHeight = winHeight;
    m_pos = Vector3f(0.0f, 0.0f, 0.0f);
    m_target = Vector3f(0.0f, 0.0f, 1.0f);
    m_target.Normalize();
    m_up = Vector3f(0.0f, 1.0f, 0.0f);

    Init();
  }


  Camera::Camera(int winWidth, int winHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up)
  {
    m_winWidth = winWidth;
    m_winHeight = winHeight;
    m_pos = pos;

    m_target = target;
    m_target.Normalize();

    m_up = up;
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
      {
        m_hAngle = 360.0f - ToDegree(asinf(HTarget.z));
      }
      else
      {
        m_hAngle = 180.0f + ToDegree(asinf(HTarget.z));
      }
    }
    else
    {
      if (HTarget.x >= 0.0f)
      {
        m_hAngle = ToDegree(asinf(-HTarget.z));
      }
      else
      {
        m_hAngle = 90.0f + ToDegree(asinf(-HTarget.z));
      }
    }

    m_vAngle = -ToDegree(asinf(m_target.y));

    m_OnUpperEdge = false;
    m_OnLowerEdge = false;
    m_OnLeftEdge = false;
    m_OnRightEdge = false;
    m_mousePos.x = m_winWidth / 2;
    m_mousePos.y = m_winHeight / 2;

    // glutWarpPointer(m_mousePos.x, m_mousePos.y);
  }


  bool Camera::OnKeyboard(KEYBOARD_KEY key)
  {
    bool Ret = false;

    switch (key) {

    case KEYBOARD_KEY::UP:
    {
      m_pos += (m_target * STEP_SCALE);
      Ret = true;
    }
    break;

    case KEYBOARD_KEY::DOWN:
    {
      m_pos -= (m_target * STEP_SCALE);
      Ret = true;
    }
    break;

    case KEYBOARD_KEY::LEFT:
    {
      Vector3f Left = m_target.Cross(m_up);
      Left.Normalize();
      Left *= STEP_SCALE;
      m_pos += Left;
      Ret = true;
    }
    break;

    case KEYBOARD_KEY::RIGHT:
    {
      Vector3f Right = m_up.Cross(m_target);
      Right.Normalize();
      Right *= STEP_SCALE;
      m_pos += Right;
      Ret = true;
    }
    break;

    case KEYBOARD_KEY::PAGE_UP:
      m_pos.y += STEP_SCALE;
      break;

    case KEYBOARD_KEY::PAGE_DN:
      m_pos.y -= STEP_SCALE;
      break;

    default:
      break;
    }

    return Ret;
  }


  void Camera::OnMouse(int x, int y)
  {
    const int DeltaX = x - m_mousePos.x;
    const int DeltaY = y - m_mousePos.y;

    m_mousePos.x = x;
    m_mousePos.y = y;

    m_hAngle += (float)DeltaX / 20.0f;
    m_vAngle += (float)DeltaY / 20.0f;

    if (DeltaX == 0) {
      if (x <= MARGIN) {
        //    m_hAngle -= 1.0f;
        m_OnLeftEdge = true;
      }
      else if (x >= (m_winWidth - MARGIN)) {
        //    m_hAngle += 1.0f;
        m_OnRightEdge = true;
      }
    }
    else {
      m_OnLeftEdge = false;
      m_OnRightEdge = false;
    }

    if (DeltaY == 0) {
      if (y <= MARGIN) {
        m_OnUpperEdge = true;
      }
      else if (y >= (m_winHeight - MARGIN)) {
        m_OnLowerEdge = true;
      }
    }
    else {
      m_OnUpperEdge = false;
      m_OnLowerEdge = false;
    }

    Update();
  }


  void Camera::OnRender()
  {
    bool ShouldUpdate = false;

    if (m_OnLeftEdge) {
      m_hAngle -= EDGE_STEP;
      ShouldUpdate = true;
    }
    else if (m_OnRightEdge) {
      m_hAngle += EDGE_STEP;
      ShouldUpdate = true;
    }

    if (m_OnUpperEdge) {
      if (m_vAngle > -90.0f) {
        m_vAngle -= EDGE_STEP;
        ShouldUpdate = true;
      }
    }
    else if (m_OnLowerEdge) {
      if (m_vAngle < 90.0f) {
        m_vAngle += EDGE_STEP;
        ShouldUpdate = true;
      }
    }

    if (ShouldUpdate) {
      Update();
    }
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

  void Camera::Update()
  {
    const Vector3f Vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vector3f View(1.0f, 0.0f, 0.0f);
    View.Rotate(m_hAngle, Vaxis);
    View.Normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f Haxis = Vaxis.Cross(View);
    Haxis.Normalize();
    View.Rotate(m_vAngle, Haxis);

    m_target = View;
    m_target.Normalize();

    m_up = m_target.Cross(Haxis);
    m_up.Normalize();
  }
}
