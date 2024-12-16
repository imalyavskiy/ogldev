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

#include "t37_ds_dir_light_pass_tech.h"
#include "t37_util.h"

namespace t37
{
    DSDirLightPassTech::DSDirLightPassTech()
    {
    }

    bool DSDirLightPassTech::Init()
    {
      if (!Technique::Init())
        return false;

      if (!AddShader(GL_VERTEX_SHADER, "shaders/light_pass.vs"))
        return false;

      if (!AddShader(GL_FRAGMENT_SHADER, "shaders/dir_light_pass.fs"))
        return false;

      if (!Finalize())
        return false;

      m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
      if (m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION)
        return false;

      m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
      if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION)
        return false;

      m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
      if (m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION)
        return false;

      m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
      if (m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION)
        return false;

      return DSLightPassTech::Init();
    }



    void DSDirLightPassTech::SetDirectionalLight(const DirectionalLight& light)
    {
      glUniform3f(m_dirLightLocation.Color, light.Color.x, light.Color.y, light.Color.z);
      glUniform1f(m_dirLightLocation.AmbientIntensity, light.AmbientIntensity);

      Vector3f direction = light.Direction;
      direction.Normalize();

      glUniform3f(m_dirLightLocation.Direction, direction.x, direction.y, direction.z);
      glUniform1f(m_dirLightLocation.DiffuseIntensity, light.DiffuseIntensity);
    }
}
