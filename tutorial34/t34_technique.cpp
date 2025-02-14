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

#include <cstdio>
#include <string>
#include <cassert>

#include <GL/glfx.h>

#include "t34_technique.h"
#include "t34_util.h"

using namespace std;

namespace t34 {
  Technique::Technique(const char* pEffectFile)
  : m_effect(glfxGenEffect())
  , m_pEffectFile(pEffectFile)
  {
  }


  Technique::~Technique()
  {
    if (m_shaderProg != 0)
    {
      glDeleteProgram(m_shaderProg);
      m_shaderProg = 0;
    }

    glfxDeleteEffect(m_effect);
  }

  bool Technique::CompileProgram(const char* pProgram)
  {
    if (!glfxParseEffectFromFile(m_effect, m_pEffectFile)) {
      const string log = glfxGetEffectLog(m_effect);
      printf("Error creating effect from file '%s':\n", m_pEffectFile);
      printf("%s\n", log.c_str());
      return false;
    }

    m_shaderProg = glfxCompileProgram(m_effect, pProgram);

    if (m_shaderProg < 0) {
      const string log = glfxGetEffectLog(m_effect);
      printf("Error compiling program '%s' in effect file '%s':\n", pProgram, m_pEffectFile);
      printf("%s\n", log.c_str());
      return false;
    }

    return true;
  }

  void Technique::Enable()
  {
    glUseProgram(m_shaderProg);
  }


  GLint Technique::GetUniformLocation(const char* pUniformName)
  {
    GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == INVALID_OGL_VALUE) {
      fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
  }

  GLint Technique::GetProgramParam(GLint param)
  {
    GLint ret;
    glGetProgramiv(m_shaderProg, param, &ret);
    return ret;
  }
}