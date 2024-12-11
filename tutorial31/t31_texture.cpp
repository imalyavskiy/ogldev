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

#include <cassert>
#include <iostream>
#include "t31_texture.h"
namespace t31 {
  FIBITMAP* GenericLoader(const char* lpszPathName, int flag) {
    auto fif = FIF_UNKNOWN;

    fif = FreeImage_GetFileType(lpszPathName, 0);
    if (fif == FIF_UNKNOWN) {
      fif = FreeImage_GetFIFFromFilename(lpszPathName);
    }

    if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
      return FreeImage_Load(fif, lpszPathName, flag);
    }

    return nullptr;
  }

  bool GenericWriter(FIBITMAP* dib, const std::string& lpszPathName, int flag) {
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    BOOL bSuccess = FALSE;

    if (dib) {
      fif = FreeImage_GetFIFFromFilename(lpszPathName.c_str());
      if (fif != FIF_UNKNOWN) {
        WORD bpp = FreeImage_GetBPP(dib);
        if (FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp)) {
          bSuccess = FreeImage_Save(fif, dib, lpszPathName.c_str(), flag);
        }
      }
    }
    return (bSuccess == TRUE) ? true : false;
  }

  Texture::Texture(GLenum textureTarget, std::string fileName)
    : m_fileName(std::move(fileName))
    , m_textureTarget(textureTarget)
  {
    glGenTextures(1, &m_textureObj);
  }

  bool Texture::Load() const
  {
    auto src = GenericLoader(m_fileName.c_str(), 0);
    if (!src)
      return false;

    const auto type = FreeImage_GetColorType(src);
    if (!(type == FIC_RGBALPHA || type == FIC_RGB))
      return false;

    if (!FreeImage_HasPixels(src))
      return false;

    const auto bpp = FreeImage_GetBPP(src);
    assert((type == FIC_RGBALPHA && 32 == bpp) || (type == FIC_RGB && 24 == bpp));

    const GLsizei width = FreeImage_GetWidth(src);
    const GLsizei height = FreeImage_GetHeight(src);

    FreeImage_FlipVertical(src);

    const auto data = FreeImage_GetBits(src);

    glBindTexture(m_textureTarget, m_textureObj);
    if (type == FIC_RGBALPHA && 32 == bpp)
      glTexImage2D(m_textureTarget, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    else if (type == FIC_RGB && 24 == bpp)
      glTexImage2D(m_textureTarget, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    else
      throw std::logic_error("Unsupported color format");

    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(src);

    return true;
  }

  void Texture::Bind(const GLenum textureUnit) const
  {
    glActiveTexture(textureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
  }
}