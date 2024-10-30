#ifndef CAMERA_H
#define	CAMERA_H

#include "t21_math_3d.h"

namespace t21
{
  class Camera
  {
  public:

    Camera(int winWidth, int winHeight);

    Camera(int winWidth, int winHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up);

    bool OnKeyboard(int key);

    void OnMouse(int x, int y);

    void OnRender();

    [[nodiscard]]
    const Vector3f& GetPos() const
    {
      return m_pos;
    }

    [[nodiscard]]
    const Vector3f& GetTarget() const
    {
      return m_target;
    }

    [[nodiscard]]
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

    int m_windowWidth;
    int m_windowHeight;

    float m_hAngle = 0.f;
    float m_vAngle = 0.f;

    Vector2i m_mousePos;
  };
}

#endif	/* CAMERA_H */
