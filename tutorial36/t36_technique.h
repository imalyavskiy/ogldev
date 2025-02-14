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

#ifndef TECHNIQUE_H
#define	TECHNIQUE_H

#include <list>
#include <GL/glew.h>

namespace t36
{
  class Technique
  {
  public:

    Technique();

    virtual ~Technique();

    virtual bool Init();

    void Enable();

    [[nodiscard]]
    GLuint GetProgram() const;

  protected:

    bool AddShader(GLenum shaderType, const char* pFileName);

    bool Finalize();

    GLint GetUniformLocation(const char* pUniformName);

    GLuint m_shaderProg = 0;

  private:

    void PrintUniformList();

    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList m_shaderObjList;
  };
}

#endif	/* TECHNIQUE_H */

