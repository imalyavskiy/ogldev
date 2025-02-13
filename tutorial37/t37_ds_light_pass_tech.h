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

#ifndef DS_LIGHT_PASS_TECH_H
#define	DS_LIGHT_PASS_TECH_H

#include <string>

#include "t37_technique.h"
#include "t37_math_3d.h"

namespace t37
{
  class DSLightPassTech : public Technique {
  public:

    DSLightPassTech() = default;

    bool Init() override;

    void SetWVP(const Matrix4f& worldViewProjection);
    void SetPositionTextureUnit(unsigned int textureUnit);
    void SetColorTextureUnit(unsigned int textureUnit);
    void SetNormalTextureUnit(unsigned int textureUnit);
    void SetEyeWorldPos(const Vector3f& eyeWorldPos);
    void SetMatSpecularIntensity(float intensity);
    void SetMatSpecularPower(float power);
    void SetScreenSize(unsigned int width, unsigned int height);

  private:

    GLuint m_WVPLocation;
    GLuint m_posTextureUnitLocation;
    GLuint m_normalTextureUnitLocation;
    GLuint m_colorTextureUnitLocation;
    GLuint m_eyeWorldPosLocation;
    GLuint m_matSpecularIntensityLocation;
    GLuint m_matSpecularPowerLocation;
    GLuint m_screenSizeLocation;
  };
}

#endif