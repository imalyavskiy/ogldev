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

#include "t14_pipeline.h"

namespace t14
{
  Pipeline::Pipeline()
   : m_scale(1.0f, 1.0f, 1.0f)
   , m_worldPos(0.0f, 0.0f, 0.0f)
   , m_rotateInfo(0.0f, 0.0f, 0.0f)
  {
  }

  void Pipeline::Scale(const float x, const float y, const float z)
  {
    m_scale.x = x;
    m_scale.y = y;
    m_scale.z = z;
  }

  void Pipeline::WorldPos(const float x, const float y, const float z)
  {
    m_worldPos.x = x;
    m_worldPos.y = y;
    m_worldPos.z = z;
  }

  void Pipeline::Rotate(const float x, const float y, const float z)
  {
    m_rotateInfo.x = x;
    m_rotateInfo.y = y;
    m_rotateInfo.z = z;
  }

  void Pipeline::SetPerspectiveProj(const float fov, const float w, const float h, const float zn, const float zf)
  {
    m_persProj.fov = fov;
    m_persProj.w = w;
    m_persProj.h = h;
    m_persProj.zn = zn;
    m_persProj.zf = zf;
  }

  void Pipeline::SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
  {
    m_camera.Pos = Pos;
    m_camera.Target = Target;
    m_camera.Up = Up;
  }

  const Matrix4f& Pipeline::GetTrans()
  {
    Matrix4f ScaleTrans;
    Matrix4f::InitScaleTransform(ScaleTrans, m_scale.x, m_scale.y, m_scale.z);

    Matrix4f RotateTrans;
    Matrix4f::InitRotateTransform(RotateTrans, m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);

    Matrix4f TranslationTrans;
    Matrix4f::InitTranslationTransform(TranslationTrans, m_worldPos.x, m_worldPos.y, m_worldPos.z);

    Matrix4f CameraTranslationTrans;
    Matrix4f::InitTranslationTransform(CameraTranslationTrans, -m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);

    Matrix4f CameraRotateTrans;
    Matrix4f::InitCameraTransform(CameraRotateTrans, m_camera.Target, m_camera.Up);

    Matrix4f PersProjTrans;
    Matrix4f::InitPersProjTransform(PersProjTrans, m_persProj.fov, m_persProj.w, m_persProj.h, m_persProj.zn, m_persProj.zf);

    m_transformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;

    return m_transformation;
  }
}
