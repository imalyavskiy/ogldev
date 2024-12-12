/*

	Copyright 2010 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "t32_pipeline.h"

namespace t32 {
  Pipeline::Pipeline()
  {
    m_scale = Vector3f(1.0f, 1.0f, 1.0f);
    m_worldPos = Vector3f(0.0f, 0.0f, 0.0f);
    m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
  }

  void Pipeline::Scale(float scaleX, float scaleY, float scaleZ)
  {
    m_scale.x = scaleX;
    m_scale.y = scaleY;
    m_scale.z = scaleZ;
  }

  void Pipeline::WorldPos(float x, float y, float z)
  {
    m_worldPos.x = x;
    m_worldPos.y = y;
    m_worldPos.z = z;
  }

  void Pipeline::WorldPos(const Vector3f& pos)
  {
    m_worldPos = pos;
  }

  void Pipeline::Rotate(float rotateX, float rotateY, float rotateZ)
  {
    m_rotateInfo.x = rotateX;
    m_rotateInfo.y = rotateY;
    m_rotateInfo.z = rotateZ;
  }

  void Pipeline::SetPerspectiveProj(const PersProjInfo& perspProjectionInfo)
  {
    m_persProjInfo = perspProjectionInfo;
  }

  void Pipeline::SetCamera(const Vector3f& pos, const Vector3f& target, const Vector3f& up)
  {
    m_camera.Pos = pos;
    m_camera.Target = target;
    m_camera.Up = up;
  }

  auto Pipeline::GetVPTrans() -> const Matrix4f&
  {
    Matrix4f cameraTranslationTrans;
    Matrix4f cameraRotateTrans;
    Matrix4f persProjTrans;

    cameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
    cameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
    persProjTrans.InitPersProjTransform(m_persProjInfo);

    m_VPTtransformation = persProjTrans * cameraRotateTrans * cameraTranslationTrans;
    return m_VPTtransformation;
  }

  auto Pipeline::GetWorldTrans() -> const Matrix4f&
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

  auto Pipeline::GetWVPTrans() -> const Matrix4f&
  {
    GetWorldTrans();
    GetVPTrans();

    m_WVPtransformation = m_VPTtransformation * m_WorldTransformation;
    return m_WVPtransformation;
  }
}
