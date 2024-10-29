#include "t20_pipeline.h"

namespace t20
{
  Pipeline::Pipeline()
  {
    m_scale = Vector3f(1.0f, 1.0f, 1.0f);
    m_worldPos = Vector3f(0.0f, 0.0f, 0.0f);
    m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
  }

  void Pipeline::Scale(float ScaleX, float ScaleY, float ScaleZ)
  {
    m_scale.x = ScaleX;
    m_scale.y = ScaleY;
    m_scale.z = ScaleZ;
  }

  void Pipeline::WorldPos(float x, float y, float z)
  {
    m_worldPos.x = x;
    m_worldPos.y = y;
    m_worldPos.z = z;
  }

  void Pipeline::Rotate(float RotateX, float RotateY, float RotateZ)
  {
    m_rotateInfo.x = RotateX;
    m_rotateInfo.y = RotateY;
    m_rotateInfo.z = RotateZ;
  }

  void Pipeline::SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
  {
    m_perspProj.FOV    = FOV;
    m_perspProj.Width  = Width;
    m_perspProj.Height = Height;
    m_perspProj.zNear  = zNear;
    m_perspProj.zFar   = zFar;
  }

  void Pipeline::SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
  {
    m_camera.Pos = Pos;
    m_camera.Target = Target;
    m_camera.Up = Up;
  }

  const Matrix4f& Pipeline::GetWorldTrans()
  {
    Matrix4f scaleTrans;
    Matrix4f rotateTrans;
    Matrix4f translationTrans;

    scaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
    rotateTrans.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
    translationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);

    m_WTransform = translationTrans * rotateTrans * scaleTrans;
    return m_WTransform;
  }

  const Matrix4f& Pipeline::GetWVPTrans()
  {
    GetWorldTrans();

    Matrix4f cameraTranslationTrans;
    Matrix4f cameraRotateTrans;
    Matrix4f perspectiveProjTrans;

    cameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
    cameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
    perspectiveProjTrans.InitPersProjTransform(m_perspProj.FOV, m_perspProj.Width, m_perspProj.Height, m_perspProj.zNear, m_perspProj.zFar);

    m_WVPTransform = perspectiveProjTrans * cameraRotateTrans * cameraTranslationTrans * m_WTransform;
    return m_WVPTransform;
  }
}
