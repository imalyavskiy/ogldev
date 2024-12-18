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

#include "t37_ds_light_pass_tech.h"
#include "t37_util.h"

namespace t37
{
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


  void DSLightPassTech::SetWVP(const Matrix4f& worldViewProjection)
  {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(worldViewProjection.m));
  }


  void DSLightPassTech::SetPositionTextureUnit(unsigned int textureUnit)
  {
    glUniform1i(m_posTextureUnitLocation, textureUnit);
  }


  void DSLightPassTech::SetColorTextureUnit(unsigned int textureUnit)
  {
    glUniform1i(m_colorTextureUnitLocation, textureUnit);
  }


  void DSLightPassTech::SetNormalTextureUnit(unsigned int textureUnit)
  {
    glUniform1i(m_normalTextureUnitLocation, textureUnit);
  }


  void DSLightPassTech::SetEyeWorldPos(const Vector3f& eyePos)
  {
    glUniform3f(m_eyeWorldPosLocation, eyePos.x, eyePos.y, eyePos.z);
  }


  void DSLightPassTech::SetScreenSize(unsigned int width, unsigned int height)
  {
    glUniform2f(m_screenSizeLocation, static_cast<float>(width), static_cast<float>(height));
  }
}