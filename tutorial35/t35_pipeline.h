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

#ifndef PIPELINE_H
#define	PIPELINE_H

#include "t35_math_3d.h"

namespace t35
{
  class Pipeline
  {
  public:
    Pipeline()
    {
      m_scale = Vector3f(1.0f, 1.0f, 1.0f);
      m_worldPos = Vector3f(0.0f, 0.0f, 0.0f);
      m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
    }

    void Scale(float scaleX, float scaleY, float scaleZ)
    {
      m_scale.x = scaleX;
      m_scale.y = scaleY;
      m_scale.z = scaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
      m_worldPos.x = x;
      m_worldPos.y = y;
      m_worldPos.z = z;
    }

    void WorldPos(const Vector3f& pos)
    {
      m_worldPos = pos;
    }

    void Rotate(float rotateX, float rotateY, float rotateZ)
    {
      m_rotateInfo.x = rotateX;
      m_rotateInfo.y = rotateY;
      m_rotateInfo.z = rotateZ;
    }

    void SetPerspectiveProj(const PerspProjInfo& perspProjInfo)
    {
      m_perspProjInfo = perspProjInfo;
    }

    void SetCamera(const Vector3f& pos, const Vector3f& target, const Vector3f& up)
    {
      m_camera.Pos = pos;
      m_camera.Target = target;
      m_camera.Up = up;
    }

    const Matrix4f& GetVPTrans();
    const Matrix4f& GetWVPTrans();
    const Matrix4f& GetWorldTrans();

  private:
    Vector3f m_scale;
    Vector3f m_worldPos;
    Vector3f m_rotateInfo;

    PerspProjInfo m_perspProjInfo;

    struct {
      Vector3f Pos;
      Vector3f Target;
      Vector3f Up;
    } m_camera;

    Matrix4f m_WVPtransformation;
    Matrix4f m_VPTtransformation;
    Matrix4f m_WorldTransformation;
  };
}
#endif	/* PIPELINE_H */

