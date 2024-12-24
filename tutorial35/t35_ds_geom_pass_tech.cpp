/*

  Copyright 2011 Etay Meiri

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

#include <climits>
#include <cstring>

#include "t35_ds_geom_pass_tech.h"
#include "t35_util.h"

namespace t35
{
  DSGeomPassTech::DSGeomPassTech()
  {
  }

  bool DSGeomPassTech::Init()
  {
    if (!Technique::Init())
      return false;

    if (!AddShader(GL_VERTEX_SHADER, "shaders/geometry_pass.vs"))
      return false;

    if (!AddShader(GL_FRAGMENT_SHADER, "shaders/geometry_pass.fs"))
      return false;

    if (!Finalize())
      return false;

    m_WVPLocation = GetUniformLocation("gWVP");
    if (m_WVPLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_WorldMatrixLocation = GetUniformLocation("gWorld");
    if (m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_colorTextureUnitLocation = GetUniformLocation("gColorMap");
    if (m_colorTextureUnitLocation == INVALID_UNIFORM_LOCATION)
      return false;

    return true;
  }


  void DSGeomPassTech::SetWVP(const Matrix4f& worldViewProjection)
  {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(worldViewProjection.m));
  }


  void DSGeomPassTech::SetWorldMatrix(const Matrix4f& world)
  {
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(world.m));
  }


  void DSGeomPassTech::SetColorTextureUnit(uint32_t textureUnit)
  {
    glUniform1i(m_colorTextureUnitLocation, textureUnit);
  }
}