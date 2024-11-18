#include <cstdio>
#include <cstring>

#include "t17_technique.h"
namespace t17
{
  Technique::Technique() {
    m_shaderProg = 0;
  }

  Technique::~Technique() {
    for (auto it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it) {
      glDeleteShader(*it);
    }

    if (m_shaderProg != 0) {
      glDeleteProgram(m_shaderProg);
      m_shaderProg = 0;
    }
  }

  bool Technique::Init() {
    m_shaderProg = glCreateProgram();

    if (m_shaderProg == 0) {
      fprintf(stderr, "Error creating shader program\n");
      return false;
    }

    return true;
  }

  bool Technique::AddShader(GLenum shaderType, const char* pShaderText) {
    const GLuint shaderObj = glCreateShader(shaderType);

    if (shaderObj == 0) {
      fprintf(stderr, "Error creating shader type %d\n", shaderType);
      return false;
    }

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
      GLchar infoLog[1024];
      glGetShaderInfoLog(shaderObj, 1024, nullptr, infoLog);
      fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog);
      return false;
    }

    glAttachShader(m_shaderProg, shaderObj);

    return true;
  }

  bool Technique::Finalize() {
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

    for (auto it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it) {
      glDeleteShader(*it);
    }

    m_shaderObjList.clear();

    return true;
  }

  void Technique::Enable() {
    glUseProgram(m_shaderProg);
  }

  GLint Technique::GetUniformLocation(const char* pUniformName) {
    GLint location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (location == 0xFFFFFFFF) {
      fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return location;
  }
}
