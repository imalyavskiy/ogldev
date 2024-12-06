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

#ifndef PICKING_TECHNIQUE_H
#define	PICKING_TECHNIQUE_H

#include "t29_technique.h"
#include "t29_math_3d.h"
#include "t29_render_callbacks.h"

namespace t29
{
  class PickingTechnique
  : public Technique
  , public IRenderCallbacks
  {
  public:

    PickingTechnique();

    bool Init() override;

    void SetWVP(const Matrix4f& WVP);

    void SetObjectIndex(unsigned int objectIndex);
    
    void DrawStartCB(unsigned int drawIndex) override;
    
  private:
    
    GLuint m_WVPLocation = GL_INVALID_VALUE;
    GLuint m_drawIndexLocation = GL_INVALID_VALUE;
    GLuint m_objectIndexLocation = GL_INVALID_VALUE;
  };
}
#endif	/* PICKING_TECHNIQUE_H */

