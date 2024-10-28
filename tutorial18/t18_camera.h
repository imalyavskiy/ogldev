#ifndef CAMERA_H
#define	CAMERA_H

#include "t18_math_3d.h"

namespace t18
{
  class Camera
  {
  public:

    Camera(int32_t WindowWidth, int32_t WindowHeight);

    Camera(int32_t WindowWidth, int32_t WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

    bool OnKeyboard(int32_t Key);

    void OnMouse(int32_t x, int32_t y);

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

    Vector3f m_position;
    Vector3f m_target;
    Vector3f m_lookup;

    int m_winWidth;
    int m_winHeight;

    float m_AngleH;
    float m_AngleV;

    Vector2i m_mousePos;
  };
}

#endif	/* CAMERA_H */

