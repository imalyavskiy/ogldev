#include <GL/freeglut.h>

#include "t20_camera.h"

namespace t20
{
  constexpr static float STEP_SCALE = 0.1f;
  constexpr static int MARGIN = 10;

  Camera::Camera(int winWidth, int winHeight)
  {
    m_windowWidth = winWidth;
    m_windowHeight = winHeight;
    m_pos = Vector3f(0.0f, 0.0f, 0.0f);

    m_target = Vector3f(0.0f, 0.0f, 1.0f);
    m_target.Normalize();

    m_up = Vector3f(0.0f, 1.0f, 0.0f);
    m_up.Normalize();

    Init();
  }

  Camera::Camera(int winWidth, int winHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up)
  {
    m_windowWidth = winWidth;
    m_windowHeight = winHeight;
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
        m_hAngle = 360.0f - ToDegree(asinf(HTarget.z));
      else
        m_hAngle = 180.0f + ToDegree(asinf(HTarget.z));
    }
    else
    {
      if (HTarget.x >= 0.0f)
        m_hAngle = ToDegree(asinf(-HTarget.z));
      else
        m_hAngle = 90.0f + ToDegree(asinf(-HTarget.z));
    }

    m_vAngle = -ToDegree(asinf(m_target.y));

    m_mousePos.x = m_windowWidth / 2;
    m_mousePos.y = m_windowHeight / 2;

    glutWarpPointer(m_mousePos.x, m_mousePos.y);
  }


  bool Camera::OnKeyboard(int Key)
  {
    switch (Key) {
      case GLUT_KEY_UP:
        {
          m_pos += (m_target * STEP_SCALE);
          return true;
        }
      case GLUT_KEY_DOWN:
        {
          m_pos -= (m_target * STEP_SCALE);
          return true;
        }
      case GLUT_KEY_LEFT:
        {
          Vector3f Left = m_target.Cross(m_up);
          Left.Normalize();
          Left *= STEP_SCALE;
          m_pos += Left;
          return true;
        }
      case GLUT_KEY_RIGHT:
        {
          Vector3f Right = m_up.Cross(m_target);
          Right.Normalize();
          Right *= STEP_SCALE;
          m_pos += Right;
          return true;
        }
    }

    return false;
  }

  void Camera::OnMouse(int x, int y)
  {
    if ((x == m_mousePos.x) && (y == m_mousePos.y)) return;

    const int dx = x - m_mousePos.x;
    const int dy = y - m_mousePos.y;

    m_hAngle += dx / 20.0f;
    m_vAngle += dy / 20.0f;

    Update();
    glutWarpPointer(m_mousePos.x, m_mousePos.y);
  }

  void Camera::OnRender()
  {
    constexpr bool shouldUpdate = false;
    if (shouldUpdate) {
      Update();
    }
  }

  void Camera::Update()
  {
    const Vector3f vAxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vector3f view(1.0f, 0.0f, 0.0f);
    view.Rotate(m_hAngle, vAxis);
    view.Normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f hAxis = vAxis.Cross(view);
    hAxis.Normalize();
    view.Rotate(m_vAngle, hAxis);

    m_target = view;
    m_target.Normalize();

    m_up = m_target.Cross(hAxis);
    m_up.Normalize();
  }
}
