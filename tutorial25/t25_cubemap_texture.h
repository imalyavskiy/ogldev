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

#ifndef CUBEMAP_H
#define	CUBEMAP_H

#include <string>
#include <GL/glew.h>

namespace t25
{
  class CubemapTexture
  {
  public:
    
    CubemapTexture(const std::string& directory,
                   const std::string& posXFilename,
                   const std::string& negXFilename,
                   const std::string& posYFilename,
                   const std::string& negYFilename,
                   const std::string& posZFilename,
                   const std::string& negZFilename);

    ~CubemapTexture();
    
    bool Load();

    void Bind(GLenum TextureUnit);

  private:
   
    std::string m_fileNames[6];
    GLuint m_textureObj;
  };
}

#endif	/* CUBEMAP_H */

