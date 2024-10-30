#include "t21_pipeline.h"

namespace t21
{
  const Matrix4f& Pipeline::GetWorldTrans()
  {
    Matrix4f scaleTrans;
    Matrix4f rotateTrans;
    Matrix4f translationTrans;

    scaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
    rotateTrans.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
    translationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);

    m_WorldTransformation = translationTrans * rotateTrans * scaleTrans;

    return m_WorldTransformation;
  }

  const Matrix4f& Pipeline::GetWVPTrans()
  {
    GetWorldTrans();

    Matrix4f cameraTranslationTrans;
    Matrix4f cameraRotateTrans;
    Matrix4f perspectiveProjTrans;

    cameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
    cameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
    perspectiveProjTrans.InitPersProjTransform(m_persProj.FOV, m_persProj.Width, m_persProj.Height, m_persProj.zNear, m_persProj.zFar);

    m_WVPtransformation = perspectiveProjTrans * cameraRotateTrans * cameraTranslationTrans * m_WorldTransformation;

    return m_WVPtransformation;
  }
}
