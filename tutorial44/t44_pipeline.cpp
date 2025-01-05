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

#include "t44_pipeline.h"

namespace t44 {
  const Matrix4f& Pipeline::GetProjTrans()
  {
    m_projTransform.InitPersProjTransform(m_persProjInfo);
    return m_projTransform;
  }


  const Matrix4f& Pipeline::GetVPTrans()
  {
    GetViewTrans();
    GetProjTrans();

    m_viewProjTransform = m_projTransform * m_viewTransform;
    return m_viewProjTransform;
  }

  const Matrix4f& Pipeline::GetWorldTrans()
  {
    Matrix4f scaleTrans, rotateTrans, translationTrans;

    scaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
    rotateTrans.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
    translationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);

    m_worldTransform = translationTrans * rotateTrans * scaleTrans;
    return m_worldTransform;
  }

  const Matrix4f& Pipeline::GetViewTrans()
  {
    Matrix4f cameraTranslationTrans, cameraRotateTrans;

    cameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
    cameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);

    m_viewTransform = cameraRotateTrans * cameraTranslationTrans;

    return m_viewTransform;
  }

  const Matrix4f& Pipeline::GetWVPTrans()
  {
    GetWorldTrans();
    GetVPTrans();

    m_worldViewProjTransform = m_viewProjTransform * m_worldTransform;
    return m_worldViewProjTransform;
  }


  const Matrix4f& Pipeline::GetWVOrthoPTrans()
  {
    GetWorldTrans();
    GetViewTrans();

    Matrix4f P;
    P.InitOrthoProjTransform(m_orthoProjInfo);

    m_worldViewProjTransform = P * m_viewTransform * m_worldTransform;
    return m_worldViewProjTransform;
  }


  const Matrix4f& Pipeline::GetWVTrans()
  {
    GetWorldTrans();
    GetViewTrans();

    m_worldViewTransform = m_viewTransform * m_worldTransform;
    return m_worldViewTransform;
  }


  void Pipeline::Scale(float s)
  {
    Scale(s, s, s);
  }

  void Pipeline::Scale(const Vector3f& scale)
  {
    Scale(scale.x, scale.y, scale.z);
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

  void Pipeline::Rotate(const Vector3f& r)
  {
    Rotate(r.x, r.y, r.z);
  }

  void Pipeline::SetPerspectiveProj(const PerspProjInfo& p)
  {
    m_persProjInfo = p;
  }

  void Pipeline::SetOrthographicProj(const OrthoProjInfo& p)
  {
    m_orthoProjInfo = p;
  }

  void Pipeline::SetCamera(const Vector3f& pos, const Vector3f& target, const Vector3f& up)
  {
    m_camera.Pos = pos;
    m_camera.Target = target;
    m_camera.Up = up;
  }

  void Pipeline::SetCamera(const Camera& camera)
  {
    SetCamera(camera.GetPos(), camera.GetTarget(), camera.GetUp());
  }

  void Pipeline::Orient(const Orientation& o)
  {
    m_scale = o.m_scale;
    m_worldPos = o.m_pos;
    m_rotateInfo = o.m_rotation;
  }

  const Matrix4f& Pipeline::GetWPTrans()
  {
    Matrix4f perspProjTrans;

    GetWorldTrans();
    perspProjTrans.InitPersProjTransform(m_persProjInfo);

    m_worldProjTransform = perspProjTrans * m_worldTransform;
    return m_worldProjTransform;
  }
}