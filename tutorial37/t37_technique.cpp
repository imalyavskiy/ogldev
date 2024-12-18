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

#include "t37_technique.h"
#include "t37_util.h"
#include "ogldev_util.h"

namespace t37
{
  Technique::Technique()
  {
    m_shaderProg = 0;
  }


  Technique::~Technique()
  {
    // Delete the intermediate shader objects that have been added to the program
    // The list will only contain something if shaders were compiled but the object itself
    // was destroyed prior to linking.
    for (auto it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it)
      glDeleteShader(*it);

    if (m_shaderProg != 0)
    {
      glDeleteProgram(m_shaderProg);
      m_shaderProg = 0;
    }
  }


  bool Technique::Init()
  {
    m_shaderProg = glCreateProgram();

    if (m_shaderProg == 0) {
      fprintf(stderr, "Error creating shader program\n");
      return false;
    }

    return true;
  }

  // Use this method to add shaders to the program. When finished - call finalize()
  bool Technique::AddShader(GLenum shaderType, const char* pFilename)
  {
    std::string s;

    if (!ReadFile(pFilename, s)) {
      return false;
    }

    const GLuint shaderObj = glCreateShader(shaderType);

    if (shaderObj == 0) {
      fprintf(stderr, "Error creating shader type %d\n", shaderType);
      return false;
    }

    // Save the shader object - will be deleted in the destructor
    m_shaderObjList.push_back(shaderObj);

    const GLchar* p[1];
    p[0] = s.c_str();
    const GLint lengths[1] = { static_cast<GLint>(s.size()) };

    glShaderSource(shaderObj, 1, p, lengths);

    glCompileShader(shaderObj);

    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
      GLchar InfoLog[1024];
      glGetShaderInfoLog(shaderObj, 1024, nullptr, InfoLog);
      fprintf(stderr, "Error compiling '%s': '%s'\n", pFilename, InfoLog);
      return false;
    }

    glAttachShader(m_shaderProg, shaderObj);

    return true;
  }


  // After all the shaders have been added to the program call this function
  // to link and validate the program.
  bool Technique::Finalize()
  {
    GLint success = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(m_shaderProg);

    glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &success);

    if (success == 0) {
      glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), nullptr, errorLog);
      fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
      return false;
    }

    glValidateProgram(m_shaderProg);

    glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &success);

    if (success == 0) {
      glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), nullptr, errorLog);
      fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
      return false;
    }

    // Delete the intermediate shader objects that have been added to the program
    for (auto it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it) {
      glDeleteShader(*it);
    }

    m_shaderObjList.clear();

    //    PrintUniformList();

    return GLCheckError();
  }


  void Technique::PrintUniformList()
  {
    int count = 0;
    glGetProgramiv(m_shaderProg, GL_ACTIVE_UNIFORMS, &count);
    printf("Active Uniforms: %d\n", count);

    GLint size;
    GLenum type;
    constexpr GLsizei bufSize = 16;
    GLsizei length;

    for (int i = 0; i < count; i++) {
      GLchar name[bufSize];

      glGetActiveUniform(m_shaderProg, static_cast<GLuint>(i), bufSize, &length, &size, &type, name);

      printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
    }
  }


  void Technique::Enable()
  {
    glUseProgram(m_shaderProg);
  }


  GLint Technique::GetUniformLocation(const char* pUniformName)
  {
    const GLuint location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (location == INVALID_UNIFORM_LOCATION) {
      fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return location;
  }
}