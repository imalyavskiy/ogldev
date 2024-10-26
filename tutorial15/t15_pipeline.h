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

#include "t15_math_3d.h"

class Pipeline
{
public:
  Pipeline();

  void Scale(const float x, const float y, const float z);

  void WorldPos(const float x, const float y, const float z);

  void Rotate(const float x, const float y, const float z);

  void SetPerspectiveProj(const float fov, const float w, const float h, const float zn, const float zf);

  void SetCamera(const Vector3f& pos, const Vector3f& target, const Vector3f& up);

  const Matrix4f& GetTrans();

protected:
  Vector3f m_scale;

  Vector3f m_worldPos;

  Vector3f m_rotateInfo;

  struct {
    float fov;
    float w;
    float h;
    float zn;
    float zf;
  } m_persProj;

  struct {
    Vector3f Pos;

    Vector3f Target;

    Vector3f Up;
  } m_camera;

  Matrix4f m_transformation;
};

#endif	/* PIPELINE_H */

