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

#ifndef LIGHTS_COMMON_H
#define	LIGHTS_COMMON_H

#include "t36_math_3d.h"

#define COLOR_WHITE Vector3f(1.0f, 1.0f, 1.0f)
#define COLOR_RED Vector3f(1.0f, 0.0f, 0.0f)
#define COLOR_GREEN Vector3f(0.0f, 1.0f, 0.0f)
#define COLOR_CYAN Vector3f(0.0f, 1.0f, 1.0f)
#define COLOR_BLUE Vector3f(0.0f, 0.0f, 1.0f)

namespace t36
{
  struct BaseLight
  {
    Vector3f Color= { 0.0f, 0.0f, 0.0f };
    float AmbientIntensity = 0.f;
    float DiffuseIntensity = 0.f;
  };

  struct DirectionalLight : public BaseLight
  {
    Vector3f Direction = {0.f, 0.f, 0.f};
  };

  struct PointLight : public BaseLight
  {
    Vector3f Position = { 0.f, 0.f, 0.f };

    struct
    {
      float Constant;
      float Linear;
      float Exp;
    } Attenuation = {0.f, 0.f, 0.f};
  };

  struct SpotLight : public PointLight
  {
    Vector3f Direction = { 0.f, 0.f, 0.f };
    float Cutoff = 0.f;
  };
}

#endif
