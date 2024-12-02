#ifndef CAMERA_H
#define	CAMERA_H

#include "t27_math_3d.h"

namespace t27
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
    const Vector3f& GetPos() const;

    [[nodiscard]]
    const Vector3f& GetTarget() const;

    [[nodiscard]]
    const Vector3f& GetUp() const;

  private:

    void Init();
    void Update();

    int m_winWidth;
    int m_winHeight;

    Vector3f m_pos;
    Vector3f m_target;
    Vector3f m_up;

    float m_hAngle;
    float m_vAngle;

    Vector2i m_mousePos;
  };
}

#endif	/* CAMERA_H */

