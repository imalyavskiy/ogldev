#ifndef CAMERA_H
#define	CAMERA_H

#include "t20_math_3d.h"

namespace t20
{
  class Camera
  {
  public:

    Camera(int winWidth, int winHeight);

    Camera(int winWidth, int winHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up);

    bool OnKeyboard(int Key);

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

    Vector3f m_pos;
    Vector3f m_target;
    Vector3f m_up;

    int m_windowWidth = 0;
    int m_windowHeight = 0;

    float m_hAngle = 0.f;
    float m_vAngle = 0.f;

    Vector2i m_mousePos;
  };
}

#endif	/* CAMERA_H */

