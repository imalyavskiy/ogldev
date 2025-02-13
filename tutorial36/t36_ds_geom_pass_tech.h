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

#ifndef DS_GEOM_PASS_TECH_H
#define	DS_GEOM_PASS_TECH_H

#include "t36_technique.h"
#include "t36_math_3d.h"

namespace t36
{
    class DSGeomPassTech : public Technique {
    public:

      DSGeomPassTech();

      bool Init() override;

      void SetWVP(const Matrix4f& worldViewProjection);
      void SetWorldMatrix(const Matrix4f& world);
      void SetColorTextureUnit(uint32_t textureUnit);

    private:

      GLuint m_WVPLocation;
      GLuint m_WorldMatrixLocation;
      GLuint m_colorTextureUnitLocation;
    };
}

#endif