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

#include "t34_pipeline.h"

namespace t34 {
  const Matrix4f& Pipeline::GetVPTrans()
  {
    Matrix4f cameraTranslationTrans;
    Matrix4f cameraRotateTrans;
    Matrix4f perspProjTrans;

    cameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
    cameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
    perspProjTrans.InitPersProjTransform(m_persProjInfo);

    m_VPTtransformation = perspProjTrans * cameraRotateTrans * cameraTranslationTrans;
    return m_VPTtransformation;
  }

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
    GetVPTrans();

    m_WVPtransformation = m_VPTtransformation * m_WorldTransformation;
    return m_WVPtransformation;
  }
}