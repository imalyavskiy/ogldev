#include "t24_pipeline.h"
namespace t24
{
  const Matrix4f& Pipeline::GetWorldTrans()
  {
    Matrix4f ScaleTrans;
    Matrix4f RotateTrans;
    Matrix4f TranslationTrans;

    ScaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
    RotateTrans.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
    TranslationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);

    m_WorldTransformation = TranslationTrans * RotateTrans * ScaleTrans;
    return m_WorldTransformation;
  }

  const Matrix4f& Pipeline::GetWVPTrans()
  {
    GetWorldTrans();

    Matrix4f CameraTranslationTrans;
    Matrix4f CameraRotateTrans;
    Matrix4f PersProjTrans;

    CameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
    CameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
    PersProjTrans.InitPersProjTransform(m_persProj.FOV, m_persProj.Width, m_persProj.Height, m_persProj.zNear, m_persProj.zFar);

    m_WVPtransformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * m_WorldTransformation;
    return m_WVPtransformation;
  }
}
