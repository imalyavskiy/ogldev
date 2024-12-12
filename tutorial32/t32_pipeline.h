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

#include "t32_math_3d.h"

namespace t32 {
  class Pipeline
  {
  public:
    Pipeline();

    void Scale(float scaleX, float scaleY, float scaleZ);

    void WorldPos(float x, float y, float z);

    void WorldPos(const Vector3f& pos);

    void Rotate(float rotateX, float rotateY, float rotateZ);

    void SetPerspectiveProj(const PersProjInfo& perspProjectionInfo);

    void SetCamera(const Vector3f& pos, const Vector3f& target, const Vector3f& up);

    auto GetVPTrans() -> const Matrix4f&;

    auto GetWVPTrans() -> const Matrix4f&;

    auto GetWorldTrans() -> const Matrix4f&;

  private:
    Vector3f m_scale;
    Vector3f m_worldPos;
    Vector3f m_rotateInfo;

    PersProjInfo m_persProjInfo;

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

