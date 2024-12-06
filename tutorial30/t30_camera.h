#ifndef CAMERA_H
#define	CAMERA_H

#include "t30_math_3d.h"

namespace t30
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

    int m_winWidth = 0;
    int m_winHeight = 0;

    Vector3f m_pos{0.0f, 0.0f, 0.0f};
    Vector3f m_target{0.0f, 0.0f, 1.0f};
    Vector3f m_up{ 0.0f, 1.0f, 0.0f };

    float m_hAngle;
    float m_vAngle;

    Vector2i m_mousePos;
  };
}

#endif	/* CAMERA_H */

