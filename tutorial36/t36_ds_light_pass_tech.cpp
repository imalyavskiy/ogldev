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

#include <limits.h>
#include <string.h>

#include "t36_ds_light_pass_tech.h"
#include "t36_util.h"

namespace t36
{
  DSLightPassTech::DSLightPassTech()
  {
  }

  bool DSLightPassTech::Init()
  {
    m_WVPLocation = GetUniformLocation("gWVP");
    if (m_WVPLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_posTextureUnitLocation = GetUniformLocation("gPositionMap");
    if (m_posTextureUnitLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_colorTextureUnitLocation = GetUniformLocation("gColorMap");
    if (m_colorTextureUnitLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_normalTextureUnitLocation = GetUniformLocation("gNormalMap");
    if (m_normalTextureUnitLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
    if (m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
    if (m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
    if (m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_screenSizeLocation = GetUniformLocation("gScreenSize");
    if (m_screenSizeLocation == INVALID_UNIFORM_LOCATION)
      return false;

    return true;
  }


  void DSLightPassTech::SetWVP(const Matrix4f& worlViewProjection)
  {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(worlViewProjection.m));
  }


  void DSLightPassTech::SetPositionTextureUnit(uint32_t textureUnit)
  {
    glUniform1i(m_posTextureUnitLocation, textureUnit);
  }


  void DSLightPassTech::SetColorTextureUnit(uint32_t textureUnit)
  {
    glUniform1i(m_colorTextureUnitLocation, textureUnit);
  }


  void DSLightPassTech::SetNormalTextureUnit(uint32_t textureUnit)
  {
    glUniform1i(m_normalTextureUnitLocation, textureUnit);
  }


  void DSLightPassTech::SetEyeWorldPos(const Vector3f& EyePos)
  {
    glUniform3f(m_eyeWorldPosLocation, EyePos.x, EyePos.y, EyePos.z);
  }


  void DSLightPassTech::SetScreenSize(uint32_t width, uint32_t height)
  {
    glUniform2f(m_screenSizeLocation, static_cast<GLfloat>(width), static_cast<GLfloat>(height));
  }
}