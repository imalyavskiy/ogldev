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

#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>
#include <GL/glew.h>
#include <FreeImage.h>

namespace t28
{
  class Texture
  {
  public:
    Texture(GLenum textureTarget, std::string fileName);

    bool Load() const;

    void Bind(const GLenum textureUnit) const;

  private:
    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
  };

  FIBITMAP* GenericLoader(const char* lpszPathName, int flag);
}

#endif	/* TEXTURE_H */