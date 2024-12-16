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

#include "t37_ds_point_light_pass_tech.h"
#include "t37_util.h"


namespace t37
{
  DSPointLightPassTech::DSPointLightPassTech()
  {
  }

  bool DSPointLightPassTech::Init()
  {
    if (!Technique::Init())
      return false;

    if (!AddShader(GL_VERTEX_SHADER, "shaders/light_pass.vs"))
      return false;


    if (!AddShader(GL_FRAGMENT_SHADER, "shaders/point_light_pass.fs"))
      return false;

    if (!Finalize())
      return false;

    m_pointLightLocation.Color = GetUniformLocation("gPointLight.Base.Color");
    if (m_pointLightLocation.Color == INVALID_UNIFORM_LOCATION)
      return false;

    m_pointLightLocation.AmbientIntensity = GetUniformLocation("gPointLight.Base.AmbientIntensity");
    if (m_pointLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION)
      return false;

    m_pointLightLocation.Position = GetUniformLocation("gPointLight.Position");
    if (m_pointLightLocation.Position == INVALID_UNIFORM_LOCATION)
      return false;

    m_pointLightLocation.DiffuseIntensity = GetUniformLocation("gPointLight.Base.DiffuseIntensity");
    if (m_pointLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION)
      return false;

    m_pointLightLocation.Atten.Constant = GetUniformLocation("gPointLight.Atten.Constant");
    if (m_pointLightLocation.Atten.Constant == INVALID_UNIFORM_LOCATION)
      return false;

    m_pointLightLocation.Atten.Linear = GetUniformLocation("gPointLight.Atten.Linear");
    if (m_pointLightLocation.Atten.Linear == INVALID_UNIFORM_LOCATION)
      return false;

    m_pointLightLocation.Atten.Exp = GetUniformLocation("gPointLight.Atten.Exp");
    if (m_pointLightLocation.Atten.Exp == INVALID_UNIFORM_LOCATION)
      return false;

    return DSLightPassTech::Init();
  }

  void DSPointLightPassTech::SetPointLight(const PointLight& light)
  {
    glUniform3f(m_pointLightLocation.Color, light.Color.x, light.Color.y, light.Color.z);
    glUniform1f(m_pointLightLocation.AmbientIntensity, light.AmbientIntensity);
    glUniform1f(m_pointLightLocation.DiffuseIntensity, light.DiffuseIntensity);
    glUniform3f(m_pointLightLocation.Position, light.Position.x, light.Position.y, light.Position.z);
    glUniform1f(m_pointLightLocation.Atten.Constant, light.Attenuation.Constant);
    glUniform1f(m_pointLightLocation.Atten.Linear, light.Attenuation.Linear);
    glUniform1f(m_pointLightLocation.Atten.Exp, light.Attenuation.Exp);
  }
}