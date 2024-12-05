#ifndef CAMERA_H
#define	CAMERA_H

#include "t29_math_3d.h"

namespace t29
{
  class Camera
  {
  public:

    Camera(int winWidth, int winHeight);

    Camera(int winWidth, int winHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up);

    bool OnKeyboard(int key);

    void OnMouse(int x, int y);

    void OnRender();

    const Vector3f& GetPos() const;

    const Vector3f& GetTarget() const;

    const Vector3f& GetUp() const;

  private:

    void Init();
    void Update();

    Vector3f m_pos;
    Vector3f m_target;
    Vector3f m_up;

    int m_winWidth;
    int m_winHeight;

    float m_hAngle;
    float m_vAngle;

    Vector2i m_mousePos;
  };
}

#endif	/* CAMERA_H */

