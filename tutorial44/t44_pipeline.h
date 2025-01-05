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

#include "t44_math_3d.h"
#include "t44_camera.h"

namespace t44 {
  struct Orientation
  {
    Vector3f m_scale = { 1.0f, 1.0f, 1.0f };
    Vector3f m_rotation = { 0.0f, 0.0f, 0.0f };
    Vector3f m_pos = { 0.0f, 0.0f, 0.0f };
  };


  class Pipeline
  {
  public:
    void Scale(float s);
    void Scale(const Vector3f& scale);
    void Scale(float scaleX, float scaleY, float scaleZ);

    void WorldPos(float x, float y, float z);
    void WorldPos(const Vector3f& pos);

    void Rotate(float rotateX, float rotateY, float rotateZ);
    void Rotate(const Vector3f& r);

    void SetPerspectiveProj(const PerspProjInfo& p);
    void SetOrthographicProj(const OrthoProjInfo& p);

    void SetCamera(const Vector3f& pos, const Vector3f& target, const Vector3f& up);
    void SetCamera(const Camera& camera);

    void Orient(const Orientation& o);

    const Matrix4f& GetWPTrans();
    const Matrix4f& GetWVTrans();
    const Matrix4f& GetVPTrans();
    const Matrix4f& GetWVPTrans();
    const Matrix4f& GetWVOrthoPTrans();
    const Matrix4f& GetWorldTrans();
    const Matrix4f& GetViewTrans();
    const Matrix4f& GetProjTrans();

  private:
    Vector3f m_scale = { 1.0f, 1.0f, 1.0f };
    Vector3f m_worldPos = { 0.0f, 0.0f, 0.0f };
    Vector3f m_rotateInfo = { 0.0f, 0.0f, 0.0f };

    PerspProjInfo m_persProjInfo;
    OrthoProjInfo m_orthoProjInfo;

    struct {
      Vector3f Pos;
      Vector3f Target;
      Vector3f Up;
    } m_camera;

    Matrix4f m_worldViewProjTransform;
    Matrix4f m_viewProjTransform;
    Matrix4f m_worldProjTransform;
    Matrix4f m_worldViewTransform;
    Matrix4f m_worldTransform;
    Matrix4f m_viewTransform;
    Matrix4f m_projTransform;
  };
}

#endif	/* PIPELINE_H */

