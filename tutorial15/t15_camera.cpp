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

  Camera::Camera(int winWidth, int winHeight)
    : m_MARGIN(winWidth / 3)
    , m_winWidth(winWidth)
    , m_winHeight(winHeight)
  {
    m_pos = Vector3f(0.0f, 0.0f, 0.0f);
    m_target = Vector3f(0.0f, 0.0f, 1.0f);
    m_up = Vector3f(0.0f, 1.0f, 0.0f);

    m_target.Normalize();

    Init();
  }

  Camera::Camera(int winWidth, int winHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up)
    : m_MARGIN(winWidth / 3)
    , m_pos(pos)
    , m_target(target)
    , m_up(up)
    , m_winWidth(winWidth)
    , m_winHeight(winHeight)
  {
    m_target.Normalize();
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
        m_hAngle = 360.0f - toDegree(asinf(hTarget.z));
      else
      if (hTarget.x < 0.0f)
        m_hAngle = 180.0f + toDegree(asinf(hTarget.z));
    }
    else
    if (hTarget.z < 0.0f)
    {
      if (hTarget.x >= 0.0f)
        m_hAngle = toDegree(asinf((-1.f) * hTarget.z));
      else
      if (hTarget.x < 0.0f)
        m_hAngle = 90.0f + toDegree(asinf((-1.f) * hTarget.z));
    }

    m_vAngle = (-1.f) * toDegree(asinf(m_target.y));

    m_onTEdge = false;
    m_onBEdge = false;
    m_onLEdge = false;
    m_onREdge = false;
    m_mousePos.x = m_winWidth / 2;
    m_mousePos.y = m_winHeight / 2;

    glutWarpPointer(m_mousePos.x, m_mousePos.y);
  }

  bool Camera::OnKeyboard(int key)
  {
    bool Ret = false;

    switch (key)
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

    m_hAngle += dx / 20.0f;
    m_vAngle += dy / 20.0f;

    if (0 == dx) {
      if (x <= m_MARGIN) {
        m_onLEdge = true;
      }
      else if (x >= (m_winWidth - m_MARGIN)) {
        m_onREdge = true;
      }
    }
    else {
      m_onLEdge = false;
      m_onREdge = false;
    }

    if (0 == dy) {
      if (y <= m_MARGIN) {
        m_onTEdge = true;
      }
      else if (y >= (m_winHeight - m_MARGIN)) {
        m_onBEdge = true;
      }
    }
    else {
      m_onTEdge = false;
      m_onBEdge = false;
    }

    Update();
  }

  void Camera::OnRender()
  {
    bool shouldUpdate = false;

    if (m_onLEdge) {
      m_hAngle -= 0.1f;
      shouldUpdate = true;
    }
    else if (m_onREdge) {
      m_hAngle += 0.1f;
      shouldUpdate = true;
    }

    if (m_onTEdge) {
      if (m_vAngle > -90.0f) {
        m_vAngle -= 0.1f;
        shouldUpdate = true;
      }
    }
    else if (m_onBEdge) {
      if (m_vAngle < 90.0f) {
        m_vAngle += 0.1f;
        shouldUpdate = true;
      }
    }

    if (shouldUpdate) {
      Update();
    }
  }

  void Camera::Update()
  {
    const Vector3f vAxis(0.0f, 1.0f, 0.0f);

    Vector3f view(1.0f, 0.0f, 0.0f);
    view.Rotate(m_hAngle, vAxis);
    view.Normalize();

    Vector3f hAxis = vAxis.Cross(view);
    hAxis.Normalize();
    view.Rotate(m_vAngle, hAxis);

    m_target = view;
    m_target.Normalize();

    m_up = m_target.Cross(hAxis);
    m_up.Normalize();
  }
}