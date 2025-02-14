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

#ifndef DS_POINT_LIGHT_PASS_TECH_H
#define	DS_POINT_LIGHT_PASS_TECH_H

#include "t36_ds_light_pass_tech.h"
#include "t36_lights_common.h"

namespace t36
{
  class DSPointLightPassTech : public DSLightPassTech
  {
  public:

    DSPointLightPassTech();

    bool Init() override;

    void SetPointLight(const PointLight& lights);

  private:

    struct {
      GLuint Color;
      GLuint AmbientIntensity;
      GLuint DiffuseIntensity;
      GLuint Position;
      struct {
        GLuint Constant;
        GLuint Linear;
        GLuint Exp;
      } Atten;
    } m_pointLightLocation;
  };
}
#endif
