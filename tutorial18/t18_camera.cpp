#include <cstdint>
#include <GL/freeglut.h>

#include "t18_camera.h"
namespace t18
{
  const static float STEP_SCALE = 0.1f;
  const static int MARGIN = 10;

  Camera::Camera(int32_t winWidth, int32_t winHeight)
  : m_winWidth(winWidth)
  , m_winHeight(winHeight)
  , m_position(Vector3f(0.0f, 0.0f, 0.0f))
  , m_target(Vector3f(0.0f, 0.0f, 1.0f).Normalize())
  , m_lookup(Vector3f(0.0f, 1.0f, 0.0f))
  {
    Init();
  }


  Camera::Camera(int32_t winWidth, int32_t winHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
  : m_winWidth(winWidth)
  , m_winHeight(winHeight)
  , m_position(Pos)
  , m_target(Target)
  , m_lookup(Up)
  {
    m_target.Normalize();
    m_lookup.Normalize();

    Init();
  }


  void Camera::Init()
  {
    Vector3f HTarget(m_target.x, 0.0, m_target.z);
    HTarget.Normalize();

    if (HTarget.z >= 0.0f)
    {
      if (HTarget.x >= 0.0f)
        m_AngleH = 360.0f - ToDegree(asinf(HTarget.z));
      else
        m_AngleH = 180.0f + ToDegree(asinf(HTarget.z));
    }
    else
    {
      if (HTarget.x >= 0.0f)
        m_AngleH = ToDegree(asinf(-HTarget.z));
      else
        m_AngleH = 90.0f + ToDegree(asinf(-HTarget.z));
    }

    m_AngleV = -ToDegree(asinf(m_target.y));

    m_mousePos.x  = m_winWidth / 2;
    m_mousePos.y  = m_winHeight / 2;

    glutWarpPointer(m_mousePos.x, m_mousePos.y);
  }


  bool Camera::OnKeyboard(int32_t Key)
  {
    bool Ret = false;

    switch (Key) {
      case GLUT_KEY_UP: {
          m_position += (m_target * STEP_SCALE);
          Ret = true;
        }
        break;

      case GLUT_KEY_DOWN: {
          m_position -= (m_target * STEP_SCALE);
          Ret = true;
        }
        break;

      case GLUT_KEY_LEFT: {
          Vector3f Left = m_target.Cross(m_lookup);
          Left.Normalize();
          Left *= STEP_SCALE;
          m_position += Left;
          Ret = true;
        }
        break;

      case GLUT_KEY_RIGHT: {
          Vector3f Right = m_lookup.Cross(m_target);
          Right.Normalize();
          Right *= STEP_SCALE;
          m_position += Right;
          Ret = true;
        }
        break;
    }

    return Ret;
  }


  void Camera::OnMouse(int32_t x, int32_t y)
  {
    if (( x == m_mousePos.x) && (y == m_mousePos.y)) 
      return;

    const int32_t dx = x - m_mousePos.x;
    const int32_t dy = y - m_mousePos.y;

    m_AngleH += static_cast<float>(dx) / 20.0f;
    m_AngleV += static_cast<float>(dy) / 20.0f;

    Update();

    glutWarpPointer(m_mousePos.x, m_mousePos.y);
  }


  void Camera::OnRender()
  {
    bool ShouldUpdate = false;

    if (ShouldUpdate) {
      Update();
    }
  }

  const Vector3f& Camera::GetPos() const
  {
    return m_position;
  }

  const Vector3f& Camera::GetTarget() const
  {
    return m_target;
  }

  const Vector3f& Camera::GetUp() const
  {
    return m_lookup;
  }

  void Camera::Update()
  {
    const Vector3f VAxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vector3f View(1.0f, 0.0f, 0.0f);
    View.Rotate(m_AngleH, VAxis);
    View.Normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f HAxis = VAxis.Cross(View);
    HAxis.Normalize();
    View.Rotate(m_AngleV, HAxis);

    m_target = View;
    m_target.Normalize();

    m_lookup = m_target.Cross(HAxis);
    m_lookup.Normalize();
  }
}
