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

#ifndef BILLBOARD_TECHNIQUE_H
#define	BILLBOARD_TECHNIQUE_H

#include "t28_technique.h"
#include "t28_math_3d.h"

namespace t28
{
  class BillboardTechnique : public Technique 
  {
  public:
    
    BillboardTechnique();;

    bool Init() override;
    
    void SetVP(const Matrix4f& VP);
    void SetCameraPosition(const Vector3f& pos);
    void SetColorTextureUnit(unsigned int textureUnit);
    void SetBillboardSize(float billboardSize);
    
  private:

    GLuint m_VPLocation;
    GLuint m_cameraPosLocation;
    GLuint m_colorMapLocation;
    GLuint m_billboardSizeLocation;
  };
}

#endif	/* BILLBOARD_TECHNIQUE_H */
