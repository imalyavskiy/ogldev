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

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "t33_technique.h"
#include "t33_util.h"

namespace t33 {
  static const char* pVSName = "VS";
  static const char* pTessCSName = "TessCS";
  static const char* pTessESName = "TessES";
  static const char* pGSName = "GS";
  static const char* pFSName = "FS";

  const char* ShaderType2ShaderName(GLuint Type)
  {
    switch (Type) {
    case GL_VERTEX_SHADER:
      return pVSName;
    case GL_TESS_CONTROL_SHADER:
      return pTessCSName;
    case GL_TESS_EVALUATION_SHADER:
      return pTessESName;
    case GL_GEOMETRY_SHADER:
      return pGSName;
    case GL_FRAGMENT_SHADER:
      return pFSName;
    default:
      assert(0);
    }

    return NULL;
  }

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
  bool Technique::AddShader(GLenum ShaderType, const char* pShaderText)
  {
    const GLuint shaderObj = glCreateShader(ShaderType);

    if (shaderObj == 0) {
      fprintf(stderr, "Error creating shader type %d\n", ShaderType);
      return false;
    }

    // Save the shader object - will be deleted in the destructor
    m_shaderObjList.push_back(shaderObj);

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint lengths[1];
    lengths[0] = strlen(pShaderText);
    glShaderSource(shaderObj, 1, p, lengths);

    glCompileShader(shaderObj);

    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
      GLchar infoLog[1024] = {};
      glGetShaderInfoLog(shaderObj, 1024, nullptr, infoLog);
      fprintf(stderr, "Error compiling %s: '%s'\n", ShaderType2ShaderName(ShaderType), infoLog);
      return false;
    }

    glAttachShader(m_shaderProg, shaderObj);

    return GLCheckError();
  }


  // After all the shaders have been added to the program call this function
  // to link and validate the program.
  bool Technique::Finalize()
  {
    GLint success = 0;
    GLchar errorLog[1024] = {};

    glLinkProgram(m_shaderProg);

    glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &success);
    if (success == 0) {
      glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), nullptr, errorLog);
      fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
      return false;
    }

    glValidateProgram(m_shaderProg);
    glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), nullptr, errorLog);
      fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
      return false;
    }

    // Delete the intermediate shader objects that have been added to the program
    for (auto it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it)
      glDeleteShader(*it);

    m_shaderObjList.clear();

    return GLCheckError();
  }


  void Technique::Enable()
  {
    glUseProgram(m_shaderProg);
  }


  GLint Technique::GetUniformLocation(const char* pUniformName)
  {
    const GLuint location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (location == INVALID_OGL_VALUE) {
      fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return location;
  }

  GLint Technique::GetProgramParam(GLint param)
  {
    GLint ret;
    glGetProgramiv(m_shaderProg, param, &ret);
    return ret;
  }
}