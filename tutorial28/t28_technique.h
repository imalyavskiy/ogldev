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
#include <string>
#include <GL/glew.h>

namespace t28
{
  class Technique
  {
  public:

    Technique(std::string name);

    virtual ~Technique();

    virtual bool Init();

    void Enable();
    
  protected:

    bool AddShader(GLenum shaderType, const char* pShaderText);

    bool Finalize();

    GLint GetUniformLocation(const char* pUniformName);
    
    GLint GetProgramParam(GLint param);
    
    GLuint m_shaderProg = 0;    
    
  private:

    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList m_shaderObjList;

    std::string m_name;
  };
}
#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF


#endif	/* TECHNIQUE_H */

