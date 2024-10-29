#ifndef PIPELINE_H
#define	PIPELINE_H

#include "t20_math_3d.h"

namespace t20
{
  class Pipeline
  {
  public:
    Pipeline();

    void Scale(float ScaleX, float ScaleY, float ScaleZ);

    void WorldPos(float x, float y, float z);

    void Rotate(float RotateX, float RotateY, float RotateZ);

    void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar);

    void SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

    const Matrix4f& GetWVPTrans();

    const Matrix4f& GetWorldTrans();

  private:
    Vector3f m_scale;
    Vector3f m_worldPos;
    Vector3f m_rotateInfo;

    struct PerspectiveProjection {
      float FOV = 0.f;
      float Width = 0.f;
      float Height = 0.f;
      float zNear = 0.f;
      float zFar = 0.f;
    };

    PerspectiveProjection m_perspProj;

    struct Camera {
      Vector3f Pos;
      Vector3f Target;
      Vector3f Up;
    };

    Camera m_camera;

    Matrix4f m_WVPTransform; // World + View + Projection
    Matrix4f m_WTransform;   // World
  };
}
#endif	/* PIPELINE_H */
